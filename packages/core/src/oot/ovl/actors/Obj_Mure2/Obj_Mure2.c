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
        if (this->actorSpawnPtrList[i] != NULL) {
            continue;
        }

        if (((this->currentActorNum >> i) & 1) == 0) {
            this->actorSpawnPtrList[i] =
                Actor_Spawn(&play->actorCtx, play, sActorSpawnIDs[actorNum], spawnPos[i].x, spawnPos[i].y,
                            spawnPos[i].z, this->actor.world.rot.x, 0, this->actor.world.rot.z, params);
            if (this->actorSpawnPtrList[i] != NULL) {
                this->actorSpawnPtrList[i]->room = this->actor.room;
            }
        }
    }
}

void ObjMure2_CleanupAndDie(Actor_ObjMure2* this, GameState_Play* play)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++)
    {
        if (((this->currentActorNum >> i) & 1) == 0)
        {
            if (this->actorSpawnPtrList[i] != NULL)
            {
                if (Actor_HasParent(this->actorSpawnPtrList[i], play))
                {
                    this->currentActorNum |= (1 << i);
                }
                else
                {
                    Actor_Kill(this->actorSpawnPtrList[i]);
                }
                this->actorSpawnPtrList[i] = NULL;
            }
        }
        else
        {
            this->actorSpawnPtrList[i] = NULL;
        }
    }
}

void func_80B9A534(Actor_ObjMure2* this)
{
    s32 i;

    for (i = 0; i < D_80B9A818[PARAMS_GET_U(this->actor.params, 0, 2)]; i++) {
        if (this->actorSpawnPtrList[i] != NULL && (((this->currentActorNum >> i) & 1) == 0) &&
            (this->actorSpawnPtrList[i]->update == NULL)) {
            this->currentActorNum |= (1 << i);
            this->actorSpawnPtrList[i] = NULL;
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
    if (Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z) < (sDistSquared1[PARAMS_GET_U(this->actor.params, 0, 2)] * this->unk_184))
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
    if ((sDistSquared2[PARAMS_GET_U(this->actor.params, 0, 2)] * this->unk_184) <= Math3D_Dist1DSq(this->actor.projectedPos.x, this->actor.projectedPos.z))
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
        this->unk_184 = 1.0f;
    }
    else
    {
        this->unk_184 = 4.0f;
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
