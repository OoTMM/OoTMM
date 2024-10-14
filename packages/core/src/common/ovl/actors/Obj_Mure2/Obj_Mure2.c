#include "Obj_Mure2.h"
#include <combo/global.h>

#define FLAGS 0

#define OBJ_MURE2_CHILD_COUNT_BUSH_RING         9
#define OBJ_MURE2_CHILD_COUNT_BUSH_SCATTERED    12
#define OBJ_MURE2_CHILD_COUNT_ROCK_RING         8

typedef enum Mure2ChildType
{
    OBJMURE2_CHILDTYPE_BUSH_RING,
    OBJMURE2_CHILDTYPE_BUSH_SCATTERED,
    OBJMURE2_CHILDTYPE_ROCK_RING,
    OBJMURE2_CHILDTYPE_MAX
}
Mure2ChildType;

static f32 sActivationRangesSq[OBJMURE2_CHILDTYPE_MAX] = {
    SQ(1600.0f), // OBJMURE2_CHILDTYPE_BUSH_RING
    SQ(1600.0f), // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    SQ(1600.0f), // OBJMURE2_CHILDTYPE_ROCK_RING
};

static f32 sDeactivationRangesSq[OBJMURE2_CHILDTYPE_MAX] = {
    SQ(1705.0f), // OBJMURE2_CHILDTYPE_BUSH_RING
    SQ(1705.0f), // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    SQ(1705.0f), // OBJMURE2_CHILDTYPE_ROCK_RING
};

static s16 sChildCounts[OBJMURE2_CHILDTYPE_MAX] = {
    OBJ_MURE2_CHILD_COUNT_BUSH_RING,      // OBJMURE2_CHILDTYPE_BUSH_RING
    OBJ_MURE2_CHILD_COUNT_BUSH_SCATTERED, // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    OBJ_MURE2_CHILD_COUNT_ROCK_RING,      // OBJMURE2_CHILDTYPE_ROCK_RING
};

static s16 sActorIds[OBJMURE2_CHILDTYPE_MAX] = {
    AC_EN_KUSA, // OBJMURE2_CHILDTYPE_BUSH_RING
    AC_EN_KUSA, // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    AC_EN_ISHI, // OBJMURE2_CHILDTYPE_ROCK_RING
};

static VecPolarS sScatteredBushSpawnInfo[OBJ_MURE2_CHILD_MAX] = {
    { 40, 0x0666 }, { 40, 0x2CCC }, { 40, 0x5999 },  { 40, -0x799A }, { 20, -0x4000 }, { 80, 0x1333 },
    { 80, 0x4000 }, { 80, 0x6CCC }, { 80, -0x6CCD }, { 80, -0x5334 }, { 80, -0x399A }, { 60, -0x2000 },
};

void ObjMure2_Init(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_Update(Actor_ObjMure2* this, GameState_Play* play);

void ObjMure2_SpawnChildren(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_Wait(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_WaitForPlayerInRange(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_WaitForPlayerOutOfRange(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_KillChildren(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_SetupWait(Actor_ObjMure2* this);
void ObjMure2_SetupWaitForPlayerInRange(Actor_ObjMure2* this);
void ObjMure2_SetupWaitForPlayerOutOfRange(Actor_ObjMure2* this);
void ObjMure2_GetChildParams(s16*, Actor_ObjMure2*);

void ObjMure2_GetBushCircleSpawnPos(Vec3f pos[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2* this)
{
    Math_Vec3f_Copy(&pos[0], &this->actor.world.pos);
    for (int i = 1; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++)
    {
        Math_Vec3f_Copy(&pos[i], &this->actor.world.pos);
        (pos + i)->x += 80.0f * Math_SinS((i - 1) * 0x2000);
        (pos + i)->z += 80.0f * Math_CosS((i - 1) * 0x2000);
    }
}

void ObjMure2_GetBushScatteredPos(Vec3f pos[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2* this)
{
    for (int i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++)
    {
        Math_Vec3f_Copy(pos + i, &this->actor.world.pos);
        (pos + i)->x += sScatteredBushSpawnInfo[i].distance * Math_CosS(sScatteredBushSpawnInfo[i].angle);
        (pos + i)->z -= sScatteredBushSpawnInfo[i].distance * Math_SinS(sScatteredBushSpawnInfo[i].angle);
    }
}

void ObjMure2_GetRocksSpawnPos(Vec3f pos[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2* this)
{
    for (int i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++)
    {
        Math_Vec3f_Copy(&pos[i], &this->actor.world.pos);
        (pos + i)->x += 80.0f * Math_SinS(i * 0x2000);
        (pos + i)->z += 80.0f * Math_CosS(i * 0x2000);
    }
}

#if defined(GAME_OOT)
void ObjMure2_GetChildParams(s16* params, Actor_ObjMure2* this)
{
    s16 dropTable;

    dropTable = OBJ_MURE2_GET_CHILD_UPPER_PARAMS(&this->actor);
    if (dropTable >= 13)
        dropTable = 0;

    *params = (dropTable << 8);
}
#endif

#if defined(GAME_MM)
void ObjMure2_GetChildParams(s16* childParams, Actor_ObjMure2* this)
{
    Mure2ChildType childType;
    s32 dropTable;

    childType = OBJ_MURE2_GET_CHILD_TYPE(&this->actor);
    dropTable = OBJ_MURE2_GET_CHILD_UPPER_PARAMS(&this->actor);
    if (childType == OBJMURE2_CHILDTYPE_ROCK_RING)
        *childParams = dropTable << 4;
    else
        *childParams = dropTable << 8;
}
#endif

Actor* ObjMure2_ActorSpawn(Actor_ObjMure2* this, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
#if defined(GAME_OOT)
    return Actor_Spawn(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
#endif

#if defined(GAME_MM)
    return Actor_SpawnAsChildAndCutscene(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable, this->actor.csId, this->actor.halfDaysBits, NULL);
#endif
}

Actor* ObjMure2_ActorSpawnEx(Actor_ObjMure2* this, GameState_Play* play, int i, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    Actor* tmp;

    memcpy(&g.xflag, &this->xflag, sizeof(Xflag));
    g.xflag.sliceId = (u8)i;
    g.xflagOverride = TRUE;
    tmp = ObjMure2_ActorSpawn(this, play, actorId, x, y, z, rx, ry, rz, variable);
    g.xflagOverride = FALSE;

    return tmp;
}

typedef void (*ObjMure2SpawnPosFunc)(Vec3f[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2*);
static ObjMure2SpawnPosFunc sSpawnPosFuncs[OBJMURE2_CHILDTYPE_MAX] =
{
    ObjMure2_GetBushCircleSpawnPos,
    ObjMure2_GetBushScatteredPos,
    ObjMure2_GetRocksSpawnPos,
};

void ObjMure2_SpawnChildren(Actor_ObjMure2* this, GameState_Play* play)
{
    Vec3f* pos;
    Mure2ChildType childType;
    Vec3f spawnPos[OBJ_MURE2_CHILD_MAX];
    s16 childParams;

    childType = OBJ_MURE2_GET_CHILD_TYPE(&this->actor);
    sSpawnPosFuncs[childType](spawnPos, this);
    ObjMure2_GetChildParams(&childParams, this);

    for (int i = 0; i < sChildCounts[childType]; i++)
    {
        if (this->actors[i] != NULL)
            continue;

        if (((this->spawnFlags >> i) & 1) == 0)
        {
            pos = &spawnPos[i];
            this->actors[i] = ObjMure2_ActorSpawnEx(this, play, i, sActorIds[childType], pos->x, pos->y, pos->z, this->actor.world.rot.x, 0, this->actor.world.rot.z, childParams);
            if (this->actors[i] != NULL)
                this->actors[i]->room = this->actor.room;
        }
    }
}

void ObjMure2_KillChildren(Actor_ObjMure2* this, GameState_Play* play)
{
    for (int i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++)
    {
        if (((this->spawnFlags >> i) & 1) == 0)
        {
            if (this->actors[i] != NULL)
            {
                if (Actor_HasParentZ(this->actors[i]))
                {
                    this->spawnFlags |= 1 << i;
                }
                else
                {
                    Actor_Kill(this->actors[i]);
                }
                this->actors[i] = NULL;
            }
        }
        else
        {
            this->actors[i] = NULL;
        }
    }
}

void ObjMure2_ClearChildrenList(Actor_ObjMure2* this)
{
    for (int i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++)
    {
        if (this->actors[i] == NULL)
            continue;

        if ((((this->spawnFlags >> i) & 1) == 0) && (this->actors[i]->update == NULL))
        {
            this->spawnFlags |= (1 << i);
            this->actors[i] = NULL;
        }
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 2100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

void ObjMure2_Init(Actor_ObjMure2* this, GameState_Play* play)
{
    comboXflagInit(&this->xflag, &this->actor, play);

    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (play->csCtx.state != CS_STATE_IDLE)
        this->actor.uncullZoneForward += 1200.0f;
    ObjMure2_SetupWait(this);
}

void ObjMure2_SetupWait(Actor_ObjMure2* this)
{
    this->actionFunc = ObjMure2_Wait;
}

void ObjMure2_Wait(Actor_ObjMure2* this, GameState_Play* play)
{
    ObjMure2_SetupWaitForPlayerInRange(this);
}

void ObjMure2_SetupWaitForPlayerInRange(Actor_ObjMure2* this)
{
    this->actionFunc = ObjMure2_WaitForPlayerInRange;
}

void ObjMure2_WaitForPlayerInRange(Actor_ObjMure2* this, GameState_Play* play)
{
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) <
        sActivationRangesSq[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)] * this->rangeMultiplier) {
        this->actor.flags |= ACTOR_FLAG_OOT_4;
        ObjMure2_SpawnChildren(this, play);
        ObjMure2_SetupWaitForPlayerOutOfRange(this);
    }
}

void ObjMure2_SetupWaitForPlayerOutOfRange(Actor_ObjMure2* this)
{
    this->actionFunc = ObjMure2_WaitForPlayerOutOfRange;
}

void ObjMure2_WaitForPlayerOutOfRange(Actor_ObjMure2* this, GameState_Play* play)
{
    ObjMure2_ClearChildrenList(this);

    if ((sDeactivationRangesSq[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)] * this->rangeMultiplier) <= Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z))
    {
        this->actor.flags &= ~ACTOR_FLAG_OOT_4;
        ObjMure2_KillChildren(this, play);
        ObjMure2_SetupWaitForPlayerInRange(this);
    }
}

void ObjMure2_Update(Actor_ObjMure2* this, GameState_Play* play)
{
    if (play->csCtx.state == CS_STATE_IDLE)
    {
        this->rangeMultiplier = 1.0f;
    }
    else
    {
        this->rangeMultiplier = 4.0f;
    }
    this->actionFunc(this, play);
}

ActorProfile ObjMure2_Profile =
{
    AC_OBJ_MURE2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_ObjMure2),
    (ActorFunc)ObjMure2_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMure2_Update,
    NULL,
};

OVL_ACTOR_INFO(AC_OBJ_MURE2, ObjMure2_Profile);
