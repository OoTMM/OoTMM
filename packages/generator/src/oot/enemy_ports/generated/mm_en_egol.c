#include <combo/enemy_keep.h>
#include "mm_native_compat.h"
#include <combo/enemy_eyegore_native.h>
#define gEyegoreEffectImpactDL (*((Gfx (*)[1])0x06000040))
#define gEyegoreEffectLargeBodyPieceDL (*((Gfx (*)[1])0x06001170))
#define gEyegoreEffectSmallBodyPieceDL (*((Gfx (*)[1])0x060013b0))
#define gEyegoreEffectSolidDebrisDL (*((Gfx (*)[1])0x06001918))
#define gEyegoreSlamAnim (*(AnimationHeader*)0x06002460)
#define gEyegoreLaserDL (*((Gfx (*)[1])0x060041f0))
#define gEyegoreEyeLaserTexAnim (*((u8 (*)[1])0x060094e4))
#define gEyegoreSkel (*(FlexSkeletonHeader*)0x06009664)
#define gEyegoreSlamWaitAnim (*(AnimationHeader*)0x06009d2c)
#define gEyegoreSlamEndAnim (*(AnimationHeader*)0x0600a204)
#define gEyegoreLaserAnim (*(AnimationHeader*)0x0600a484)
#define gEyegoreUnusedLaserEndAnim (*(AnimationHeader*)0x0600a73c)
#define gEyegoreDamagedAnim (*(AnimationHeader*)0x0600adf8)
#define gEyegoreDeathAnim (*(AnimationHeader*)0x0600bc60)
#define gEyegoreRetreatAnim (*(AnimationHeader*)0x0600beb8)
#define gEyegoreLeftPunchAnim (*(AnimationHeader*)0x0600c434)
#define gEyegoreRightPunchAnim (*(AnimationHeader*)0x0600c9b4)
#define gEyegoreStunnedAnim (*(AnimationHeader*)0x0600cc4c)
#define gEyegoreStunEndAnim (*(AnimationHeader*)0x0600ce54)
#define gEyegoreSitAnim (*(AnimationHeader*)0x0600d3d8)
#define gEyegoreStandAnim (*(AnimationHeader*)0x0600ee4c)
#define gEyegoreWalkAnim (*(AnimationHeader*)0x060100c0)

#include "types/en_estone.h"
#include "types/en_egol.h"
#define TATL_HINT_ID_EYEGORE 0xff
#undef ACTOR_EN_ARROW
#define ACTOR_EN_ARROW 0xf
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_EGOL
#define ACTOR_EN_EGOL 0x184
#undef ACTOR_EN_ESTONE
#define ACTOR_EN_ESTONE 0x18e
#undef OBJECT_EG
#define OBJECT_EG 0x118d
#undef NA_SE_IT_SHIELD_BOUND
#define NA_SE_IT_SHIELD_BOUND 0x1806
#undef NA_SE_IT_BOMB_EXPLOSION
#define NA_SE_IT_BOMB_EXPLOSION 0x180e
#undef NA_SE_EV_EXPLOSION
#define NA_SE_EV_EXPLOSION 0x2802
#undef NA_SE_EN_EYEGOLE_DEAD
#define NA_SE_EN_EYEGOLE_DEAD 0x3846
#undef NA_SE_EN_EYEGOLE_DAMAGE
#define NA_SE_EN_EYEGOLE_DAMAGE 0x3847
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_EYEGOLE_BEAM
#define NA_SE_EN_EYEGOLE_BEAM 0x391b
#undef NA_SE_EN_EYEGOLE_WALK
#define NA_SE_EN_EYEGOLE_WALK 0x3a06
#undef NA_SE_EN_EYEGOLE_STAND
#define NA_SE_EN_EYEGOLE_STAND 0x3a07
#undef NA_SE_EN_EYEGOLE_SIT
#define NA_SE_EN_EYEGOLE_SIT 0x3a08
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Flags_GetSwitch
#define Flags_GetSwitch ((s32 (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Collider_DestroyQuad
#define Collider_DestroyQuad ((s32 (*)(struct PlayState* play, ColliderQuad* quad))(void*)FOREIGN_ENEMY_API_QUAD_DESTROY)
#undef func_800B8D50
#define func_800B8D50 ((void (*)(struct PlayState* play, Actor* actor, f32 arg2, s16 yaw, f32 arg4, u32 arg5))(void*)FOREIGN_ENEMY_API_KNOCKBACK_LARGE)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CutsceneManager_IsNext
#define CutsceneManager_IsNext ((s16 (*)(s16 csId))(void*)FOREIGN_ENEMY_API_CUTSCENE_IS_NEXT_NONE)
#undef CutsceneManager_Queue
#define CutsceneManager_Queue ((void (*)(s16 csId))(void*)FOREIGN_ENEMY_API_CUTSCENE_QUEUE_NONE)
#undef CutsceneManager_StartWithPlayerCs
#define CutsceneManager_StartWithPlayerCs ((s16 (*)(s16 csId, struct Actor* actor))(void*)FOREIGN_ENEMY_API_CUTSCENE_START_PLAYER_NONE)
#undef Flags_SetSwitch
#define Flags_SetSwitch ((void (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef CutsceneManager_Stop
#define CutsceneManager_Stop ((s16 (*)(s16 csId))(void*)FOREIGN_ENEMY_API_CUTSCENE_STOP_NONE)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef AnimatedMat_Draw
#define AnimatedMat_Draw ((void (*)(struct PlayState* play, AnimatedMaterial* matAnim))(void*)FOREIGN_ENEMY_API_MATERIAL_DRAW)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZYX)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef Actor_RequestQuakeAndRumble
#define Actor_RequestQuakeAndRumble ((void (*)(Actor* actor, struct PlayState* play, s16 quakeY, s16 quakeDuration))0x80026cf4u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(struct PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 countMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800260e8u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId))0x800308b4u)
#undef Math_FAtan2F
#define Math_FAtan2F ((f32 (*)(f32 y, f32 x))0x800cd76cu)
#undef func_800B31BC
#define func_800B31BC ((void (*)(struct PlayState* play, Vec3f* pos, s16 scale, s16 scaleStep, s16 alpha, s32 life))0x8001e428u)
#undef func_800BBFB0
#define func_800BBFB0 ((void (*)(struct PlayState* play, Vec3f* position, f32 arg2, s32 arg3, s16 arg4, s16 scaleStep, u8 arg6))0x80026308u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef CollisionCheck_BlueBlood
#define CollisionCheck_BlueBlood ((void (*)(struct PlayState* play, Collider* collider, Vec3f* v))0x8004c430u)
#undef EffectSsHitmark_SpawnFixedScale
#define EffectSsHitmark_SpawnFixedScale ((void (*)(struct PlayState* play, s32 type, Vec3f* pos))0x8001d7a8u)
#undef CollisionCheck_SpawnShieldParticlesMetal
#define CollisionCheck_SpawnShieldParticlesMetal ((void (*)(struct PlayState* play, Vec3f* v))0x800511b8u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f34u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x800aa6ecu)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x800aa724u)
#undef Collider_SetQuadVertices
#define Collider_SetQuadVertices ((void (*)(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d))0x80050b64u)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef Play_SetFog
#define Play_SetFog ((Gfx* (*)(PlayState* this, Gfx* gfx))0x8009a574u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x800abe54u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
static Actor* enemyExtraProjectileQuery(PlayState* play, Actor* owner, f32 radius, u32 sourceMask) {
    Actor* projectile = play->actorCtx.actors[7].first;
    while (projectile != NULL) {
        u32 kind = 0;
        if (projectile->id == 0x66)
            kind = 0x80;
        else if (projectile->id == 0x32)
            kind = 0x10;
        else if (projectile->id == 0x16) {
            s16 params = projectile->params;
            if (params == -1 || params == 1 || params == 2)
                kind = 0x20;
            else if (params == 0 || params == 3)
                kind = 0x800;
            else if (params == 4)
                kind = 0x1000;
            else if (params == 5)
                kind = 0x2000;
            else if (params == 9)
                kind = 0x10000;
            else if (params == 10 || params == -10)
                kind = 1;
        }
        if ((kind & sourceMask) && projectile != owner && projectile->update != NULL && projectile->speed > 0.0f) {
            f32 x = projectile->world.pos.x - owner->world.pos.x;
            f32 y = projectile->world.pos.y - owner->world.pos.y;
            f32 z = projectile->world.pos.z - owner->world.pos.z;
            if (owner->colChkInfo.cylRadius <= 0 || owner->colChkInfo.cylHeight <= 0) {
                f32 reach = radius + projectile->speed * 2.5f;
                s16 yaw = Math_Vec3f_Yaw(&projectile->world.pos, &owner->world.pos) - projectile->world.rot.y;
                if (x * x + y * y + z * z < reach * reach && ABS_ALT(yaw) < 0x1400)
                    return projectile;
                projectile = projectile->next;
                continue;
            }
            f32 reach = radius + projectile->speed * 10.0f;
            if (x * x + y * y + z * z <= reach * reach) {
                Vec3f target, hit1, hit2;
                target.x = projectile->world.pos.x + projectile->speed * 10.0f * Math_SinS(projectile->world.rot.y);
                target.y = projectile->world.pos.y + projectile->velocity.y + projectile->gravity * 10.0f;
                target.z = projectile->world.pos.z + projectile->speed * 10.0f * Math_CosS(projectile->world.rot.y);
                if (((s32(*)(f32, f32, f32, Vec3f*, Vec3f*, Vec3f*, Vec3f*, Vec3f*))0x80051330)(
                        owner->colChkInfo.cylRadius, owner->colChkInfo.cylHeight, 0.0f, &owner->world.pos,
                        &projectile->world.pos, &target, &hit1, &hit2))
                    return projectile;
            }
        }
        projectile = projectile->next;
    }
    return NULL;
}

#define ACTOR_FLAG_MINIMAP_ICON_ENABLED 0
#define CLEAR_TAG_SMALL_EXPLOSION 0
#define EFFECT_HITMARK_METAL 3
#define ARROW_TYPE_LIGHT 5
#define RAD_TO_BINANG_ALT2(radians) TRUNCF_BINANG(((radians) * 0x8000) / M_PIf)
static Vec3s enemyExtraZeroVec3s;
static void enemyExtraQuadInit(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* init) {
    ((s32(*)(PlayState*, ColliderQuad*))0x8004b858)(play, dest);
    ((s32(*)(PlayState*, ColliderQuad*, Actor*, ColliderQuadInit*))0x8004b960)(play, dest, actor, init);
}

static Gfx* enemyExtraTexScroll(GraphicsContext* gfx, u32 x, u32 y) {
    return ((Gfx * (*)(GraphicsContext*, u32, u32, s32, s32))0x8007eb00)(gfx, x, y, 0, 0);
}

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED | ACTOR_FLAG_MINIMAP_ICON_ENABLED)

typedef enum {
    EYEGORE_ACTION_WAIT,
    EYEGORE_ACTION_STAND,
    EYEGORE_ACTION_WALK,
    EYEGORE_ACTION_RETREAT,
    EYEGORE_ACTION_LASER,
    EYEGORE_ACTION_SLAM,
    EYEGORE_ACTION_PUNCH,
    EYEGORE_ACTION_SLAM_WAIT,
    EYEGORE_ACTION_STUNNED,
    EYEGORE_ACTION_STUN_END,
    EYEGORE_ACTION_SLAM_END,
    EYEGORE_ACTION_DAMAGED,
    EYEGORE_ACTION_STOP,
    EYEGORE_ACTION_DYING,
    EYEGORE_ACTION_DEAD
} EnEgolAction;

typedef enum {
    EYEGORE_EFFECT_IMPACT,
    EYEGORE_EFFECT_PIECE_LARGE,
    EYEGORE_EFFECT_PIECE_SMALL,
    EYEGORE_EFFECT_DEBRIS
} EnEgolEffectType;

typedef enum {
    EYEGORE_LASER_OFF,
    EYEGORE_LASER_START,
    EYEGORE_LASER_CHARGING,
    EYEGORE_LASER_FIRE,
    EYEGORE_LASER_ON = 7
} EnEgolLaserState;

void EnEgol_Init(Actor* thisx, PlayState* play);
void EnEgol_Destroy(Actor* thisx, PlayState* play);
void EnEgol_Update(Actor* thisx, PlayState* play);
void EnEgol_Draw(Actor* thisx, PlayState* play2);

void EnEgol_SetupWait(EnEgol* this);
void EnEgol_Wait(EnEgol* this, PlayState* play);
void EnEgol_SetupStand(EnEgol* this);
void EnEgol_Stand(EnEgol* this, PlayState* play);
void EnEgol_SetupWalk(EnEgol* this);
void EnEgol_Walk(EnEgol* this, PlayState* play);
void EnEgol_SetupRetreat(EnEgol* this);
void EnEgol_Retreat(EnEgol* this, PlayState* play);
void EnEgol_SetupLaser(EnEgol* this);
void EnEgol_Laser(EnEgol* this, PlayState* play);
void EnEgol_SetupStop(EnEgol* this);
void EnEgol_Stop(EnEgol* this, PlayState* play);
void EnEgol_SetupSlam(EnEgol* this);
void EnEgol_Slam(EnEgol* this, PlayState* play);
void EnEgol_SetupPunch(EnEgol* this);
void EnEgol_Punch(EnEgol* this, PlayState* play);
void EnEgol_SetupSlamWait(EnEgol* this);
void EnEgol_SlamWait(EnEgol* this, PlayState* play);
void EnEgol_SetupStunned(EnEgol* this);
void EnEgol_Stunned(EnEgol* this, PlayState* play);
void EnEgol_SetupStunEnd(EnEgol* this);
void EnEgol_StunEnd(EnEgol* this, PlayState* play);
void EnEgol_SetupSlamEnd(EnEgol* this);
void EnEgol_SlamEnd(EnEgol* this, PlayState* play);
void EnEgol_SetupDamaged(EnEgol* this);
void EnEgol_Damaged(EnEgol* this, PlayState* play);
void EnEgol_StartDeathCutscene(EnEgol* this, PlayState* play);
void EnEgol_Death(EnEgol* this, PlayState* play);

void EnEgol_SpawnEffect(EnEgol* this, Vec3f* pos, Vec3s* rot, s16 lifetime, f32 scale, s16 type);
void EnEgol_UpdateEffects(EnEgol* this, PlayState* play);
void EnEgol_DrawEffects(EnEgol* this, PlayState* play);

static ColliderJntSphElementInit sEyeJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { EYEGORE_LIMB_EYE_COLLIDER, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphElementInit sBodySphElementsInit[6] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_LEFT_HAND, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_RIGHT_HAND, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_HEAD, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { EYEGORE_LIMB_LEFT_SHOULDER, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { EYEGORE_LIMB_RIGHT_SHOULDER, { { 0, 0, 0 }, 0 }, 1 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { EYEGORE_LIMB_FAULDS, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sBodyJntSphInit = {
    {
        COL_MATERIAL_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sBodySphElementsInit),
    sBodySphElementsInit,
};

static ColliderJntSphInit sEyeJntSphInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sEyeJntSphElementsInit),
    sEyeJntSphElementsInit,
};

static ColliderQuadInit sLaserQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x07, 0x10 },
        { 0x00000000, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    EYEGORE_DMGEFF_IMMUNE_0,
    EYEGORE_DMGEFF_LIGHT_ARROW = 4,
    EYEGORE_DMGEFF_NONE = 0xE,
    EYEGORE_DMGEFF_IMMUNE_F
} EnEgolDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F), DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(2, EYEGORE_DMGEFF_LIGHT_ARROW),
    DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(2, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F),
    DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),     DMG_ENTRY(1, EYEGORE_DMGEFF_NONE),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_F), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0),
    DMG_ENTRY(0, EYEGORE_DMGEFF_IMMUNE_0), DMG_ENTRY(1, EYEGORE_DMGEFF_IMMUNE_F),
} };

static Color_RGB8 sEyePrimColors[] = {
    { 175, 255, 255 },
    { 195, 245, 10 },
    { 255, 55, 205 },
};

static Color_RGB8 sEyeEnvColors[] = {
    { 255, 115, 155 },
    { 40, 155, 155 },
    { 215, 255, 255 },
};

static Color_RGB8 sLightOrbColors[] = {
    { 255, 255, 255 },
    { 255, 255, 255 },
};

ActorProfile En_Egol_Profile = {
    0xd84, ACTORCAT_ENEMY, FLAGS, 0x118d, sizeof(EnEgol), EnEgol_Init, EnEgol_Destroy, EnEgol_Update, EnEgol_Draw,
};

typedef enum {
    EYEGORE_ANIM_STAND,
    EYEGORE_ANIM_WALK,
    EYEGORE_ANIM_SLAM,
    EYEGORE_ANIM_SLAM_WAIT,
    EYEGORE_ANIM_SLAM_END,
    EYEGORE_ANIM_DAMAGED,
    EYEGORE_ANIM_DEATH,
    EYEGORE_ANIM_LASER,
    EYEGORE_ANIM_LASER_END,
    EYEGORE_ANIM_STUNNED,
    EYEGORE_ANIM_STUN_END,
    EYEGORE_ANIM_RETREAT,
    EYEGORE_ANIM_SIT,
    EYEGORE_ANIM_LEFT_PUNCH,
    EYEGORE_ANIM_RIGHT_PUNCH,
    EYEGORE_ANIM_MAX
} EnEgolAnimation;

static AnimationHeader* sAnimations[EYEGORE_ANIM_MAX] = {
    &gEyegoreStandAnim,    &gEyegoreWalkAnim,      &gEyegoreSlamAnim,
    &gEyegoreSlamWaitAnim, &gEyegoreSlamEndAnim,   &gEyegoreDamagedAnim,
    &gEyegoreDeathAnim,    &gEyegoreLaserAnim,     &gEyegoreUnusedLaserEndAnim,
    &gEyegoreStunnedAnim,  &gEyegoreStunEndAnim,   &gEyegoreRetreatAnim,
    &gEyegoreSitAnim,      &gEyegoreLeftPunchAnim, &gEyegoreRightPunchAnim,
};

static u8 sAnimationModes[EYEGORE_ANIM_MAX] = {
    ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE,
    ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE,
    ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE,
};

void EnEgol_ChangeAnim(EnEgol* this, s32 animIndex) {
    this->animIndex = animIndex;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[this->animIndex]);
    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], 1.0f, 0.0f, this->animEndFrame,
                     sAnimationModes[this->animIndex], 0.0f);
}

void EnEgol_FootstepEffects(EnEgol* this, PlayState* play, f32 leftFootFrame, f32 rightFootFrame) {
    if (Animation_OnFrame(&this->skelAnime, leftFootFrame) || Animation_OnFrame(&this->skelAnime, rightFootFrame)) {
        Vec3f spawnPos;
        Player* player = GET_PLAYER(play);
        s32 quakeYOffset;

        Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_WALK);
        quakeYOffset = 4 - (s32)(fabsf(player->actor.world.pos.y - this->actor.world.pos.y) * 0.02f);
        if (quakeYOffset > 4) {
            quakeYOffset = 4;
        } else if (quakeYOffset < 1) {
            quakeYOffset = 1;
        }

        if (true) {
            Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
        }
        if (Animation_OnFrame(&this->skelAnime, leftFootFrame)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight;
            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 0.0f, 10, 6.0f, 50, 30, true);
        } else {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight;
            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 0.0f, 10, 6.0f, 50, 30, true);
        }
    }
}

void EnEgol_DestroyBlocks(EnEgol* this, PlayState* play, Vec3f pos1, Vec3f pos2) {}

void EnEgol_GetWaypoint(EnEgol* this) {
    s16 yaw = this->actor.yawTowardsPlayer + (this->isRetreating ? 0x8000 : 0);
    this->waypoint = 0;
    this->waypointPos = this->actor.world.pos;
    this->waypointPos.x += Math_SinS(yaw) * 100.0f;
    this->waypointPos.z += Math_CosS(yaw) * 100.0f;
}

#define EYEGORE_SET_SPH_DIM(element, centerX, centerY, centerZ, radiusV, scaleV) \
    element.dim.modelSphere.radius = radiusV;                                    \
    element.dim.scale = scaleV;                                                  \
    element.dim.modelSphere.center.x = centerX;                                  \
    element.dim.modelSphere.center.y = centerY;                                  \
    element.dim.modelSphere.center.z = centerZ

void EnEgol_Init(Actor* thisx, PlayState* play) {
    EnEgol* this = (EnEgol*)thisx;

    this->actor.gravity = -2.0f;
    Actor_SetScale(&this->actor, 0.015f);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 60.0f);

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.hintId = TATL_HINT_ID_EYEGORE;
    this->actor.colChkInfo.health = 8;

    SkelAnime_InitFlex(play, &this->skelAnime, &gEyegoreSkel, &gEyegoreStandAnim, this->jointTable, this->morphTable,
                       EYEGORE_LIMB_MAX);

    Collider_InitAndSetJntSph(play, &this->bodyCollider, &this->actor, &sBodyJntSphInit, this->bodyElements);
    Collider_InitAndSetJntSph(play, &this->eyeCollider, &this->actor, &sEyeJntSphInit, this->eyeElements);
    enemyExtraQuadInit(play, &this->laserCollider, &this->actor, &sLaserQuadInit);

    if (thisx->params != 0x1fc0) {
        Actor_Kill(thisx);
        return;
    }
    this->pathIndex = 0;
    this->path = NULL;
    this->wakeupRange = 300.0f;
    this->minLaserRange = 150.0f;
    this->actor.world.rot.x = 0;
    this->actor.world.rot.z = 0;

    EYEGORE_SET_SPH_DIM(this->eyeCollider.elements[0], 500, 0, 0, 26, 1.0f);

    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[0], 1800, 100, 0, 20, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[1], 1800, 100, 0, 20, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[2], 1000, -500, 0, 35, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[3], 200, 200, 0, 25, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[4], 300, 200, 0, 25, 1.0f);
    EYEGORE_SET_SPH_DIM(this->bodyCollider.elements[5], 2100, -300, 0, 37, 1.0f);

    this->switchFlag = -1;
    EnEgol_SetupStand(this);
}

void EnEgol_Destroy(Actor* thisx, PlayState* play) {
    EnEgol* this = (EnEgol*)thisx;

    Collider_DestroyJntSph(play, &this->bodyCollider);
    Collider_DestroyJntSph(play, &this->eyeCollider);
    Collider_DestroyQuad(play, &this->laserCollider);
}

void EnEgol_SetupWait(EnEgol* this) {
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->action = EYEGORE_ACTION_WAIT;
    this->actionFunc = EnEgol_Wait;
}

void EnEgol_Wait(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s16 angleToFacing = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;

    if ((curFrame >= this->animEndFrame) && (ABS_ALT(angleToFacing) < 0x3000) &&
        (this->actor.xzDistToPlayer < this->wakeupRange)) {
        EnEgol_SetupStand(this);
    }
}

void EnEgol_SetupStand(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STAND);
    this->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
    this->action = EYEGORE_ACTION_STAND;
    Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_STAND);
    this->actionFunc = EnEgol_Stand;
}

void EnEgol_Stand(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupWalk(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_WALK);
    this->laserCount = 0;
    EnEgol_GetWaypoint(this);
    this->action = EYEGORE_ACTION_WALK;
    this->actionFunc = EnEgol_Walk;
}

void EnEgol_Walk(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    f32 curFrame = this->skelAnime.curFrame;
    Player* player = GET_PLAYER(play);
    Vec3f spawnPos;

    Math_SmoothStepToS(&this->headRot, 0, 1, 0x3E8, 0);
    Math_Vec3f_Copy(&spawnPos, &gZeroVec3f);
    if (curFrame >= this->animEndFrame) {
        this->laserCount++;
        if (this->laserCount > 0) {
            this->laserCount = 0;
            if (this->actor.xzDistToPlayer > this->minLaserRange) {
                EnEgol_SetupLaser(this);
                return;
            }
        }
    }
    if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
        if (Animation_OnFrame(&this->skelAnime, 24.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &enemyExtraZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
        if (Animation_OnFrame(&this->skelAnime, 9.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &enemyExtraZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
    }
    if ((ABS_ALT(angleToFacing) < 0x3000) && (this->actor.xzDistToPlayer < 100.0f)) {
        EnEgol_SetupSlam(this);
    } else {
        s16 angleBehind;

        if (!((this->skelAnime.curFrame == 0.0f) ||
              ((9.0f <= this->skelAnime.curFrame) && (this->skelAnime.curFrame <= 15.0f)) ||
              ((24.0f <= this->skelAnime.curFrame) && (this->skelAnime.curFrame <= 29.0f)))) {
            Math_ApproachF(&this->actor.world.pos.x, this->waypointPos.x, 0.5f,
                           fabsf(Math_SinS(this->actor.world.rot.y) * 4.0f));
            Math_ApproachF(&this->actor.world.pos.z, this->waypointPos.z, 0.5f,
                           fabsf(Math_CosS(this->actor.world.rot.y) * 4.0f));
        }
        Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos), 1,
                           0x7D0, 0);
        EnEgol_FootstepEffects(this, play, 9.0f, 24.0f);
        angleBehind = this->actor.world.rot.y - this->actor.yawTowardsPlayer + 0x8000;
        if (ABS_ALT(angleBehind) < 0x2000) {
            this->waypoint--;
            if (this->waypoint < 0) {
                this->waypoint = 0;
            }
            EnEgol_SetupStop(this);
        } else {
            f32 dx = this->actor.world.pos.x - this->waypointPos.x;
            f32 dz = this->actor.world.pos.z - this->waypointPos.z;

            if (sqrtf(SQ(dx) + SQ(dz)) < 4.0f) {
                if (this->path != NULL) {
                    this->waypoint++;
                    if (this->waypoint >= this->path->count) {
                        this->waypoint -= 2;
                        if (this->waypoint < 0) {
                            this->waypoint = 0;
                        }
                        EnEgol_SetupStop(this);
                    }
                    EnEgol_GetWaypoint(this);
                }
            }
            if (this->actor.wallBgId != BGCHECK_SCENE) {
                EnEgol_DestroyBlocks(this, play, this->rightFootPos, this->leftFootPos);
            }
        }
    }
}

void EnEgol_SetupRetreat(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_RETREAT);
    this->laserCount = 0;
    EnEgol_GetWaypoint(this);
    this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos);
    this->actor.shape.rot.y = this->actor.world.rot.y + 0x8000;
    this->action = EYEGORE_ACTION_RETREAT;
    this->actionFunc = EnEgol_Retreat;
}

void EnEgol_Retreat(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    Vec3f toWaypoint;
    Vec3f spawnPos;
    Player* player = GET_PLAYER(play);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        EnEgol_SetupStop(this);
        return;
    }

    if ((ABS_ALT(angleToFacing) < 0x3000) && (fabsf(this->actor.world.pos.y - player->actor.world.pos.y) < 50.0f) &&
        (this->actor.xzDistToPlayer < 100.0f) && (player->invincibilityTimer == 0)) {
        func_800B8D50(play, &this->actor, 2.0f, (s32)Rand_CenteredFloat(0x2000) + this->actor.world.rot.y, 5.0f, 0x10);
    }
    Math_Vec3f_Copy(&spawnPos, &gZeroVec3f);
    if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
        if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->rightFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &enemyExtraZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
        if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
            Math_Vec3f_Copy(&spawnPos, &this->leftFootPos);
            spawnPos.y = this->actor.floorHeight + 2.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &enemyExtraZeroVec3s, 100, 0.005f, EYEGORE_EFFECT_IMPACT);
        }
    }
    Math_ApproachF(&this->actor.world.pos.x, this->waypointPos.x, 0.5f,
                   fabsf(Math_SinS(this->actor.world.rot.y) * 10.0f));
    Math_ApproachF(&this->actor.world.pos.z, this->waypointPos.z, 0.5f,
                   fabsf(Math_CosS(this->actor.world.rot.y) * 10.0f));
    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos), 1, 0x7D0,
                       0);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y + 0x8000, 1, 0x7D0, 0);
    EnEgol_FootstepEffects(this, play, 5.0f, 0.0f);
    toWaypoint.x = this->actor.world.pos.x - this->waypointPos.x;
    toWaypoint.z = this->actor.world.pos.z - this->waypointPos.z;
    if (sqrtf(SQXZ(toWaypoint)) < 4.0f) {
        this->waypoint--;
        if (this->waypoint < 0) {
            this->waypoint = 0;
            EnEgol_SetupStop(this);
        }
        EnEgol_GetWaypoint(this);
    }
}

void EnEgol_SetupLaser(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_LASER);
    this->actionTimer = 0;
    this->laserCount = 0;
    this->action = EYEGORE_ACTION_LASER;
    this->actionFunc = EnEgol_Laser;
}

void EnEgol_Laser(EnEgol* this, PlayState* play) {
    static s16 sLaserAngles[3] = { 0x1F40, 0xBB8, 0x7D0 };
    Vec3f stonePos;
    CollisionPoly* colPoly;
    Vec3f hitPos;
    s32 bgId;

    if ((ABS_ALT((s16)(this->actor.world.rot.y - this->actor.yawTowardsPlayer)) < 0x3000) &&
        (this->actor.xzDistToPlayer < 100.0f)) {
        this->chargingLaser = false;
        this->chargeLevel = 0;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupSlam(this);
    } else if (this->actor.xzDistToPlayer < this->minLaserRange) {
        this->chargingLaser = false;
        this->chargeLevel = 0;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupWalk(this);
    } else {
        if (this->chargingLaser) {
            switch (this->chargeLevel) {
                case 0:
                    this->waitTimer = 10;
                    this->chargeLevel++;
                    break;

                case 1:
                    Math_ApproachF(&this->chargeLightScale, 8.0f, 0.5f, 2.5f);
                    if (this->waitTimer == 0) {
                        this->waitTimer = 10;
                        this->chargeLevel++;
                    }
                    break;

                case 2:
                    Math_ApproachF(&this->chargeLightScale, 1.0f, 0.5f, 1.0f);
                    if (this->waitTimer == 0) {
                        this->chargeLevel = 0;
                        this->laserState = EYEGORE_LASER_FIRE;
                        this->chargingLaser = false;
                        this->chargeLightScale = 0.0f;
                    }
                    break;

                default:
                    break;
            }
            this->chargeLightRot += 0x7D0;
        }
        if ((this->laserState != EYEGORE_LASER_OFF) || (this->laserCount != 0)) {
            Math_SmoothStepToS(&this->headRot, -0x2710, 5, 0x1F4, 5);
        } else {
            Math_SmoothStepToS(&this->headRot, 0, 5, 0x1F4, 5);
        }
        if (this->laserState == EYEGORE_LASER_OFF) {
            if (this->laserCount >= 3) {
                EnEgol_SetupWalk(this);
            } else {
                this->laserRot.x = sLaserAngles[this->laserCount];
                this->laserScaleTarget.z = 0.0f;
                this->laserScale.z = 0.0f;
                if (this->laserCount == 0) {
                    this->laserState = EYEGORE_LASER_START;
                } else {
                    this->laserState = EYEGORE_LASER_FIRE;
                    this->laserScaleTarget.x = 0.03f;
                    this->laserScaleTarget.y = 0.03f;
                }
                this->laserCount++;
            }
        } else if (this->laserState >= EYEGORE_LASER_FIRE) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_BEAM - SFX_FLAG);
            if (this->laserState != EYEGORE_LASER_OFF) {
                EnEgol_DestroyBlocks(this, play, this->laserCollider.dim.quad[0], this->laserCollider.dim.quad[1]);
            }
            if ((this->actionTimer == 0) && BgCheck_EntityLineTest1(&play->colCtx, &this->laserCollider.dim.quad[3],
                                                                    &this->laserCollider.dim.quad[1], &hitPos, &colPoly,
                                                                    false, true, false, true, &bgId)) {
                Vec3s rotToNorm;
                f32 nx;
                f32 ny;
                f32 nz;
                s32 i;
                s32 quakeYOffset;
                Player* player = GET_PLAYER(play);

                nx = COLPOLY_GET_NORMAL(colPoly->normal.x);
                ny = COLPOLY_GET_NORMAL(colPoly->normal.y);
                nz = COLPOLY_GET_NORMAL(colPoly->normal.z);

                rotToNorm.x = RAD_TO_BINANG_ALT2(-Math_FAtan2F(-nz * ny, 1.0f));
                rotToNorm.z = RAD_TO_BINANG_ALT2(Math_FAtan2F(-nx * ny, 1.0f));

                if ((this->actor.world.pos.y - 50.0f) <= player->actor.world.pos.y) {
                    EnEgol_SpawnEffect(this, &hitPos, &rotToNorm, 100, 0.02f, EYEGORE_EFFECT_IMPACT);
                }
                quakeYOffset = 4 - (s32)(fabsf(player->actor.world.pos.y - this->actor.world.pos.y) * 0.02f);
                if (quakeYOffset > 4) {
                    quakeYOffset = 4;
                } else if (quakeYOffset < 1) {
                    quakeYOffset = 1;
                }
                if (true) {
                    Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
                }
                Actor_PlaySfx(&this->actor, NA_SE_EV_EXPLOSION);
                func_800B31BC(play, &hitPos, 40, -2, 255, 20);
                func_800BBFB0(play, &hitPos, 6.0f, 2, 120, 20, true);

                if ((this->actor.world.pos.y - 50.0f <= player->actor.world.pos.y) &&
                    (this->actor.floorBgId == BGCHECK_SCENE)) {
                    Math_Vec3f_Copy(&stonePos, &hitPos);
                    stonePos.x += Math_SinS(this->actor.world.rot.y) * 60.0f;
                    stonePos.z += Math_CosS(this->actor.world.rot.y) * 60.0f;
                    if (!Eyegore_CanMove(play, &this->actor, hitPos, stonePos))
                        stonePos = hitPos;
                    for (i = 0; i < 3; i++) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ESTONE, stonePos.x, stonePos.y, stonePos.z, 0,
                                    this->actor.world.rot.y, 0, ENESTONE_TYPE_SMALL);
                    }
                }
                if (this->actor.world.pos.y - 50.0f <= player->actor.world.pos.y) {
                    for (i = 0; i < 10; i++) {
                        EnEgol_SpawnEffect(this, &hitPos, &enemyExtraZeroVec3s, 30,
                                           (Rand_ZeroFloat(1.0f) * 0.1f) + 0.2f, EYEGORE_EFFECT_DEBRIS);
                    }
                }
                this->actionTimer = 1;
            }
            if (this->actionTimer != 0) {
                this->actionTimer++;
                if (this->actionTimer >= 5) {
                    this->laserState = EYEGORE_LASER_OFF;
                    Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                    this->actionTimer = 0;
                }
            }
            Math_ApproachF(&this->laserScale.x, this->laserScaleTarget.x, 0.5f, 0.5f);
            Math_ApproachF(&this->laserScale.y, this->laserScaleTarget.y, 0.5f, 0.5f);
            Math_ApproachF(&this->laserScale.z, this->laserScaleTarget.z, 0.5f, 0.5f);
        }
    }
}

void EnEgol_SetupStop(EnEgol* this) {
    if (!this->isRetreating) {
        EnEgol_ChangeAnim(this, EYEGORE_ANIM_LASER);
    } else {
        EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUN_END);
    }
    this->action = EYEGORE_ACTION_STOP;
    this->actionFunc = EnEgol_Stop;
}

void EnEgol_Stop(EnEgol* this, PlayState* play) {
    s32 isHome = false;
    f32 curFrame = this->skelAnime.curFrame;

    EnEgol_FootstepEffects(this, play, 28.0f, 13.0f);
    if (this->isRetreating) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 5, 0xBB8, 5);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        s16 angleToHome = this->actor.world.rot.y - this->actor.home.rot.y;
        if (ABS_ALT(angleToHome) < 0x64) {
            this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.home.rot.y;
            isHome = true;
        }
    } else {
        isHome = true;
    }
    if (isHome && (curFrame >= this->animEndFrame)) {
        this->isRetreating ^= true;
        if (!this->isRetreating) {
            EnEgol_ChangeAnim(this, EYEGORE_ANIM_SIT);
            Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_SIT);
            this->laserCount = 0;
            this->actionFunc = EnEgol_Wait;
        } else {
            EnEgol_SetupRetreat(this);
        }
    }
}

void EnEgol_SetupSlam(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM);
    this->action = EYEGORE_ACTION_SLAM;
    this->actionFunc = EnEgol_Slam;
}

void EnEgol_Slam(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->headRot, 0, 1, 0x3E8, 0);
    if (Animation_OnFrame(&this->skelAnime, 17.0f)) {
        s32 i;
        s32 spawnCount;
        Player* player = GET_PLAYER(play);
        Vec3f spawnPos;
        f32 dyToPlayer = fabsf(player->actor.world.pos.y - this->actor.world.pos.y);
        s32 quakeYOffset;

        Actor_PlaySfx(&this->actor, NA_SE_EV_EXPLOSION);
        quakeYOffset = 4 - (s32)(dyToPlayer * 0.02f);
        if (quakeYOffset > 4) {
            quakeYOffset = 4;
        } else if (quakeYOffset < 1) {
            quakeYOffset = 1;
        }
        Actor_RequestQuakeAndRumble(&this->actor, play, quakeYOffset, 2);
        if (this->actor.floorBgId == BGCHECK_SCENE) {
            Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
            spawnPos.x += Math_SinS(this->actor.world.rot.y) * 60.0f;
            spawnPos.y = this->actor.floorHeight;
            spawnPos.z += Math_CosS(this->actor.world.rot.y) * 60.0f;
            Vec3f origin = this->actor.world.pos;
            origin.y = this->actor.floorHeight;
            if (!Eyegore_CanMove(play, &this->actor, origin, spawnPos))
                spawnPos = origin;
            spawnCount = Rand_S16Offset(1, 3);
            for (i = 0; i < spawnCount; i++) {
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ESTONE, spawnPos.x, spawnPos.y, spawnPos.z, 0,
                            this->actor.world.rot.y, 0, ENESTONE_TYPE_LARGE);
            }
            Actor_SpawnFloorDustRing(play, &this->actor, &spawnPos, 30.0f, 30, 10.0f, 100, 30, true);
            Math_Vec3f_Copy(&spawnPos, &this->actor.world.pos);
            spawnPos.x += Math_SinS(this->actor.world.rot.y) * 55.0f;
            spawnPos.y = this->actor.floorHeight + 2.0f;
            spawnPos.z += Math_CosS(this->actor.world.rot.y) * 55.0f;
            EnEgol_SpawnEffect(this, &spawnPos, &enemyExtraZeroVec3s, 100, 0.03f, EYEGORE_EFFECT_IMPACT);
        }
        EnEgol_DestroyBlocks(this, play, this->rightHandPos, this->leftHandPos);
    }
    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupSlamWait(this);
    } else if ((this->skelAnime.curFrame <= 17.0f) && (this->skelAnime.curFrame >= 10.0f)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyCollider.base);
    }
}

void EnEgol_SetupPunch(EnEgol* this) {
    this->bodyCollider.elements[0].dim.modelSphere.radius = 40;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 40;
    this->hitPlayer = false;
    this->action = EYEGORE_ACTION_PUNCH;
    this->actionFunc = EnEgol_Punch;
}

void EnEgol_Punch(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->headRot, 0, 1, 0x3E8, 0);
    if (curFrame >= this->animEndFrame) {
        this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
        this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
        EnEgol_SetupWalk(this);
    } else if (!this->hitPlayer) {
        if ((this->bodyCollider.elements[0].base.atElemFlags & ATELEM_HIT) ||
            (this->bodyCollider.elements[1].base.atElemFlags & ATELEM_HIT)) {
            this->hitPlayer = true;
            func_800B8D50(play, &this->actor, 10.0f, this->actor.home.rot.y, 10.0f, 0);
        }
        if (!(this->bodyCollider.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyCollider.base);
        }
    }
}

void EnEgol_SetupSlamWait(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM_WAIT);
    this->actionTimer = 0;
    this->eyeCollider.elements[0].base.elemMaterial = ELEM_MATERIAL_UNK1;
    this->action = EYEGORE_ACTION_SLAM_WAIT;
    this->actionFunc = EnEgol_SlamWait;
}

void EnEgol_SlamWait(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        this->actionTimer++;
        if (this->actionTimer > 20) {
            this->eyeCollider.elements[0].base.elemMaterial = ELEM_MATERIAL_UNK2;
            EnEgol_SetupSlamEnd(this);
        }
    }
}

void EnEgol_SetupStunned(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUNNED);
    this->actionTimer = 0;
    this->eyeCollider.elements[0].base.elemMaterial = ELEM_MATERIAL_UNK1;
    this->bodyCollider.elements[0].dim.modelSphere.radius = 0;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 0;
    this->action = EYEGORE_ACTION_STUNNED;
    this->actionFunc = EnEgol_Stunned;
}

void EnEgol_Stunned(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        this->actionTimer++;
        if (this->actionTimer > 80) {
            this->eyeCollider.elements[0].base.elemMaterial = ELEM_MATERIAL_UNK2;
            EnEgol_SetupStunEnd(this);
        }
    }
}

void EnEgol_SetupStunEnd(EnEgol* this) {
    this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_STUN_END);
    this->action = EYEGORE_ACTION_STUN_END;
    this->actionFunc = EnEgol_StunEnd;
}

void EnEgol_StunEnd(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupSlamEnd(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_SLAM_END);
    this->actionTimer = 0;
    this->action = EYEGORE_ACTION_SLAM_END;
    this->actionFunc = EnEgol_SlamEnd;
}

void EnEgol_SlamEnd(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnEgol_SetupWalk(this);
    }
}

void EnEgol_SetupDamaged(EnEgol* this) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_DAMAGED);
    this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
    this->bodyCollider.elements[1].dim.modelSphere.radius = 20;
    this->action = EYEGORE_ACTION_DAMAGED;
    this->actionFunc = EnEgol_Damaged;
}

void EnEgol_Damaged(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        if (this->actor.colChkInfo.health > 0) {
            EnEgol_SetupWalk(this);
        } else {
            Enemy_StartFinishingBlow(play, &this->actor);
            Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_DEAD);
            this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->actionFunc = EnEgol_StartDeathCutscene;
        }
    }
}

void EnEgol_StartDeathCutscene(EnEgol* this, PlayState* play) {
    EnEgol_ChangeAnim(this, EYEGORE_ANIM_DEATH);
    this->action = EYEGORE_ACTION_DYING;
    this->actionFunc = EnEgol_Death;
    this->actor.shape.shadowScale = 0.0f;
}

void EnEgol_Death(EnEgol* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    if ((this->action == EYEGORE_ACTION_DEAD) && (this->waitTimer == 1)) {
        Actor_Kill(&this->actor);
    } else {
        if (Animation_OnFrame(&this->skelAnime, 46.0f)) {
            Actor_PlaySfx(&this->actor, NA_SE_EV_EXPLOSION);
            Actor_RequestQuakeAndRumble(&this->actor, play, 10, 5);
        }
        if ((curFrame >= this->animEndFrame) && (this->action != EYEGORE_ACTION_DEAD)) {
            s32 i;

            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, CLEAR_TAG_PARAMS(CLEAR_TAG_SMALL_EXPLOSION));
            this->waitTimer = 30;
            Actor_PlaySfx(&this->actor, NA_SE_IT_BOMB_EXPLOSION);
            this->action = EYEGORE_ACTION_DEAD;
            for (i = 0; i < 20; i++) {
                EnEgol_SpawnEffect(this, &this->actor.world.pos, &enemyExtraZeroVec3s, 10.0f + Rand_ZeroFloat(20.0f),
                                   0.01f + (0.02f * Rand_ZeroFloat(1.0f)), (i & 1) + EYEGORE_EFFECT_PIECE_LARGE);
            }
        }
    }
}

typedef enum { EYEGORE_HIT_NONE, EYEGORE_HIT_DAMAGE, EYEGORE_HIT_IMMUNE } EyegoreHitReaction;

void EnEgol_CollisionCheck(EnEgol* this, PlayState* play) {
    s16 angleToFacing = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
    s32 reaction = EYEGORE_HIT_NONE;

    if ((this->action == EYEGORE_ACTION_LASER) && (this->laserCollider.base.atFlags & AT_BOUNCED)) {
        this->laserCollider.base.atFlags &= ~AT_HIT;
        this->laserCollider.base.atFlags &= ~AT_BOUNCED;
        this->actionTimer = 0;
        this->laserState = EYEGORE_LASER_OFF;
        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
        EnEgol_SetupWalk(this);
    }
    if (this->eyeCollider.elements[0].base.acElemFlags & ACELEM_HIT) {
        reaction = EYEGORE_HIT_IMMUNE;
        switch (this->actor.colChkInfo.damageEffect) {
            case EYEGORE_DMGEFF_LIGHT_ARROW:
                if (ABS_ALT(angleToFacing) < 0x3000) {
                    if ((this->action == EYEGORE_ACTION_STUNNED) || (this->action == EYEGORE_ACTION_SLAM_WAIT)) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x,
                                    this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0,
                                    CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                        this->dmgEffectTimer = 20;
                        reaction = EYEGORE_HIT_DAMAGE;
                    } else if ((this->action >= EYEGORE_ACTION_WALK) && (this->action <= EYEGORE_ACTION_STUN_END) &&
                               !this->isRetreating) {
                        reaction = EYEGORE_HIT_NONE;
                        this->chargingLaser = false;
                        this->chargeLevel = 0;
                        this->actionTimer = 0;
                        this->laserState = EYEGORE_LASER_OFF;
                        Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                        Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                        Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA,
                                             40);
                        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                        EnEgol_SetupStunned(this);
                    }
                }
                break;

            case EYEGORE_DMGEFF_NONE:
                if ((this->action == EYEGORE_ACTION_SLAM_WAIT) || (this->action == EYEGORE_ACTION_STUNNED)) {
                    reaction = EYEGORE_HIT_DAMAGE;
                }
                break;

            default:
                break;
        }
    }
    if (reaction == EYEGORE_HIT_DAMAGE) {
        Actor_ApplyDamage(&this->actor);
        CollisionCheck_BlueBlood(play, NULL, &this->eyePos);
        CollisionCheck_BlueBlood(play, NULL, &this->eyePos);
        CollisionCheck_BlueBlood(play, NULL, &this->eyePos);
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 25);
        Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_DAMAGE);
        EnEgol_SetupDamaged(this);
    } else if (reaction == EYEGORE_HIT_IMMUNE) {
        Vec3f hitPos;

        hitPos.x = this->eyeCollider.elements[0].base.acDmgInfo.hitPos.x;
        hitPos.y = this->eyeCollider.elements[0].base.acDmgInfo.hitPos.y;
        hitPos.z = this->eyeCollider.elements[0].base.acDmgInfo.hitPos.z;
        Actor_PlaySfx(&this->actor, NA_SE_IT_SHIELD_BOUND);
        EffectSsHitmark_SpawnFixedScale(play, EFFECT_HITMARK_METAL, &hitPos);
        CollisionCheck_SpawnShieldParticlesMetal(play, &hitPos);
    }
}

void EnEgol_Update(Actor* thisx, PlayState* play) {
    EnEgol* this = (EnEgol*)thisx;
    Player* player = GET_PLAYER(play);
    if (this->actionFunc == EnEgol_Wait) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x7d0, 0);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    } else if (this->actionFunc == EnEgol_Stand ||
               (this->actionFunc == EnEgol_Laser && this->laserState < EYEGORE_LASER_FIRE)) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x7d0, 0);
    } else if (this->actionFunc == EnEgol_Walk) {
        this->waypoint = 0;
        this->waypointPos = player->actor.world.pos;
    }

    if (!((this->action == EYEGORE_ACTION_PUNCH) || (this->action == EYEGORE_ACTION_RETREAT) ||
          (this->action >= EYEGORE_ACTION_DYING))) {
        s16 angleToFacing = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;
        s16 angleBehind = this->actor.world.rot.y - this->actor.yawTowardsPlayer + 0x8000;

        if ((ABS_ALT(angleToFacing) > 0x1888) && (ABS_ALT(angleBehind) > 0x2000) &&
            (this->actor.xzDistToPlayer < 100.0f)) {
            if (angleToFacing < 0) {
                if (this->animIndex != EYEGORE_ANIM_LEFT_PUNCH) {
                    EnEgol_ChangeAnim(this, EYEGORE_ANIM_LEFT_PUNCH);
                }
            } else if (this->animIndex != EYEGORE_ANIM_RIGHT_PUNCH) {
                EnEgol_ChangeAnim(this, EYEGORE_ANIM_RIGHT_PUNCH);
            }
            this->chargingLaser = false;
            this->chargeLevel = 0;
            this->actionTimer = 0;
            this->laserState = EYEGORE_LASER_OFF;
            Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
            Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
            Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
            EnEgol_SetupPunch(this);
        }
    }
    if (this->action != EYEGORE_ACTION_WAIT) {
        SkelAnime_Update(&this->skelAnime);
    }
    if (this->laserState == EYEGORE_LASER_OFF) {
        Math_ApproachZeroF(&this->laserLightScale, 0.5f, 0.5f);
    } else if (this->laserState >= EYEGORE_LASER_FIRE) {
        Math_ApproachF(&this->laserLightScale, 1.0f, 0.5f, 0.5f);
    }
    Actor_SetScale(&this->actor, 0.015f);

    if (!((this->action == EYEGORE_ACTION_STOP) || (this->action == EYEGORE_ACTION_RETREAT))) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
    this->texScroll += 12;
    DECR(this->waitTimer);
    DECR(this->eyeShutTimer);
    DECR(this->dmgEffectTimer);

    EnEgol_UpdateEffects(this, play);
    Vec3f before = this->actor.world.pos;
    s16 action = this->action;
    if (action == EYEGORE_ACTION_LASER && this->laserState == EYEGORE_LASER_ON && !this->waitTimer &&
        !this->actionTimer)
        this->actionTimer = 1;
    this->actionFunc(this, play);
    if ((action == EYEGORE_ACTION_WALK || action == EYEGORE_ACTION_RETREAT) &&
        !Eyegore_CanMove(play, &this->actor, before, this->actor.world.pos)) {
        this->actor.world.pos.x = before.x;
        this->actor.world.pos.z = before.z;
        if (action == EYEGORE_ACTION_RETREAT && this->action == EYEGORE_ACTION_RETREAT)
            EnEgol_SetupStop(this);
    }
    Math_Vec3f_Copy(&this->actor.focus.pos, &this->eyePos);
    this->actor.focus.rot.x = this->actor.world.rot.x;
    this->actor.focus.rot.y = this->actor.world.rot.y;
    this->actor.focus.rot.z = this->actor.world.rot.z;
    if (!((this->action == EYEGORE_ACTION_SLAM_WAIT) || (this->action == EYEGORE_ACTION_WAIT) ||
          (this->action == EYEGORE_ACTION_RETREAT) || (this->action == EYEGORE_ACTION_STAND) ||
          (this->action == EYEGORE_ACTION_LASER) || (this->action == EYEGORE_ACTION_STUNNED) ||
          (this->action == EYEGORE_ACTION_DAMAGED) || (this->action == EYEGORE_ACTION_DYING))) {

        this->pupilRot.x = Math_Vec3f_Pitch(&this->eyePos, &player->actor.world.pos);
        this->pupilRot.y = -Math_Vec3f_Yaw(&this->eyePos, &player->actor.world.pos);

        this->pupilRot.y += this->actor.world.rot.y;

        if (this->pupilRot.x > 0x7D0) {
            this->pupilRot.x = 0x7D0;
        } else if (this->pupilRot.x < -0x7D0) {
            this->pupilRot.x = -0x7D0;
        }
        if (this->pupilRot.y > 0x1770) {
            this->pupilRot.y = 0x1770;
        } else if (this->pupilRot.y < -0x1770) {
            this->pupilRot.y = -0x1770;
        }
    } else {
        this->pupilRot.x = this->pupilRot.y = 0;
    }
    if (this->eyeShutTimer == 0) {
        if (((this->action == EYEGORE_ACTION_WALK) || (this->action == EYEGORE_ACTION_LASER)) && !this->isRetreating) {
            Actor* projectile = enemyExtraProjectileQuery(play, &this->actor, 80.0f, 0x138B0);

            if ((projectile != NULL) && !((projectile->id == 0x16) && (projectile->params == ARROW_TYPE_LIGHT))) {
                this->eyelidRotTarget = 0xFA0;
                this->eyeShutTimer = 20;
                if (this->action == EYEGORE_ACTION_LASER) {
                    this->chargingLaser = false;
                    this->chargeLevel = 0;
                    this->actionTimer = 0;
                    this->laserState = EYEGORE_LASER_OFF;
                    Math_Vec3f_Copy(&this->laserScale, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserScaleTarget, &gZeroVec3f);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[1], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[3], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[0], &this->laserBase);
                    Math_Vec3f_Copy(&this->laserCollider.dim.quad[2], &this->laserBase);
                    EnEgol_SetupWalk(this);
                }
            }
        } else {
            this->eyelidRotTarget = 0;
        }
    } else if (this->eyeShutTimer == 1) {
        this->eyelidRotTarget = 0;
    }
    Math_SmoothStepToS(&this->eyelidRot, this->eyelidRotTarget, 1, 0x7D0, 0);
    if (this->action != EYEGORE_ACTION_DYING && this->action != EYEGORE_ACTION_DEAD)
        EnEgol_CollisionCheck(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 50.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
    if (this->action != EYEGORE_ACTION_DEAD && this->action != EYEGORE_ACTION_DYING) {
        if (this->action != EYEGORE_ACTION_DAMAGED) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->eyeCollider.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
    }
    if (this->action != EYEGORE_ACTION_DYING && this->action != EYEGORE_ACTION_DEAD &&
        this->laserState >= EYEGORE_LASER_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->laserCollider.base);
    }
}

s32 EnEgol_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnEgol* this = (EnEgol*)thisx;

    if (limbIndex == EYEGORE_LIMB_HEAD) {
        rot->z += this->headRot;
    }
    if (limbIndex == EYEGORE_LIMB_UPPER_EYELID) {
        rot->z += this->eyelidRot;
    }
    if (limbIndex == EYEGORE_LIMB_LOWER_EYELID) {
        rot->z += -1 * this->eyelidRot;
    }
    if (limbIndex == EYEGORE_LIMB_PUPIL) {
        rot->y += this->pupilRot.y;
        rot->z += this->pupilRot.x;
    }
    if (limbIndex == EYEGORE_LIMB_EYEBALL) {
        s32 eyeColorIndex = 0;

        OPEN_DISPS(play->state.gfxCtx);

        if ((this->action == EYEGORE_ACTION_DAMAGED) || (this->action == EYEGORE_ACTION_DYING)) {
            eyeColorIndex = 2;
        } else if ((this->action == EYEGORE_ACTION_STUNNED) || (this->action == EYEGORE_ACTION_SLAM_WAIT)) {
            eyeColorIndex = 1;
        }
        this->eyePrimColor.r = sEyePrimColors[eyeColorIndex].r;
        this->eyePrimColor.g = sEyePrimColors[eyeColorIndex].g;
        this->eyePrimColor.b = sEyePrimColors[eyeColorIndex].b;
        this->eyeEnvColor.r = sEyeEnvColors[eyeColorIndex].r;
        this->eyeEnvColor.g = sEyeEnvColors[eyeColorIndex].g;
        this->eyeEnvColor.b = sEyeEnvColors[eyeColorIndex].b;

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0xFF, (u8)this->eyePrimColor.r, (u8)this->eyePrimColor.g,
                        (u8)this->eyePrimColor.b, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, (u8)this->eyeEnvColor.r, (u8)this->eyeEnvColor.g, (u8)this->eyeEnvColor.b, 255);
        gSPSegment(POLY_OPA_DISP++, 0x09, enemyExtraTexScroll(play->state.gfxCtx, 0, this->texScroll));

        CLOSE_DISPS();
    }
    return false;
}

void EnEgol_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnEgol* this = (EnEgol*)thisx;
    Vec3f footOffset = { 1000.0f, 0.0f, 0.0f };
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == EYEGORE_LIMB_LEFT_HAND) {
        Matrix_MultVec3f(&gZeroVec3f, &this->leftHandPos);
    }
    if (limbIndex == EYEGORE_LIMB_RIGHT_HAND) {
        Matrix_MultVec3f(&gZeroVec3f, &this->rightHandPos);
    }
    if (limbIndex == EYEGORE_LIMB_LASER_ATTACH) {
        Matrix_MultVec3f(&gZeroVec3f, &this->eyePos);
    }
    if ((limbIndex == EYEGORE_LIMB_LASER_ATTACH) && (this->laserState >= EYEGORE_LASER_FIRE)) {
        Vec3f laserLengthZ = { 0.0f, 0.0f, 0.0f };
        Vec3f laserVec;
        f32 targetScale;

        Matrix_MultVec3f(&zeroVec, &this->laserBase);
        this->laserCollider.dim.quad[3].x =
            this->laserBase.x + Math_SinS(this->actor.world.rot.y + 0x4000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[3].y = this->laserBase.y;
        this->laserCollider.dim.quad[3].z =
            this->laserBase.z + Math_CosS(this->actor.world.rot.y + 0x4000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[2].x =
            this->laserBase.x + Math_SinS(this->actor.world.rot.y + 0xC000) * this->laserScale.x * 77.0f;
        this->laserCollider.dim.quad[2].y = this->laserBase.y;
        this->laserCollider.dim.quad[2].z =
            this->laserBase.z + Math_CosS(this->actor.world.rot.y + 0xC000) * this->laserScale.x * 77.0f;

        f32 dropPerScale = Math_SinS(this->laserRot.x) * 700.f;
        f32 height = this->laserBase.y + 20.f - this->actor.floorHeight;
        targetScale = dropPerScale > .01f ? height / dropPerScale : 1.4f;
        this->laserScaleTarget.z = MAX(0.f, MIN(1.4f, targetScale));
        Matrix_Push();
        Matrix_RotateYS(this->actor.world.rot.y, MTXMODE_NEW);
        Matrix_RotateXS(this->laserRot.x, MTXMODE_APPLY);
        laserLengthZ.z = this->laserScale.z * 700.0f;
        Matrix_MultVec3f(&laserLengthZ, &laserVec);
        Matrix_Pop();
        if (this->laserState == EYEGORE_LASER_ON && this->waitTimer <= 8 &&
            !Eyegore_LaserHitsSurface(play, this->laserBase, laserVec)) {
            this->laserScale.x *= .65f;
            this->laserScale.y *= .65f;
            this->laserScaleTarget.x = this->laserScaleTarget.y = 0.f;
        }
        f32 wallFraction = Eyegore_LaserWallFraction(play, this->laserBase, laserVec);
        if (wallFraction < 1.f) {
            this->laserScale.z *= wallFraction;
            laserVec.x *= wallFraction;
            laserVec.y *= wallFraction;
            laserVec.z *= wallFraction;
            if (!this->actionTimer)
                this->actionTimer = 1;
        }
        this->laserCollider.dim.quad[1].x = this->laserCollider.dim.quad[3].x + laserVec.x;
        this->laserCollider.dim.quad[1].y = this->laserCollider.dim.quad[3].y + laserVec.y;
        this->laserCollider.dim.quad[1].z = this->laserCollider.dim.quad[3].z + laserVec.z;
        this->laserCollider.dim.quad[0].x = this->laserCollider.dim.quad[2].x + laserVec.x;
        this->laserCollider.dim.quad[0].y = this->laserCollider.dim.quad[2].y + laserVec.y;
        this->laserCollider.dim.quad[0].z = this->laserCollider.dim.quad[2].z + laserVec.z;
        Collider_SetQuadVertices(&this->laserCollider, &this->laserCollider.dim.quad[0],
                                 &this->laserCollider.dim.quad[1], &this->laserCollider.dim.quad[2],
                                 &this->laserCollider.dim.quad[3]);
        if (this->laserState == EYEGORE_LASER_FIRE)
            this->waitTimer = 35;
        this->laserState = EYEGORE_LASER_ON;
    }
    if ((limbIndex == EYEGORE_LIMB_LASER_ATTACH) && (this->laserState == EYEGORE_LASER_START)) {
        this->chargingLaser = true;
        this->laserState = EYEGORE_LASER_CHARGING;
        this->laserScaleTarget.x = 0.03f;
        this->laserScaleTarget.y = 0.03f;
    }
    if (limbIndex == EYEGORE_LIMB_LEFT_FOOT) {
        Matrix_MultVec3f(&footOffset, &this->leftFootPos);
    }
    if (limbIndex == EYEGORE_LIMB_RIGHT_FOOT) {
        Matrix_MultVec3f(&footOffset, &this->rightFootPos);
    }
    if ((limbIndex == EYEGORE_LIMB_HEAD) || (limbIndex == EYEGORE_LIMB_LEFT_SHOULDER) ||
        (limbIndex == EYEGORE_LIMB_LEFT_ARM) || (limbIndex == EYEGORE_LIMB_LEFT_HAND) ||
        (limbIndex == EYEGORE_LIMB_RIGHT_SHOULDER) || (limbIndex == EYEGORE_LIMB_RIGHT_ARM) ||
        (limbIndex == EYEGORE_LIMB_RIGHT_HAND) || (limbIndex == EYEGORE_LIMB_UPPER_EYELID) ||
        (limbIndex == EYEGORE_LIMB_LOWER_EYELID) || (limbIndex == EYEGORE_LIMB_HIPS) ||
        (limbIndex == EYEGORE_LIMB_LEFT_SHIN) || (limbIndex == EYEGORE_LIMB_RIGHT_SHIN)) {
        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
        this->bodyPartIndex++;
        if (this->bodyPartIndex >= EYEGORE_BODYPART_MAX) {
            this->bodyPartIndex = 0;
        }
    }
    Collider_UpdateSpheres(limbIndex, &this->bodyCollider);
    Collider_UpdateSpheres(limbIndex, &this->eyeCollider);
}

void EnEgol_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnEgol* this = (EnEgol*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    if (this->action != EYEGORE_ACTION_DEAD) {
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gEyegoreEyeLaserTexAnim));
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              EnEgol_OverrideLimbDraw, EnEgol_PostLimbDraw, &this->actor);
        POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
    }
    if (this->dmgEffectTimer != 0) {
        f32 drawDmgEffAlpha = 0.05f * this->dmgEffectTimer;

        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EYEGORE_BODYPART_MAX, 0.8f, 0.8f,
                                drawDmgEffAlpha, ACTOR_DRAW_DMGEFF_LIGHT_ORBS);
    }
    if (this->laserState >= EYEGORE_LASER_FIRE) {
        this->laserRot.y = this->actor.world.rot.y;
        gSPSegment(POLY_OPA_DISP++, 0x08, enemyExtraTexScroll(play->state.gfxCtx, 0, this->texScroll));
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        Matrix_Translate(this->laserBase.x, this->laserBase.y, this->laserBase.z, MTXMODE_NEW);
        Matrix_RotateZYX(this->laserRot.x, this->laserRot.y, 0, MTXMODE_APPLY);
        Matrix_Scale(this->laserScale.x, this->laserScale.y, this->laserScale.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gEyegoreLaserDL);
    }
    if (((this->action == EYEGORE_ACTION_LASER) && (this->laserState >= EYEGORE_LASER_FIRE)) || this->chargingLaser) {
        const Gfx* orbMaterial = comboEnemyKeepAsset(0x1001, 0x23348);
        const Gfx* orbModel = comboEnemyKeepAsset(0x1001, 0x23428);
        if (orbMaterial && orbModel) {
            Gfx_SetupDL25_Xlu(play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, orbMaterial);
            if (!this->chargingLaser) {
                s32 i;
                f32 laserLightScaleMod;
                f32 laserLightAlpha;

                gDPSetEnvColor(POLY_XLU_DISP++, 155, 255, 255, 128);
                Matrix_Translate(this->eyePos.x, this->eyePos.y, this->eyePos.z, MTXMODE_NEW);
                Matrix_Scale(this->laserLightScale, this->laserLightScale, this->laserLightScale, MTXMODE_APPLY);

                laserLightScaleMod = 10.0f;
                laserLightAlpha = 80.0f;
                for (i = 0; i < ARRAY_COUNT(sLightOrbColors); i++) {
                    Matrix_Push();
                    Matrix_Scale(laserLightScaleMod, laserLightScaleMod, laserLightScaleMod, MTXMODE_APPLY);
                    Matrix_ReplaceRotation(&play->billboardMtxF);
                    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, sLightOrbColors[i].r, sLightOrbColors[i].g,
                                    sLightOrbColors[i].b, laserLightAlpha);
                    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                    gSPDisplayList(POLY_XLU_DISP++, orbModel);
                    Matrix_Pop();
                    laserLightScaleMod = 3.0f;
                    laserLightAlpha = 200.0f;
                }
            } else {
                gDPSetEnvColor(POLY_XLU_DISP++, 155, 255, 255, 128);
                Matrix_Translate(this->eyePos.x, this->eyePos.y, this->eyePos.z, MTXMODE_NEW);
                Matrix_Scale(this->chargeLightScale, this->chargeLightScale, this->chargeLightScale, MTXMODE_APPLY);
                Matrix_Push();
                Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
                Matrix_ReplaceRotation(&play->billboardMtxF);
                Matrix_RotateZS(this->chargeLightRot, MTXMODE_APPLY);
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
                MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, orbModel);
                Matrix_Pop();
            }
        }
    }
    EnEgol_DrawEffects(this, play);

    CLOSE_DISPS();
}

void EnEgol_SpawnEffect(EnEgol* this, Vec3f* pos, Vec3s* rot, s16 lifetime, f32 scale, s16 type) {
    EnEgolEffect* effect = this->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isActive) {
            effect->isActive = true;
            effect->scale = scale;
            effect->pos = *pos;
            effect->timer = lifetime;
            effect->alpha = 255;
            effect->type = type;
            effect->rot.x = Rand_CenteredFloat(0x7530);
            effect->rot.y = Rand_CenteredFloat(0x7530);
            effect->rot.z = Rand_CenteredFloat(0x7530);
            if ((effect->type == EYEGORE_EFFECT_PIECE_LARGE) || (effect->type == EYEGORE_EFFECT_PIECE_SMALL)) {
                effect->accel.y = -1.0f;
                effect->velocity.x = 4.0f * (Rand_ZeroOne() - 0.5f);
                effect->velocity.y = 10.0f + (10.0f * Rand_ZeroOne());
                effect->velocity.z = 4.0f * (Rand_ZeroOne() - 0.5f);
                break;
            } else if (effect->type == EYEGORE_EFFECT_DEBRIS) {
                effect->accel.y = -1.0f;
                effect->velocity.x = 0.5f * (Rand_ZeroOne() - 0.5f);
                effect->velocity.y = 5.0f + (5.0f * Rand_ZeroOne());
                effect->velocity.z = 0.5f * (Rand_ZeroOne() - 0.5f);
                effect->timer = 20.0f + Rand_ZeroFloat(10.0f);
                break;
            } else if (effect->type == EYEGORE_EFFECT_IMPACT) {
                effect->rot.x = rot->x;
                effect->rot.y = 0;
                effect->rot.z = rot->z;
                break;
            }
        }
    }
}

void EnEgol_UpdateEffects(EnEgol* this, PlayState* play) {
    EnEgolEffect* effect = this->effects;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            if (this->action == EYEGORE_ACTION_DYING) {
                effect->timer = 0;
            }
            if (effect->type != EYEGORE_EFFECT_IMPACT) {
                effect->rot.x += 0x100;
                effect->rot.z += 0x130;

                effect->pos.x += effect->velocity.x;
                effect->pos.y += effect->velocity.y;
                effect->pos.z += effect->velocity.z;

                effect->velocity.y += effect->accel.y;
            }
            if (effect->timer != 0) {
                effect->timer--;
            } else if (effect->type == EYEGORE_EFFECT_IMPACT) {
                effect->alpha -= 10;
                if (effect->alpha < 10) {
                    effect->isActive = false;
                }
            } else {
                effect->isActive = false;
            }
        }
    }
}

void EnEgol_DrawEffects(EnEgol* this, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    EnEgolEffect* effect = this->effects;
    s16 i;

    OPEN_DISPS(gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            Matrix_Push();

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);

            switch (effect->type) {
                case EYEGORE_EFFECT_IMPACT:
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 155, 155, 155, 255);
                    Matrix_Translate(0.0f, 50.0f, 0.0f, MTXMODE_APPLY);
                    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, effect->alpha);
                    gSPDisplayList(POLY_OPA_DISP++, gEyegoreEffectImpactDL);
                    break;

                case EYEGORE_EFFECT_PIECE_LARGE:
                case EYEGORE_EFFECT_PIECE_SMALL:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
                    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, effect->alpha);
                    if (effect->type == EYEGORE_EFFECT_PIECE_LARGE) {
                        gSPDisplayList(POLY_OPA_DISP++, gEyegoreEffectLargeBodyPieceDL);
                    } else {
                        gSPDisplayList(POLY_OPA_DISP++, gEyegoreEffectSmallBodyPieceDL);
                    }
                    break;

                case EYEGORE_EFFECT_DEBRIS:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
                    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
                    gSPDisplayList(POLY_OPA_DISP++, gEyegoreEffectSolidDebrisDL);
                    break;

                default:
                    break;
            }
            Matrix_Pop();
        }
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x184, En_Egol_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_EGOL, En_Egol_Profile, EnEgol_Init, EnEgol_Destroy, EnEgol_Update,
                     EnEgol_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x184u, 0x18du, 0xb6d3d288u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_EGOL_ORIGIN, generatedNativeOrigin);
