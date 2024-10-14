#if defined(GAME_OOT)
#include "Obj_Mure2.h"

#define FLAGS 0

typedef void (*ObjMure2SetPosFunc)(Vec3f* vec, Actor_ObjMure2* this);

typedef struct Mure2sScatteredShrubInfo
{
    s16 radius;
    s16 angle;
}
Mure2sScatteredShrubInfo;

void ObjMure2_Init(Actor_ObjMure2* thisx, GameState_Play* play);
void ObjMure2_Update(Actor_ObjMure2* thisx, GameState_Play* play);

void ObjMure2_SetPosShrubCircle(Vec3f* vec, Actor_ObjMure2* this);
void ObjMure2_SetPosShrubScattered(Vec3f* vec, Actor_ObjMure2* this);
void ObjMure2_SetPosRockCircle(Vec3f* vec, Actor_ObjMure2* this);
void ObjMure2_Wait(Actor_ObjMure2* this, GameState_Play* play);
void func_80B9A668(Actor_ObjMure2* this, GameState_Play* play);
void func_80B9A6F8(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_SetupWait(Actor_ObjMure2* this);
void func_80B9A658(Actor_ObjMure2* this);
void func_80B9A6E8(Actor_ObjMure2* this);

static f32 sDistSquared1[] = { SQ(1600.0f), SQ(1600.0f), SQ(1600.0f) };

static f32 sDistSquared2[] = { SQ(1705.0f), SQ(1705.0f), SQ(1705.0f) };

static s16 D_80B9A818[] = { 9, 12, 8 };

static s16 sActorSpawnIDs[] = { AC_EN_KUSA, AC_EN_KUSA, AC_EN_ISHI };

void ObjMure2_SetPosShrubCircle(Vec3f* vec, Actor_ObjMure2* this) {
    s32 i;

    Math_Vec3f_Copy(vec, &this->actor.world.pos);
    for (i = 1; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &this->actor.world.pos);
        (vec + i)->x += (80.0f * Math_SinS((i - 1) * 0x2000));
        (vec + i)->z += (80.0f * Math_CosS((i - 1) * 0x2000));
    }
}

static Mure2sScatteredShrubInfo sScatteredShrubInfo[] = {
    { 40, 0x0666 }, { 40, 0x2CCC }, { 40, 0x5999 }, { 40, 0x8666 }, { 20, 0xC000 }, { 80, 0x1333 },
    { 80, 0x4000 }, { 80, 0x6CCC }, { 80, 0x9333 }, { 80, 0xACCC }, { 80, 0xC666 }, { 60, 0xE000 },
};

void ObjMure2_SetPosShrubScattered(Vec3f* vec, Actor_ObjMure2* this)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &this->actor.world.pos);
        (vec + i)->x += (sScatteredShrubInfo[i].radius * Math_CosS(sScatteredShrubInfo[i].angle));
        (vec + i)->z -= (sScatteredShrubInfo[i].radius * Math_SinS(sScatteredShrubInfo[i].angle));
    }
}

void ObjMure2_SetPosRockCircle(Vec3f* vec, Actor_ObjMure2* this)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++) {
        Math_Vec3f_Copy(vec + i, &this->actor.world.pos);
        (vec + i)->x += (80.0f * Math_SinS(i * 0x2000));
        (vec + i)->z += (80.0f * Math_CosS(i * 0x2000));
    }
}

void ObjMure2_SetActorSpawnParams(s16* params, Actor_ObjMure2* this)
{
    static s16 actorSpawnParams[] = { 0, 0, 0 };
    s16 dropTable = PARAMS_GET_U(this->actor.params, 8, 4);

    if (dropTable >= 13) {
        dropTable = 0;
    }
    *params = actorSpawnParams[PARAMS_GET_U(this->actor.params, 0, 2)] & 0xF0FF;
    *params |= (dropTable << 8);
}

void ObjMure2_SpawnActors(Actor_ObjMure2* this, GameState_Play* play)
{
    static ObjMure2SetPosFunc setPosFunc[] = {
        ObjMure2_SetPosShrubCircle,
        ObjMure2_SetPosShrubScattered,
        ObjMure2_SetPosRockCircle,
    };
    s32 actorNum = PARAMS_GET_U(this->actor.params, 0, 2);
    s32 i;
    Vec3f spawnPos[12];
    s16 params;

    setPosFunc[actorNum](spawnPos, this);
    ObjMure2_SetActorSpawnParams(&params, this);

    for (i = 0; i < D_80B9A818[actorNum]; i++) {
        if (this->actors[i] != NULL) {
            continue;
        }

        if (((this->spawnFlags >> i) & 1) == 0) {
            this->actors[i] =
                Actor_Spawn(&play->actorCtx, play, sActorSpawnIDs[actorNum], spawnPos[i].x, spawnPos[i].y,
                            spawnPos[i].z, this->actor.world.rot.x, 0, this->actor.world.rot.z, params);
            if (this->actors[i] != NULL) {
                this->actors[i]->room = this->actor.room;
            }
        }
    }
}

void ObjMure2_CleanupAndDie(Actor_ObjMure2* this, GameState_Play* play)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++)
    {
        if (((this->spawnFlags >> i) & 1) == 0)
        {
            if (this->actors[i] != NULL)
            {
                if (Actor_HasParent(this->actors[i], play))
                {
                    this->spawnFlags |= (1 << i);
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

void func_80B9A534(Actor_ObjMure2* this)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++) {
        if (this->actors[i] != NULL && (((this->spawnFlags >> i) & 1) == 0) &&
            (this->actors[i]->update == NULL)) {
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
    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (play->csCtx.state != CS_STATE_IDLE) {
        this->actor.uncullZoneForward += 1200.0f;
    }
    ObjMure2_SetupWait(this);
}

void ObjMure2_SetupWait(Actor_ObjMure2* this)
{
    this->actionFunc = ObjMure2_Wait;
}

void ObjMure2_Wait(Actor_ObjMure2* this, GameState_Play* play)
{
    func_80B9A658(this);
}

void func_80B9A658(Actor_ObjMure2* this)
{
    this->actionFunc = func_80B9A668;
}

void func_80B9A668(Actor_ObjMure2* this, GameState_Play* play)
{
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) < (sDistSquared1[PARAMS_GET_U(this->actor.params, 0, 2)] * this->rangeMultiplier))
    {
        this->actor.flags |= ACTOR_FLAG_OOT_4;
        ObjMure2_SpawnActors(this, play);
        func_80B9A6E8(this);
    }
}

void func_80B9A6E8(Actor_ObjMure2* this) {
    this->actionFunc = func_80B9A6F8;
}

void func_80B9A6F8(Actor_ObjMure2* this, GameState_Play* play)
{
    func_80B9A534(this);
    if ((sDistSquared2[PARAMS_GET_U(this->actor.params, 0, 2)] * this->rangeMultiplier) <= Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z))
    {
        this->actor.flags &= ~ACTOR_FLAG_OOT_4;
        ObjMure2_CleanupAndDie(this, play);
        func_80B9A658(this);
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
#endif

#if defined(GAME_MM)
#include "Obj_Mure2.h"

#define FLAGS 0

#define OBJ_MURE2_CHILD_COUNT_BUSH_RING 9
#define OBJ_MURE2_CHILD_COUNT_BUSH_SCATTERED 12
#define OBJ_MURE2_CHILD_COUNT_ROCK_RING 8

typedef enum Mure2ChildType {
    /* 0 */ OBJMURE2_CHILDTYPE_BUSH_RING,
    /* 1 */ OBJMURE2_CHILDTYPE_BUSH_SCATTERED,
    /* 2 */ OBJMURE2_CHILDTYPE_ROCK_RING,
    /* 3 */ OBJMURE2_CHILDTYPE_MAX
} Mure2ChildType;

void ObjMure2_Init(Actor_ObjMure2* thisx, GameState_Play* play);
void ObjMure2_Update(Actor_ObjMure2* thisx, GameState_Play* play);

void ObjMure2_SpawnChildren(Actor_ObjMure2* this, GameState_Play* play);
void func_809613C4(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_WaitForPlayerInRange(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_WaitForPlayerOutOfRange(Actor_ObjMure2* this, GameState_Play* play);
void ObjMure2_KillChildren(Actor_ObjMure2* this, GameState_Play* play);
void func_809613B0(Actor_ObjMure2* this);
void ObjMure2_SetupWaitForPlayerInRange(Actor_ObjMure2* this);
void ObjMure2_SetupWaitForPlayerOutOfRange(Actor_ObjMure2* this);
void ObjMure2_GetChildParams(s16*, Actor_ObjMure2*);

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

void ObjMure2_GetBushCircleSpawnPos(Vec3f pos[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2* this)
{
    Math_Vec3f_Copy(&pos[0], &this->actor.world.pos);
    for (int i = 1; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++) {
        Math_Vec3f_Copy(&pos[i], &this->actor.world.pos);
        (pos + i)->x += 80.0f * Math_SinS((i - 1) * 0x2000);
        (pos + i)->z += 80.0f * Math_CosS((i - 1) * 0x2000);
    }
}

void ObjMure2_GetBushScatteredPos(Vec3f pos[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2* this)
{
    for (int i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++) {
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

void ObjMure2_GetChildParams(s16* childParams, Actor_ObjMure2* this)
{
    Mure2ChildType childType = OBJ_MURE2_GET_CHILD_TYPE(&this->actor);
    s32 temp_a2 = OBJ_MURE2_GET_CHILD_UPPER_PARAMS(&this->actor);

    if (childType == OBJMURE2_CHILDTYPE_ROCK_RING) {
        *childParams = temp_a2 << 4;
    } else {
        *childParams = temp_a2 << 8;
    }
}

typedef void (*ObjMure2SpawnPosFunc)(Vec3f[OBJ_MURE2_CHILD_MAX], Actor_ObjMure2*);
static ObjMure2SpawnPosFunc sSpawnPosFuncs[OBJMURE2_CHILDTYPE_MAX] = {
    ObjMure2_GetBushCircleSpawnPos, // OBJMURE2_CHILDTYPE_BUSH_RING
    ObjMure2_GetBushScatteredPos,   // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    ObjMure2_GetRocksSpawnPos,      // OBJMURE2_CHILDTYPE_ROCK_RING
};

void ObjMure2_SpawnChildren(Actor_ObjMure2* this, GameState_Play* play)
{
    Vec3f* pos;
    Mure2ChildType childType = OBJ_MURE2_GET_CHILD_TYPE(&this->actor);
    Vec3f spawnPos[OBJ_MURE2_CHILD_MAX];
    s16 childParams;
    s32 i;

    sSpawnPosFuncs[childType](spawnPos, this);
    ObjMure2_GetChildParams(&childParams, this);
    for (i = 0; i < sChildCounts[childType]; i++) {
        if (this->actors[i] != NULL) {
            continue;
        }
        if (((this->spawnFlags >> i) & 1) == 0) {
            pos = &spawnPos[i];
            this->actors[i] = Actor_SpawnAsChildAndCutscene(
                &play->actorCtx, play, sActorIds[childType], pos->x, pos->y, pos->z, this->actor.world.rot.x, 0,
                this->actor.world.rot.z, childParams, this->actor.csId, this->actor.halfDaysBits, NULL);
            if (this->actors[i] != NULL) {
                this->actors[i]->room = this->actor.room;
            }
        }
    }
}

void ObjMure2_KillChildren(Actor_ObjMure2* this, GameState_Play* play) {
    s32 i;

    for (i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++) {
        if (((this->spawnFlags >> i) & 1) == 0) {
            if (this->actors[i] != NULL) {
                if (Actor_HasParent(this->actors[i], play)) {
                    this->spawnFlags |= 1 << i;
                } else {
                    Actor_Kill(this->actors[i]);
                }
                this->actors[i] = NULL;
            }
        } else {
            this->actors[i] = NULL;
        }
    }
}

void ObjMure2_ClearChildrenList(Actor_ObjMure2* this) {
    s32 i;

    for (i = 0; i < sChildCounts[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)]; i++) {
        if (this->actors[i] == NULL) {
            continue;
        }
        if ((((this->spawnFlags >> i) & 1) == 0) && (this->actors[i]->update == NULL)) {
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
    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (play->csCtx.state != CS_STATE_IDLE) {
        this->actor.uncullZoneForward += 1200.0f;
    }
    func_809613B0(this);
}

void func_809613B0(Actor_ObjMure2* this) {
    this->actionFunc = func_809613C4;
}

void func_809613C4(Actor_ObjMure2* this, GameState_Play* play) {
    ObjMure2_SetupWaitForPlayerInRange(this);
}

void ObjMure2_SetupWaitForPlayerInRange(Actor_ObjMure2* this) {
    this->actionFunc = ObjMure2_WaitForPlayerInRange;
}

void ObjMure2_WaitForPlayerInRange(Actor_ObjMure2* this, GameState_Play* play)
{
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) <
        sActivationRangesSq[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)] * this->rangeMultiplier) {
        this->actor.flags |= ACTOR_FLAG_MM_10;
        ObjMure2_SpawnChildren(this, play);
        ObjMure2_SetupWaitForPlayerOutOfRange(this);
    }
}

void ObjMure2_SetupWaitForPlayerOutOfRange(Actor_ObjMure2* this) {
    this->actionFunc = ObjMure2_WaitForPlayerOutOfRange;
}

void ObjMure2_WaitForPlayerOutOfRange(Actor_ObjMure2* this, GameState_Play* play)
{
    ObjMure2_ClearChildrenList(this);

    if ((sDeactivationRangesSq[OBJ_MURE2_GET_CHILD_TYPE(&this->actor)] * this->rangeMultiplier) <=
        Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z)) {
        this->actor.flags &= ~ACTOR_FLAG_MM_10;
        ObjMure2_KillChildren(this, play);
        ObjMure2_SetupWaitForPlayerInRange(this);
    }
}

void ObjMure2_Update(Actor_ObjMure2* this, GameState_Play* play)
{
    if (play->csCtx.state == CS_STATE_IDLE) {
        this->rangeMultiplier = 1.0f;
    } else {
        this->rangeMultiplier = 4.0f;
    }
    this->actionFunc(this, play);
}

ActorProfile Obj_Mure2_Profile =
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

OVL_ACTOR_INFO(AC_OBJ_MURE2, Obj_Mure2_Profile);
#endif
