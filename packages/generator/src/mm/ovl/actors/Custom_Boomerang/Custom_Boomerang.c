#include <combo.h>
#include <combo/custom.h>
#include <combo/player.h>
#include <combo/mm/player.h>
#include <combo/math.h>
#include <combo/global.h>
#include <combo/effect.h>
#include <combo/collision.h>
#include "Custom_Boomerang.h"

typedef struct EffectBlureInit1 {
    char unk_00[0x184];
    u8 p1StartColor[4];
    u8 p2StartColor[4];
    u8 p1EndColor[4];
    u8 p2EndColor[4];
    s32 elemDuration;
    s32 unkFlag;
    s32 calcMode;
} EffectBlureInit1;

#define MM_ACTOR_KILL                              0x800B670C
#define MM_ACTOR_PROCESS_INIT_CHAIN                0x800FFADC
#define MM_EFFECT_ADD                              0x800AF960

#define MM_COLLIDER_INIT_QUAD                      0x800E1E18
#define MM_COLLIDER_DESTROY_QUAD                   0x800E1E68
#define MM_COLLIDER_SET_QUAD                       0x800E1F20

#define MM_COLLISION_SPAWN_SHIELD_PARTICLES_METAL  0x800E8668
#define MM_PLAYER_UPDATE_WEAPON_INFO               0x80126440

#define MM_ACTOR_MOVE_WITH_GRAVITY                 0x800B6A88

#define MM_ACTOR_PLAY_SFX_FLAGGED                  0x800B9010
#define MM_MATH_SCALED_STEP_TO_S                   0x800FEE70
#define MM_MATH_VEC3F_DIST_XYZ                     0x800FF884
#define MM_MATH_VEC3F_COPY                         0x800FF50C
#define MM_ACTOR_WORLD_YAW_TOWARD_POINT            0x800B6D2C
#define MM_ACTOR_WORLD_PITCH_TOWARD_POINT          0x800B6DA8
#define MM_BGCHECK_ENTITY_LINE_TEST1               0x800C55C4
#define MM_ACTOR_SET_FOCUS                         0x800B675C
#define MM_EFFECT_BLURE_ADD_VERTEX                 0x800A81F0
#define MM_EFFECT_GET_BY_INDEX                     0x800AF720
#define MM_GFX_SETUPDL25_OPA                       0x8012C28C

#define MM_MATRIX_MULTVEC3F                        0x80181A98
#define MM_MATRIX_FINALIZE                         0x80181A40
#define MM_MATRIX_ROTATE_XS                        0x80180478
#define MM_MATRIX_ROTATE_YS                        0x801809AC
#define MM_MATRIX_ROTATE_ZS                        0x80180CF8
#define MM_EFFECT_DESTROY                          0x800AFDCC
#define MM_MATH_COS_S                              0x800FED44
#define MM_MATH_SIN_S                              0x800FED84
#define MM_MATRIX_TRANSLATE                        0x8018029c
#define MM_MATRIX_SCALE                            0x8018039C
#define ACTOR_FLAG_HOOKSHOT_ATTACHED               (1 << 13)

#define PLAYER_FOCUS_ACTOR(player) \
(*(Actor**)((u8*)(player) + 0x730))

#define PLAYER_ZORA_BOOMERANG_ACTOR(player) \
(*(Actor**)((u8*)(player) + 0xA7C))

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

extern int prepareObject(PlayState* play, u16 objectId);

typedef void (*MmMatrixScaleFunc)(
    f32 x,
    f32 y,
    f32 z,
    s32 mode
);

typedef void (*MmMatrixRotateSFunc)(s16 angle, s32 mode);

static void CustomBoomerang_MatrixRotateXS(s16 angle, s32 mode)
{
    ((MmMatrixRotateSFunc)MM_MATRIX_ROTATE_XS)(angle, mode);
}

static void CustomBoomerang_MatrixRotateYS(s16 angle, s32 mode)
{
    ((MmMatrixRotateSFunc)MM_MATRIX_ROTATE_YS)(angle, mode);
}

static void CustomBoomerang_MatrixRotateZS(s16 angle, s32 mode)
{
    ((MmMatrixRotateSFunc)MM_MATRIX_ROTATE_ZS)(angle, mode);
}

typedef void (*MmEffectDestroyFunc)(
    PlayState* play,
    s32 effectIndex
);

typedef f32 (*MmMathTrigSFunc)(
    s16 angle
);

typedef void (*MmActorKillFunc)(Actor* actor);

typedef void (*MmActorProcessInitChainFunc)(Actor* actor, InitChainEntry* initChain);

typedef s32 (*MmEffectAddFunc)(
    PlayState* play,
    s32* effectIndex,
    s32 effectType,
    u8 arg3,
    u8 arg4,
    void* init
);

typedef void (*MmColliderInitQuadFunc)(
    PlayState* play,
    ColliderQuad* collider
);

typedef void (*MmColliderDestroyQuadFunc)(
    PlayState* play,
    ColliderQuad* collider
);

typedef void (*MmColliderSetQuadFunc)(
    PlayState* play,
    ColliderQuad* collider,
    Actor* actor,
    ColliderQuadInit* init
);

typedef void (*MmCollisionSpawnShieldParticlesMetalFunc)(
    PlayState* play,
    Vec3f* pos
);

typedef s32 (*MmPlayerUpdateWeaponInfoFunc)(
    PlayState* play,
    ColliderQuad* collider,
    WeaponInfo* weaponInfo,
    Vec3f* newTip,
    Vec3f* newBase
);

typedef void (*MmActorPlaySfxFlaggedFunc)(
    Actor* actor,
    s16 sfxId
);

typedef s32 (*MmMathScaledStepToSFunc)(
    s16* value,
    s16 target,
    s16 step
);

typedef f32 (*MmMathVec3fDistXYZFunc)(
    Vec3f* a,
    Vec3f* b
);

typedef void (*MmMathVec3fCopyFunc)(
    Vec3f* dest,
    Vec3f* src
);

typedef s16 (*MmActorWorldYawTowardPointFunc)(
    Actor* actor,
    Vec3f* point
);

typedef s16 (*MmActorWorldPitchTowardPointFunc)(
    Actor* actor,
    Vec3f* point
);

typedef s32 (*MmBgCheckEntityLineTest1Func)(
    CollisionContext* colCtx,
    Vec3f* posA,
    Vec3f* posB,
    Vec3f* hitPos,
    CollisionPoly** outPoly,
    s32 chkOneFace,
    s32 chkBg,
    s32 chkActors,
    s32 chkDyna,
    s32* bgId
);

typedef void (*MmActorSetFocusFunc)(
    Actor* actor,
    f32 height
);

typedef void (*MmEffectBlureAddVertexFunc)(
    void* blure,
    Vec3f* p1,
    Vec3f* p2
);

typedef void* (*MmEffectGetByIndexFunc)(
    s32 effectIndex
);

typedef void (*MmGfxSetupDL25OpaFunc)(
    GraphicsContext* gfxCtx
);

typedef void (*MmMatrixMultVec3fFunc)(
    Vec3f* src,
    Vec3f* dest
);

typedef Mtx* (*MmMatrixFinalizeFunc)(
    GraphicsContext* gfxCtx
);

typedef void (*MmActorMoveWithGravityFunc)(
    Actor* actor
);

typedef void (*MmMatrixTranslateFunc)(
    f32 x,
    f32 y,
    f32 z,
    s32 mode
);

static void CustomBoomerang_MatrixScale(f32 x, f32 y, f32 z, s32 mode)
{
    MmMatrixScaleFunc func =
        (MmMatrixScaleFunc)MM_MATRIX_SCALE;

    func(x, y, z, mode);
}

static void CustomBoomerang_MatrixTranslate(f32 x, f32 y, f32 z, s32 mode)
{
    MmMatrixTranslateFunc func =
        (MmMatrixTranslateFunc)MM_MATRIX_TRANSLATE;

    func(x, y, z, mode);
}

static void CustomBoomerang_EffectDestroy(PlayState* play, s32 effectIndex)
{
    MmEffectDestroyFunc func =
        (MmEffectDestroyFunc)MM_EFFECT_DESTROY;

    func(play, effectIndex);
}

static f32 CustomBoomerang_MathCosS(s16 angle)
{
    MmMathTrigSFunc func =
        (MmMathTrigSFunc)MM_MATH_COS_S;

    return func(angle);
}

static f32 CustomBoomerang_MathSinS(s16 angle)
{
    MmMathTrigSFunc func =
        (MmMathTrigSFunc)MM_MATH_SIN_S;

    return func(angle);
}

static void CustomBoomerang_ActorMoveWithGravity(Actor* actor)
{
    MmActorMoveWithGravityFunc func =
        (MmActorMoveWithGravityFunc)MM_ACTOR_MOVE_WITH_GRAVITY;

    func(actor);
}

static void CustomBoomerang_ActorKill(Actor* actor)
{
    MmActorKillFunc func = (MmActorKillFunc)MM_ACTOR_KILL;

    func(actor);
}

static void CustomBoomerang_ActorProcessInitChain(Actor* actor, InitChainEntry* initChain)
{
    MmActorProcessInitChainFunc func =
        (MmActorProcessInitChainFunc)MM_ACTOR_PROCESS_INIT_CHAIN;

    func(actor, initChain);
}

static s32 CustomBoomerang_EffectAdd(
    PlayState* play,
    s32* effectIndex,
    s32 effectType,
    u8 arg3,
    u8 arg4,
    void* init
)
{
    MmEffectAddFunc func = (MmEffectAddFunc)MM_EFFECT_ADD;

    return func(play, effectIndex, effectType, arg3, arg4, init);
}

static void CustomBoomerang_ColliderInitQuad(PlayState* play, ColliderQuad* collider)
{
    MmColliderInitQuadFunc func =
        (MmColliderInitQuadFunc)MM_COLLIDER_INIT_QUAD;

    func(play, collider);
}

static void CustomBoomerang_ColliderDestroyQuad(PlayState* play, ColliderQuad* collider)
{
    MmColliderDestroyQuadFunc func =
        (MmColliderDestroyQuadFunc)MM_COLLIDER_DESTROY_QUAD;

    func(play, collider);
}

static void CustomBoomerang_ColliderSetQuad(
    PlayState* play,
    ColliderQuad* collider,
    Actor* actor,
    ColliderQuadInit* init
)
{
    MmColliderSetQuadFunc func =
        (MmColliderSetQuadFunc)MM_COLLIDER_SET_QUAD;

    func(play, collider, actor, init);
}

static void CustomBoomerang_SpawnShieldParticlesMetal(PlayState* play, Vec3f* pos)
{
    MmCollisionSpawnShieldParticlesMetalFunc func =
        (MmCollisionSpawnShieldParticlesMetalFunc)MM_COLLISION_SPAWN_SHIELD_PARTICLES_METAL;

    func(play, pos);
}

static s32 CustomBoomerang_UpdateWeaponInfo(
    PlayState* play,
    ColliderQuad* collider,
    WeaponInfo* weaponInfo,
    Vec3f* newTip,
    Vec3f* newBase
)
{
    MmPlayerUpdateWeaponInfoFunc func =
        (MmPlayerUpdateWeaponInfoFunc)MM_PLAYER_UPDATE_WEAPON_INFO;

    return func(play, collider, weaponInfo, newTip, newBase);
}

static void CustomBoomerang_ActorPlaySfxFlagged(Actor* actor, s16 sfxId)
{
    MmActorPlaySfxFlaggedFunc func =
        (MmActorPlaySfxFlaggedFunc)MM_ACTOR_PLAY_SFX_FLAGGED;

    func(actor, sfxId);
}

static s32 CustomBoomerang_MathScaledStepToS(s16* value, s16 target, s16 step)
{
    MmMathScaledStepToSFunc func =
        (MmMathScaledStepToSFunc)MM_MATH_SCALED_STEP_TO_S;

    return func(value, target, step);
}

static f32 CustomBoomerang_MathVec3fDistXYZ(Vec3f* a, Vec3f* b)
{
    MmMathVec3fDistXYZFunc func =
        (MmMathVec3fDistXYZFunc)MM_MATH_VEC3F_DIST_XYZ;

    return func(a, b);
}

static void CustomBoomerang_MathVec3fCopy(Vec3f* dest, Vec3f* src)
{
    MmMathVec3fCopyFunc func =
        (MmMathVec3fCopyFunc)MM_MATH_VEC3F_COPY;

    func(dest, src);
}

static s16 CustomBoomerang_ActorWorldYawTowardPoint(Actor* actor, Vec3f* point)
{
    MmActorWorldYawTowardPointFunc func =
        (MmActorWorldYawTowardPointFunc)MM_ACTOR_WORLD_YAW_TOWARD_POINT;

    return func(actor, point);
}

static s16 CustomBoomerang_ActorWorldPitchTowardPoint(Actor* actor, Vec3f* point)
{
    MmActorWorldPitchTowardPointFunc func =
        (MmActorWorldPitchTowardPointFunc)MM_ACTOR_WORLD_PITCH_TOWARD_POINT;

    return func(actor, point);
}

static s32 CustomBoomerang_BgCheckEntityLineTest1(
    CollisionContext* colCtx,
    Vec3f* posA,
    Vec3f* posB,
    Vec3f* hitPos,
    CollisionPoly** outPoly,
    s32 chkOneFace,
    s32 chkBg,
    s32 chkActors,
    s32 chkDyna,
    s32* bgId
)
{
    MmBgCheckEntityLineTest1Func func =
        (MmBgCheckEntityLineTest1Func)MM_BGCHECK_ENTITY_LINE_TEST1;

    return func(colCtx, posA, posB, hitPos, outPoly, chkOneFace, chkBg, chkActors, chkDyna, bgId);
}

static void CustomBoomerang_ActorSetFocus(Actor* actor, f32 height)
{
    MmActorSetFocusFunc func =
        (MmActorSetFocusFunc)MM_ACTOR_SET_FOCUS;

    func(actor, height);
}

static void CustomBoomerang_EffectBlureAddVertex(void* blure, Vec3f* p1, Vec3f* p2)
{
    MmEffectBlureAddVertexFunc func =
        (MmEffectBlureAddVertexFunc)MM_EFFECT_BLURE_ADD_VERTEX;

    func(blure, p1, p2);
}

static void* CustomBoomerang_EffectGetByIndex(s32 effectIndex)
{
    MmEffectGetByIndexFunc func =
        (MmEffectGetByIndexFunc)MM_EFFECT_GET_BY_INDEX;

    return func(effectIndex);
}

static void CustomBoomerang_GfxSetupDL25Opa(GraphicsContext* gfxCtx)
{
    MmGfxSetupDL25OpaFunc func =
        (MmGfxSetupDL25OpaFunc)MM_GFX_SETUPDL25_OPA;

    func(gfxCtx);
}

static void CustomBoomerang_MatrixMultVec3f(Vec3f* src, Vec3f* dest)
{
    MmMatrixMultVec3fFunc func =
        (MmMatrixMultVec3fFunc)MM_MATRIX_MULTVEC3F;

    func(src, dest);
}

static Mtx* CustomBoomerang_MatrixFinalize(GraphicsContext* gfxCtx)
{
    MmMatrixFinalizeFunc func =
        (MmMatrixFinalizeFunc)MM_MATRIX_FINALIZE;

    return func(gfxCtx);
}

void CustomBoomerang_Init(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Destroy(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Update(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Draw(Actor_CustomBoomerang* boom, PlayState* play);
void CustomBoomerang_Fly(Actor_CustomBoomerang* boom, PlayState* play);

static ColliderQuadInit sQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000010, 0x00, 0x01 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_NEAREST | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    {
        {
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f },
        },
    },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(attentionRangeType, ATTENTION_RANGE_5, ICHAIN_CONTINUE),
    ICHAIN_VEC3S(shape.rot, 0, ICHAIN_STOP),
};

static void CustomBoomerang_SetProjectileSpeed(Actor* actor, f32 speed)
{
    actor->speed = CustomBoomerang_MathCosS(actor->world.rot.x) * speed;
    actor->velocity.y = -CustomBoomerang_MathSinS(actor->world.rot.x) * speed;
}

void CustomBoomerang_SetupAction(Actor_CustomBoomerang* boom, CustomBoomerangActionFunc actionFunc)
{
    boom->actionFunc = actionFunc;
}

void CustomBoomerang_Init(Actor_CustomBoomerang* boom, PlayState* play)
{
    static u8 sP1StartColor[4] = { 255, 255, 100, 255 };
    static u8 sP2StartColor[4] = { 255, 255, 100, 64 };
    static u8 sP1EndColor[4]   = { 255, 255, 100, 0 };
    static u8 sP2EndColor[4]   = { 255, 255, 100, 0 };

    EffectBlureInit1 blureInit;
    s32 i;

    CustomBoomerang_ActorProcessInitChain(&boom->actor, sInitChain);

    bzero(&blureInit, sizeof(blureInit));

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = sP1StartColor[i];
        blureInit.p2StartColor[i] = sP2StartColor[i];
        blureInit.p1EndColor[i] = sP1EndColor[i];
        blureInit.p2EndColor[i] = sP2EndColor[i];
    }

    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 0;

    boom->effectIndex = -1;
    CustomBoomerang_EffectAdd(play, &boom->effectIndex, EFFECT_BLURE1, 0, 0, &blureInit);

    CustomBoomerang_ColliderInitQuad(play, &boom->collider);
    CustomBoomerang_ColliderSetQuad(play, &boom->collider, &boom->actor, &sQuadInit);

    CustomBoomerang_SetupAction(boom, CustomBoomerang_Fly);
}

void CustomBoomerang_Destroy(Actor_CustomBoomerang* boom, PlayState* play)
{
    if (boom->effectIndex >= 0) {
        CustomBoomerang_EffectDestroy(play, boom->effectIndex);
    }

    CustomBoomerang_ColliderDestroyQuad(play, &boom->collider);
}

/* ============================================================================
 * Main flight behavior
 * ========================================================================== */

void CustomBoomerang_Fly(Actor_CustomBoomerang* boom, PlayState* play)
{
    Actor* target;
    Player* player;
    s32 collided;
    s16 yawTarget;
    s16 yawDiff;
    s16 pitchTarget;
    s16 pitchDiff;
    float distXYZScale;
    float distFromLink;
    s32 hitDynaId;
    Vec3f hitPoint;

    player = GET_PLAYER(play);
    target = boom->moveTo;

    if (target != NULL) {
        yawTarget = CustomBoomerang_ActorWorldYawTowardPoint(&boom->actor, &target->focus.pos);
        yawDiff = boom->actor.world.rot.y - yawTarget;

        pitchTarget = CustomBoomerang_ActorWorldPitchTowardPoint(&boom->actor, &target->focus.pos);
        pitchDiff = boom->actor.world.rot.x - pitchTarget;

        distXYZScale =
            (200.0f - CustomBoomerang_MathVec3fDistXYZ(&boom->actor.world.pos, &target->focus.pos)) * 0.005f;

        if (distXYZScale < 0.12f) {
            distXYZScale = 0.12f;
        }

        if ((target != &player->actor) && ((target->update == NULL) || (ABS(yawDiff) > 0x4000))) {
            boom->moveTo = NULL;
        } else {
            CustomBoomerang_MathScaledStepToS(
                &boom->actor.world.rot.y,
                yawTarget,
                (s16)(ABS(yawDiff) * distXYZScale)
            );

            CustomBoomerang_MathScaledStepToS(
                &boom->actor.world.rot.x,
                pitchTarget,
                (s16)(ABS(pitchDiff) * distXYZScale)
            );
        }
    }

    CustomBoomerang_SetProjectileSpeed(&boom->actor, 12.0f);
    CustomBoomerang_ActorMoveWithGravity(&boom->actor);

    CustomBoomerang_ActorPlaySfxFlagged(&boom->actor, NA_SE_IT_BOOMERANG_FLY - SFX_FLAG);

    collided = !!(boom->collider.base.atFlags & AT_HIT);

    if (collided) {
        if ((boom->collider.base.at->id == ACTOR_EN_ITEM00) ||
            (boom->collider.base.at->id == ACTOR_EN_SI)) {
            boom->grabbed = boom->collider.base.at;

            if (boom->collider.base.at->id == ACTOR_EN_SI) {
                boom->collider.base.at->flags |= ACTOR_FLAG_HOOKSHOT_ATTACHED;
            }
        }
    }

    if (DECR(boom->returnTimer) == 0) {
        distFromLink = CustomBoomerang_MathVec3fDistXYZ(&boom->actor.world.pos, &player->actor.focus.pos);
        boom->moveTo = &player->actor;

        if (distFromLink < 40.0f) {
            target = boom->grabbed;

            if (target != NULL) {
                CustomBoomerang_MathVec3fCopy(&target->world.pos, &player->actor.world.pos);

                if (target->id == ACTOR_EN_ITEM00) {
                    target->gravity = -0.9f;
                    target->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
                } else {
                    target->flags &= ~ACTOR_FLAG_HOOKSHOT_ATTACHED;
                }
            }

            player->stateFlags1 &= ~PLAYER_STATE1_MM_2000000;
            PLAYER_ZORA_BOOMERANG_ACTOR(player) = NULL;
            CustomBoomerang_ActorKill(&boom->actor);
        }
    } else {
        collided = !!(boom->collider.base.atFlags & AT_HIT);

        if (collided) {
            CustomBoomerang_MathVec3fCopy(&boom->actor.world.pos, &boom->actor.prevPos);
        } else {
            collided = CustomBoomerang_BgCheckEntityLineTest1(
                &play->colCtx,
                &boom->actor.prevPos,
                &boom->actor.world.pos,
                &hitPoint,
                &boom->actor.wallPoly,
                true,
                true,
                true,
                true,
                &hitDynaId
            );

            if (collided) {
                CustomBoomerang_SpawnShieldParticlesMetal(play, &hitPoint);
            }
        }

        if (collided) {
            boom->actor.world.rot.x = -boom->actor.world.rot.x;
            boom->actor.world.rot.y += 0x8000;
            boom->moveTo = &player->actor;
            boom->returnTimer = 0;
        }
    }

    target = boom->grabbed;

    if (target != NULL) {
        if (target->update == NULL) {
            boom->grabbed = NULL;
        } else {
            CustomBoomerang_MathVec3fCopy(&target->world.pos, &boom->actor.world.pos);
        }
    }
}

void CustomBoomerang_Update(Actor_CustomBoomerang* boom, PlayState* play)
{
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & PLAYER_STATE1_MM_20000000)) {
        boom->actionFunc(boom, play);
        CustomBoomerang_ActorSetFocus(&boom->actor, 0.0f);
        boom->activeTimer++;
    }
}

#define BOOMERANG_MODEL_SCALE 0.0125f
#define BOOMERANG_BLURE_HALF_LEN (960.0f * BOOMERANG_MODEL_SCALE)

void CustomBoomerang_Draw(Actor_CustomBoomerang* boom, PlayState* play)
{
    static Vec3f sPosAOffset = { -BOOMERANG_BLURE_HALF_LEN, 0.0f, 0.0f };
    static Vec3f sPosBOffset = {  BOOMERANG_BLURE_HALF_LEN, 0.0f, 0.0f };

    Vec3f posA;
    Vec3f posB;
    void* blure;

    if (!prepareObject(play, CUSTOM_OBJECT_ID_EQ_BOOMERANG)) {
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    CustomBoomerang_MatrixTranslate(
        boom->actor.world.pos.x,
        boom->actor.world.pos.y,
        boom->actor.world.pos.z,
        MTXMODE_NEW
    );
    CustomBoomerang_MatrixRotateYS(boom->actor.world.rot.y, MTXMODE_APPLY);
    CustomBoomerang_MatrixRotateZS(0x1F40, MTXMODE_APPLY);
    CustomBoomerang_MatrixRotateXS(boom->actor.world.rot.x, MTXMODE_APPLY);
    CustomBoomerang_MatrixMultVec3f(&sPosAOffset, &posA);
    CustomBoomerang_MatrixMultVec3f(&sPosBOffset, &posB);

    if (CustomBoomerang_UpdateWeaponInfo(play, &boom->collider, &boom->weaponInfo, &posA, &posB)) {
        blure = CustomBoomerang_EffectGetByIndex(boom->effectIndex);

        if (blure != NULL) {
            CustomBoomerang_EffectBlureAddVertex(blure, &posA, &posB);
        }
    }

    CustomBoomerang_GfxSetupDL25Opa(play->state.gfxCtx);
    CustomBoomerang_MatrixRotateYS(
        boom->activeTimer * -0x2EE0,
        MTXMODE_APPLY
    );
    CustomBoomerang_MatrixRotateXS(
        -0x4000,
        MTXMODE_APPLY
    );
    CustomBoomerang_MatrixTranslate(
        -5.0f,
        -5.0f,
        0.0f,
        MTXMODE_APPLY
    );
    CustomBoomerang_MatrixScale(
        BOOMERANG_MODEL_SCALE,
        BOOMERANG_MODEL_SCALE,
        BOOMERANG_MODEL_SCALE,
        MTXMODE_APPLY
    );

    gSPMatrix(
        POLY_OPA_DISP++,
        CustomBoomerang_MatrixFinalize(play->state.gfxCtx),
        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
    );

    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_EQ_BOOMERANG_0);

    CLOSE_DISPS();
}

ActorProfile Custom_Boomerang_InitVars = {
    ACTOR_CUSTOM_BOOMERANG,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Actor_CustomBoomerang),
    (ActorFunc)CustomBoomerang_Init,
    (ActorFunc)CustomBoomerang_Destroy,
    (ActorFunc)CustomBoomerang_Update,
    (ActorFunc)CustomBoomerang_Draw,
};

OVL_INFO_ACTOR(ACTOR_CUSTOM_BOOMERANG, Custom_Boomerang_InitVars);