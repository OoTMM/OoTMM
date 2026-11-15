#include "mm_native_compat.h"
#define gBioDekuBabaHeadAttackAnim (*(AnimationHeader*)0x060006a4)
#define gBioDekuBabaHeadChargeAnim (*(AnimationHeader*)0x06000a5c)
#define gBioDekuBabaLilyPadIdleAnim (*(AnimationHeader*)0x06000abc)
#define gBioDekuBabaLilyPadSkel (*(FlexSkeletonHeader*)0x060024e0)
#define gBioDekuBabaHeadDamagedAnim (*(AnimationHeader*)0x06002f0c)
#define gBioDekuBabaHeadSpotAnim (*(AnimationHeader*)0x06003448)
#define gBioDekuBabaHeadEyeFlashTexAnim (*((u8 (*)[1])0x06006240))
#define gBioDekuBabaHeadSkel (*(FlexSkeletonHeader*)0x06006378)
#define gBioDekuBabaHeadChompAnim (*(AnimationHeader*)0x06006484)
#define gBioDekuBabaHeadTransformAnim (*(AnimationHeader*)0x06006e50)
#define gBioDekuBabaHeadIdleAnim (*(AnimationHeader*)0x06007488)
#define gBioDekuBabaHeadWalkAnim (*(AnimationHeader*)0x06007908)

#include "types/boss_05.h"
#define TATL_HINT_ID_BIO_DEKU_BABA 0xff
#undef ACTOR_BOSS_05
#define ACTOR_BOSS_05 0x12d
#undef OBJECT_BOSS05
#define OBJECT_BOSS05 0x115e
#undef NA_SE_EV_OUT_OF_WATER
#define NA_SE_EV_OUT_OF_WATER 0x2808
#undef NA_SE_EV_ICE_BROKEN
#define NA_SE_EV_ICE_BROKEN 0x28cb
#undef NA_SE_EN_MIZUBABA2_VOICE
#define NA_SE_EN_MIZUBABA2_VOICE 0x3806
#undef NA_SE_EN_MIZUBABA2_DAMAGE
#define NA_SE_EN_MIZUBABA2_DAMAGE 0x3807
#undef NA_SE_EN_MIZUBABA1_DAMAGE
#define NA_SE_EN_MIZUBABA1_DAMAGE 0x381e
#undef NA_SE_EN_MIZUBABA_DEAD
#define NA_SE_EN_MIZUBABA_DEAD 0x381f
#undef NA_SE_EN_MIZUBABA_TRANSFORM
#define NA_SE_EN_MIZUBABA_TRANSFORM 0x3825
#undef NA_SE_EN_MIZUBABA2_WALK
#define NA_SE_EN_MIZUBABA2_WALK 0x3827
#undef NA_SE_EN_MIZUBABA2_ATTACK
#define NA_SE_EN_MIZUBABA2_ATTACK 0x3837
#undef NA_SE_EN_MIZUBABA1_MOUTH
#define NA_SE_EN_MIZUBABA1_MOUTH 0x3860
#undef NA_SE_EN_MIZUBABA1_ATTACK
#define NA_SE_EN_MIZUBABA1_ATTACK 0x3861
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef CollisionHeader_GetVirtual
#define CollisionHeader_GetVirtual ((void (*)(CollisionHeader* colHeader, CollisionHeader** dest))(void*)FOREIGN_ENEMY_API_LILYPAD_COLLISION)
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Flags_GetClear
extern u8 FOREIGN_ENEMY_API_GET_CLEAR[];
#define Flags_GetClear ((s32 (*)(struct PlayState* play, s32 roomNumber))(void*)FOREIGN_ENEMY_API_GET_CLEAR)
#undef Actor_ChangeCategory
#define Actor_ChangeCategory ((void (*)(struct PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory))(void*)FOREIGN_ENEMY_API_CATEGORY)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_SpawnAsChild
#define Actor_SpawnAsChild ((Actor* (*)(ActorContext* actorCtx, Actor* parent, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN_CHILD)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Math_Atan2S
#define Math_Atan2S ((s16 (*)(f32 y, f32 x))(void*)FOREIGN_ENEMY_API_ATAN2S)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_MultVecY
#define Matrix_MultVecY ((void (*)(f32 y, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Y)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef AnimatedMat_Draw
#define AnimatedMat_Draw ((void (*)(struct PlayState* play, AnimatedMaterial* matAnim))(void*)FOREIGN_ENEMY_API_MATERIAL_DRAW)
#undef SkelAnime_DrawTransformFlexOpa
#define SkelAnime_DrawTransformFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, TransformLimbDrawOpa transformLimbDraw, struct Actor* actor))(void*)FOREIGN_ENEMY_API_TRANSFORM_FLEX_OPA)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef EffectSsEnIce_Spawn
#define EffectSsEnIce_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, f32 scale, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s32 life))0x8001ddb8u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef DynaPolyActor_Init
#define DynaPolyActor_Init ((void (*)(DynaPolyActor* dynaActor, s32 transformFlags))0x80035260u)
#undef DynaPoly_SetBgActor
#define DynaPoly_SetBgActor ((s32 (*)(struct PlayState* play, DynaCollisionContext* dyna, struct Actor* actor, CollisionHeader* colHeader))0x800313a4u)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef DynaPoly_DeleteBgActor
#define DynaPoly_DeleteBgActor ((void (*)(struct PlayState* play, DynaCollisionContext* dyna, s32 bgId))0x80031638u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef EffectSsGRipple_Spawn
#define EffectSsGRipple_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life))0x8001cf3cu)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef EffectSsBubble_Spawn
#define EffectSsBubble_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale))0x8001cedcu)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d21cu)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Matrix_Get
#define Matrix_Get ((void (*)(MtxF* dest))0x800aa740u)
#undef Matrix_MtxFToYXZRot
#define Matrix_MtxFToYXZRot ((void (*)(MtxF* src, Vec3s* dest, s32 nonUniformScale))0x800abf50u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef Gfx_SetFog
#define Gfx_SetFog ((Gfx* (*)(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 n, s32 f))0x8007dc40u)
#undef Play_SetFog
#define Play_SetFog ((Gfx* (*)(PlayState* this, Gfx* gfx))0x8009a574u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

#define TIMER_CURRENT_ACTION 0
#define TIMER_FALLING_HEAD_FALL 2

void Boss05_Init(Actor* thisx, PlayState* play);
void Boss05_Destroy(Actor* thisx, PlayState* play);
void Boss05_Update(Actor* thisx, PlayState* play);
void Boss05_Draw(Actor* thisx, PlayState* play);

void Boss05_LilyPadWithHead_SetupMove(Boss05* this, PlayState* play);
void Boss05_LilyPadWithHead_Move(Boss05* this, PlayState* play);
void Boss05_LilyPad_Idle(Boss05* this, PlayState* play);
void Boss05_FallingHead_Fall(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupTransform(Boss05* this, PlayState* play);
void Boss05_WalkingHead_Transform(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupIdle(Boss05* this, PlayState* play);
void Boss05_WalkingHead_Idle(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupWalk(Boss05* this, PlayState* play);
void Boss05_WalkingHead_Walk(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupSpottedPlayer(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SpottedPlayer(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupCharge(Boss05* this, PlayState* arg1);
void Boss05_WalkingHead_Charge(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupAttack(Boss05* this, PlayState* arg1);
void Boss05_WalkingHead_Attack(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupDamaged(Boss05* this, PlayState* play);
void Boss05_WalkingHead_Damaged(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupStunned(Boss05* this, PlayState* play);
void Boss05_WalkingHead_SetupFreeze(Boss05* this, PlayState* play);
void Boss05_WalkingHead_Stunned(Boss05* this, PlayState* play);
void Boss05_Fragment_Move(Boss05* this, PlayState* play);

typedef enum BioDekuBabaDrawDmgEffState {
    BIO_BABA_DRAW_DMGEFF_STATE_NONE,
    BIO_BABA_DRAW_DMGEFF_STATE_FIRE_INIT,
    BIO_BABA_DRAW_DMGEFF_STATE_FIRE_ACTIVE,
    BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_INIT = 10,
    BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_ACTIVE,
    BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT = 20,
    BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_ACTIVE
} BioDekuBabaDrawDmgEffState;

typedef enum BioDekuBabaFragmentState {
    BIO_BABA_FRAGMENT_STATE_SPAWNED,
    BIO_BABA_FRAGMENT_STATE_UNDERWATER,
    BIO_BABA_FRAGMENT_STATE_ABOVE_WATER,
    BIO_BABA_FRAGMENT_STATE_FLYING_THROUGH_AIR
} BioDekuBabaFragmentState;

typedef enum BioDekuBabaHeadHitReaction {
    BIO_BABA_HEAD_HIT_REACTION_NONE_OR_DAMAGED,
    BIO_BABA_HEAD_HIT_REACTION_DEAD = 2,
    BIO_BABA_HEAD_HIT_REACTION_DEATCH = 10
} BioDekuBabaHeadHitReaction;

typedef enum BioDekuBabaLilyPadWithHeadAttackState {
    BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_IDLE,
    BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_WIND_UP,
    BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_LUNGE_ATTACK,
    BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_FLIP_ATTACK = 10
} BioDekuBabaLilyPadWithHeadAttackState;

typedef enum BioDekuBabaLilyPadWithHeadMovementState {
    BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_IDLE,
    BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_WIND_UP,
    BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_LUNGE_ATTACK,
    BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_FLIP_ATTACK
} BioDekuBabaLilyPadWithHeadMovementState;

BgCamInfo sBioBabaLilypadColCamDataList[] = {
    { 0x0000, 0, NULL },
};

SurfaceType sBioBabaLilypadColSurfaceType[] = {
    { { 0x00010000, 0x000007C8 } },
};

CollisionPoly sBioBabaLilypadColPolygons[] = {
    { 0x0000, { { 0x8000, 0x0001, 0x0002 } }, { 0x0000, 0x7FFF, 0x0000 }, 0x0000 },
    { 0x0000, { { 0x8000, 0x0002, 0x0003 } }, { 0x0000, 0x7FFF, 0x0000 }, 0x0000 },
    { 0x0000, { { 0x8004, 0x0005, 0x0000 } }, { 0x0000, 0x0000, 0x8001 }, 0xF254 },
    { 0x0000, { { 0x8004, 0x0000, 0x0003 } }, { 0x0000, 0x0000, 0x8001 }, 0xF254 },
    { 0x0000, { { 0x8006, 0x0004, 0x0003 } }, { 0x7FFF, 0x0000, 0x0000 }, 0xF254 },
    { 0x0000, { { 0x8006, 0x0003, 0x0002 } }, { 0x7FFF, 0x0000, 0x0000 }, 0xF254 },
    { 0x0000, { { 0x8007, 0x0006, 0x0002 } }, { 0x0000, 0x0000, 0x7FFF }, 0xF254 },
    { 0x0000, { { 0x8007, 0x0002, 0x0001 } }, { 0x0000, 0x0000, 0x7FFF }, 0xF254 },
    { 0x0000, { { 0x8005, 0x0007, 0x0001 } }, { 0x8001, 0x0000, 0x0000 }, 0xF254 },
    { 0x0000, { { 0x8005, 0x0001, 0x0000 } }, { 0x8001, 0x0000, 0x0000 }, 0xF254 },
};

Vec3s sBioBabaLilypadColVertices[] = {
    { -3500, 0, -3500 },    { -3500, 0, 3500 },      { 3500, 0, 3500 },     { 3500, 0, -3500 },
    { 3500, -4000, -3500 }, { -3500, -4000, -3500 }, { 3500, -4000, 3500 }, { -3500, -4000, 3500 },
};

CollisionHeader sBioBabaLilypadCol = { { -3500, -4000, -3500 },
                                       { 3500, 0, 3500 },
                                       ARRAY_COUNT(sBioBabaLilypadColVertices),
                                       sBioBabaLilypadColVertices,
                                       ARRAY_COUNT(sBioBabaLilypadColPolygons),
                                       sBioBabaLilypadColPolygons,
                                       sBioBabaLilypadColSurfaceType,
                                       sBioBabaLilypadColCamDataList,
                                       0,
                                       NULL };

static ColliderJntSphElementInit sLilyPadJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { BIO_DEKU_BABA_LILY_PAD_LIMB_NONE, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { BIO_DEKU_BABA_LILY_PAD_LIMB_NONE, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sLilyPadJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sLilyPadJntSphElementsInit),
    sLilyPadJntSphElementsInit,
};

static ColliderJntSphElementInit sHeadJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { BIO_DEKU_BABA_HEAD_LIMB_NONE, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sHeadJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sHeadJntSphElementsInit),
    sHeadJntSphElementsInit,
};

static ColliderJntSphElementInit sWalkingHeadJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7FFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { BIO_DEKU_BABA_HEAD_LIMB_NONE, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sWalkingHeadJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sWalkingHeadJntSphElementsInit),
    sWalkingHeadJntSphElementsInit,
};

void Boss05_WalkingHead_Thaw(Boss05* this, PlayState* play) {
    static Color_RGBA8 sIcePrimColor = { 170, 255, 255, 255 };
    static Color_RGBA8 sIceEnvColor = { 200, 200, 255, 255 };
    static Vec3f sIceAccel = { 0.0f, -1.0f, 0.0f };
    Vec3f pos;
    Vec3f velocity;
    s32 i;

    SoundSource_PlaySfxAtFixedWorldPos(play, &this->bodyPartsPos[BIO_BABA_BODYPART_HEAD], 30, NA_SE_EV_ICE_BROKEN);
    for (i = 0; i < 8; i++) {
        velocity.x = Rand_CenteredFloat(7.0f);
        velocity.z = Rand_CenteredFloat(7.0f);
        velocity.y = Rand_ZeroFloat(6.0f) + 4.0f;
        pos.x = this->bodyPartsPos[BIO_BABA_BODYPART_HEAD].x + velocity.x;
        pos.y = this->bodyPartsPos[BIO_BABA_BODYPART_HEAD].y + velocity.y;
        pos.z = this->bodyPartsPos[BIO_BABA_BODYPART_HEAD].z + velocity.z;
        EffectSsEnIce_Spawn(play, &pos, Rand_ZeroFloat(0.5f) + 0.7f, &velocity, &sIceAccel, &sIcePrimColor,
                            &sIceEnvColor, 30);
    }
}

void Boss05_SetColliderSphere(s32 index, ColliderJntSph* collider, Vec3f* sphereCenter) {
    collider->elements[index].dim.worldSphere.center.x = sphereCenter->x;
    collider->elements[index].dim.worldSphere.center.y = sphereCenter->y;
    collider->elements[index].dim.worldSphere.center.z = sphereCenter->z;
    collider->elements[index].dim.worldSphere.radius =
        collider->elements[index].dim.modelSphere.radius * collider->elements[index].dim.scale;
}

typedef enum BioDekuBabaDamageEffect {
    BIO_BABA_DMGEFF_IMMUNE,

    BIO_BABA_DMGEFF_STUN,

    BIO_BABA_DMGEFF_FIRE,

    BIO_BABA_DMGEFF_FREEZE,

    BIO_BABA_DMGEFF_LIGHT_ORB,

    BIO_BABA_DMGEFF_SWORD = 0xE,

    BIO_BABA_DMGEFF_DAMAGE
} BioDekuBabaDamageEffect;

static DamageTable sLilyPadWithHeadDamageTable = { {
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(3, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(3, BIO_BABA_DMGEFF_IMMUNE),    DMG_ENTRY(2, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_SWORD),  DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(1, BIO_BABA_DMGEFF_FIRE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_FREEZE), DMG_ENTRY(3, BIO_BABA_DMGEFF_LIGHT_ORB), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(3, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(2, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
} };

static DamageTable sWalkingHeadDamageTable = { {
    DMG_ENTRY(0, BIO_BABA_DMGEFF_STUN),   DMG_ENTRY(3, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(0, BIO_BABA_DMGEFF_STUN),      DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(2, BIO_BABA_DMGEFF_FIRE),
    DMG_ENTRY(2, BIO_BABA_DMGEFF_FREEZE), DMG_ENTRY(2, BIO_BABA_DMGEFF_LIGHT_ORB), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_STUN),   DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(2, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_STUN),   DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE), DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(1, BIO_BABA_DMGEFF_FIRE),   DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE),
    DMG_ENTRY(0, BIO_BABA_DMGEFF_IMMUNE), DMG_ENTRY(1, BIO_BABA_DMGEFF_DAMAGE),
} };

ActorProfile Boss_05_Profile = {
    0xd2d, ACTORCAT_ENEMY, FLAGS, 0x115e, sizeof(Boss05), Boss05_Init, Boss05_Destroy, Boss05_Update, Boss05_Draw,
};

void Boss05_Init(Actor* thisx, PlayState* play) {
    Boss05* this = (Boss05*)thisx;
    CollisionHeader* colHeader = NULL;

    this->dyna.actor.attentionRangeType = ATTENTION_RANGE_3;
    this->dyna.actor.colChkInfo.mass = MASS_HEAVY;
    this->dyna.actor.colChkInfo.health = 2;
    this->frameCounter = Rand_ZeroFloat(1000.0f);
    this->lowerJawScaleXZ = 1.0f;
    this->dyna.actor.gravity = -0.3f;

    Actor_SetScale(&this->dyna.actor, 0.01f);

    if ((BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_LILY_PAD_WITH_HEAD) ||
        (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_NO_LEAF_LILY_PAD_WITH_HEAD)) {
        if (BIO_BABA_FORCE_DETACH_TIMER(&this->dyna.actor) == 0) {
            BIO_BABA_FORCE_DETACH_TIMER(&this->dyna.actor) = 0;
        }

        this->dyna.actor.shape.rot.z = 0;
        this->forceDetachTimer = BIO_BABA_FORCE_DETACH_TIMER(&this->dyna.actor);
        this->dyna.actor.world.rot.z = this->dyna.actor.shape.rot.z;
        this->dyna.actor.colChkInfo.damageTable = &sLilyPadWithHeadDamageTable;

        DynaPolyActor_Init(&this->dyna, 0);
        CollisionHeader_GetVirtual(&sBioBabaLilypadCol, &colHeader);
        this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
        Boss05_LilyPadWithHead_SetupMove(this, play);

        SkelAnime_InitFlex(play, &this->lilyPadSkelAnime, &gBioDekuBabaLilyPadSkel, &gBioDekuBabaLilyPadIdleAnim,
                           this->lilyPadJointTable, this->lilyPadMorphTable, BIO_DEKU_BABA_LILY_PAD_LIMB_MAX);
        SkelAnime_InitFlex(play, &this->headSkelAnime, &gBioDekuBabaHeadSkel, &gBioDekuBabaHeadChompAnim,
                           this->headJointTable, this->headMorphTable, BIO_DEKU_BABA_HEAD_LIMB_MAX);
        this->animEndFrame = Animation_GetLastFrame(&gBioDekuBabaHeadChompAnim);

        Collider_InitAndSetJntSph(play, &this->lilyPadCollider, &this->dyna.actor, &sLilyPadJntSphInit,
                                  this->lilyPadColliderElements);
        Collider_InitAndSetJntSph(play, &this->headCollider, &this->dyna.actor, &sHeadJntSphInit,
                                  this->headColliderElements);

        if (Flags_GetClear(play, play->roomCtx.curRoom.num)) {
            this->dyna.actor.params = BIO_BABA_TYPE_LILY_PAD;
            this->actionFunc = Boss05_LilyPad_Idle;
            this->dyna.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            Actor_ChangeCategory(play, &play->actorCtx, &this->dyna.actor, ACTORCAT_BG);
        }
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_LILY_PAD) {
        this->actionFunc = Boss05_LilyPad_Idle;

        CollisionHeader_GetVirtual(&sBioBabaLilypadCol, &colHeader);
        this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);

        SkelAnime_InitFlex(play, &this->lilyPadSkelAnime, &gBioDekuBabaLilyPadSkel, &gBioDekuBabaLilyPadIdleAnim,
                           this->lilyPadJointTable, this->lilyPadMorphTable, BIO_DEKU_BABA_LILY_PAD_LIMB_MAX);
        this->dyna.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        Actor_ChangeCategory(play, &play->actorCtx, &this->dyna.actor, ACTORCAT_BG);
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_FALLING_HEAD) {
        this->actionFunc = Boss05_FallingHead_Fall;
        this->fallingHeadLilyPadLimbScale = 1.0f;

        SkelAnime_InitFlex(play, &this->lilyPadSkelAnime, &gBioDekuBabaLilyPadSkel, &gBioDekuBabaLilyPadIdleAnim,
                           this->lilyPadJointTable, this->lilyPadMorphTable, BIO_DEKU_BABA_LILY_PAD_LIMB_MAX);
        SkelAnime_InitFlex(play, &this->headSkelAnime, &gBioDekuBabaHeadSkel, &gBioDekuBabaHeadChompAnim,
                           this->headJointTable, this->headMorphTable, BIO_DEKU_BABA_HEAD_LIMB_MAX);
        this->animEndFrame = Animation_GetLastFrame(&gBioDekuBabaHeadChompAnim);

        Collider_InitAndSetJntSph(play, &this->lilyPadCollider, &this->dyna.actor, &sLilyPadJntSphInit,
                                  this->lilyPadColliderElements);
        Collider_InitAndSetJntSph(play, &this->headCollider, &this->dyna.actor, &sHeadJntSphInit,
                                  this->headColliderElements);

        ActorShape_Init(&this->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        this->dyna.actor.colChkInfo.damageTable = &sLilyPadWithHeadDamageTable;
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_WALKING_HEAD) {
        Boss05_WalkingHead_SetupTransform(this, play);
        this->dyna.actor.colChkInfo.mass = 90;

        SkelAnime_InitFlex(play, &this->headSkelAnime, &gBioDekuBabaHeadSkel, &gBioDekuBabaHeadChompAnim,
                           this->headJointTable, this->headMorphTable, BIO_DEKU_BABA_HEAD_LIMB_MAX);

        Collider_InitAndSetJntSph(play, &this->headCollider, &this->dyna.actor, &sWalkingHeadJntSphInit,
                                  this->headColliderElements);

        ActorShape_Init(&this->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        this->dyna.actor.colChkInfo.damageTable = &sWalkingHeadDamageTable;
        this->dyna.actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED;
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) >= BIO_BABA_TYPE_FRAGMENT_BASE) {
        SkelAnime_InitFlex(play, &this->headSkelAnime, &gBioDekuBabaHeadSkel, &gBioDekuBabaHeadChompAnim,
                           this->headJointTable, this->headMorphTable, BIO_DEKU_BABA_HEAD_LIMB_MAX);

        this->dyna.actor.gravity = 0.0f;
        this->dyna.actor.world.rot.y = Rand_ZeroFloat(0x8000);
        this->dyna.actor.speed = Rand_ZeroFloat(3.0f) + 3.0f;
        this->dyna.actor.velocity.y = Rand_ZeroFloat(1.5f) + 1.5f;

        this->fragmentAngularVelocity.x = Rand_CenteredFloat(700.0f);
        this->fragmentAngularVelocity.y = Rand_CenteredFloat(1500.0f);
        this->timers[TIMER_CURRENT_ACTION] = Rand_ZeroFloat(30.0f) + 50.0f;

        this->dyna.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        this->actionFunc = Boss05_Fragment_Move;
    }
}

void Boss05_Destroy(Actor* thisx, PlayState* play) {
    Boss05* this = (Boss05*)thisx;

    if ((BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_LILY_PAD) ||
        (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_LILY_PAD_WITH_HEAD) ||
        (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_NO_LEAF_LILY_PAD_WITH_HEAD)) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

s32 Boss05_LilyPadWithHead_UpdateDamage(Boss05* this, PlayState* play) {
    if (this->damagedFlashTimer == 0) {
        s32 i = 0;

        while (true) {
            if (this->lilyPadCollider.elements[i].base.acElemFlags & ACELEM_HIT) {
                switch (this->dyna.actor.colChkInfo.damageEffect) {
                    case BIO_BABA_DMGEFF_FIRE:
                        return BIO_BABA_HEAD_HIT_REACTION_DEATCH + BIO_BABA_DRAW_DMGEFF_STATE_FIRE_INIT;

                    case BIO_BABA_DMGEFF_FREEZE:
                        return BIO_BABA_HEAD_HIT_REACTION_DEATCH + BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_INIT;

                    case BIO_BABA_DMGEFF_LIGHT_ORB:
                        return BIO_BABA_HEAD_HIT_REACTION_DEATCH + BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT;

                    default:
                        return BIO_BABA_HEAD_HIT_REACTION_DEATCH + BIO_BABA_DRAW_DMGEFF_STATE_NONE;
                }
            }

            i++;
            if (i == BIO_BABA_LILY_PAD_COLLIDER_MAX) {
                if (this->headCollider.elements[BIO_BABA_HEAD_COLLIDER_HEAD].base.acElemFlags & ACELEM_HIT) {
                    u8 damage = this->dyna.actor.colChkInfo.damage;

                    this->dyna.actor.colChkInfo.health -= damage;
                    if ((s8)this->dyna.actor.colChkInfo.health <= 0) {
                        Enemy_StartFinishingBlow(play, &this->dyna.actor);
                        return BIO_BABA_HEAD_HIT_REACTION_DEAD;
                    }

                    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_DAMAGE);
                    this->damagedFlashTimer = 15;
                    this->lilyPadWithHeadAttackState = BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_IDLE;
                    this->timers[TIMER_CURRENT_ACTION] = 30;
                    return BIO_BABA_HEAD_HIT_REACTION_NONE_OR_DAMAGED;
                }

                break;
            }
        }
    }

    return BIO_BABA_HEAD_HIT_REACTION_NONE_OR_DAMAGED;
}

void Boss05_LilyPadWithHead_SetupMove(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_LilyPadWithHead_Move;
}

static Vec3s sWindUpLimbRot[BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_MAX] = {
    { 0x3200, 0, 0 }, { -0x1E00, 0, 0 }, { -0x1400, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
};

static Vec3s sLungeAttackLimbRot[BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_MAX] = {
    { -0x3200, 0, 0 }, { 0, 0, 0 }, { 0x1E00, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
};

void Boss05_LilyPadWithHead_Move(Boss05* this, PlayState* play) {
    s32 i;
    Player* player = GET_PLAYER(play);
    s32 j;
    u8 disableATCollisions = false;
    Vec3s targetLimbRot[BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_MAX];
    s32 frequencyX;
    s32 shiftX;
    s32 amplitudeX;
    s32 frequencyY;
    s32 shiftY;
    s32 amplitudeY;
    s32 frequencyZ;
    s32 shiftZ;
    s32 amplitudeZ;
    s32 limbRotMaxAngularVelocityFrac;
    s32 limbRotAngularVelocity;
    s16 rotAngularVelocity;
    s16 rotMaxAngularVelocityFrac;
    f32 diffY;
    f32 attackRange;
    Vec3f splashPos;
    s32 hitReaction;

    this->dyna.actor.hintId = TATL_HINT_ID_BIO_DEKU_BABA;

    if (this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_IDLE) {
        frequencyX = 0x3E8;
        shiftX = 0x3E80;
        amplitudeX = 0x7D0;

        frequencyY = 0x5DC;
        shiftY = 0x1770;
        amplitudeY = 0xBB8;

        frequencyZ = 0x514;
        shiftZ = 0x5208;
        amplitudeZ = 0xDAC;

        limbRotMaxAngularVelocityFrac = 0xA;
        limbRotAngularVelocity = 0x300;

        if (this->damagedFlashTimer != 0) {
            frequencyX = 0x1B58;
            amplitudeX = 0x1770;

            amplitudeY = 0x1770;

            frequencyZ = 0x1C84;
            amplitudeZ = 0x1964;

            limbRotMaxAngularVelocityFrac = 1;
            limbRotAngularVelocity = 0x1000;
        }
    } else if (this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_FLIP_ATTACK) {
        frequencyX = 0x1B58;
        shiftX = 0x3E80;
        amplitudeX = 0x1388;

        frequencyY = 0x5DC;
        shiftY = 0x2328;
        amplitudeY = 0x1388;

        frequencyZ = 0x1C84;
        shiftZ = 0x5208;
        amplitudeZ = 0x157C;

        limbRotMaxAngularVelocityFrac = 1;
        limbRotAngularVelocity = 0x1000;
    } else {
        frequencyX = shiftX = amplitudeX = frequencyY = shiftY = amplitudeY = frequencyZ = shiftZ = amplitudeZ =
            limbRotMaxAngularVelocityFrac = limbRotAngularVelocity = 0;
    }

    for (i = 0; i < BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_MAX; i++) {
        if ((this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_IDLE) ||
            (this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_FLIP_ATTACK)) {
            if (i <= BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_LOWER_STEM) {
                targetLimbRot[i].y = 0;
                targetLimbRot[i].x = Math_SinS((this->frameCounter * frequencyX) + (i * shiftX)) * amplitudeX;
            } else {
                targetLimbRot[i].x = 0;
                targetLimbRot[i].y = Math_SinS((this->frameCounter * frequencyY) + (i * shiftY)) * amplitudeY;
            }

            if ((i == BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_LEFT_LOWER_ARM) ||
                (i == BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_RIGHT_LOWER_ARM)) {
                targetLimbRot[i].z = Math_SinS((this->frameCounter * frequencyZ) + (i * shiftZ)) * amplitudeZ * 2.0f;
            } else {
                targetLimbRot[i].z = Math_SinS((this->frameCounter * frequencyZ) + (i * shiftZ)) * amplitudeZ;
            }

            rotMaxAngularVelocityFrac = limbRotMaxAngularVelocityFrac;
            rotAngularVelocity = limbRotAngularVelocity;
        } else {
            if (this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_WIND_UP) {
                limbRotMaxAngularVelocityFrac = 5;
                limbRotAngularVelocity = 0x1000;

                targetLimbRot[i].x = sWindUpLimbRot[i].x;
                targetLimbRot[i].y = sWindUpLimbRot[i].y;
                targetLimbRot[i].z = sWindUpLimbRot[i].z;
            } else if (this->lilyPadWithHeadMovementState == BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_LUNGE_ATTACK) {
                limbRotMaxAngularVelocityFrac = 1;
                limbRotAngularVelocity = 0x2000;

                targetLimbRot[i].x = sLungeAttackLimbRot[i].x;
                targetLimbRot[i].y = sLungeAttackLimbRot[i].y;
                targetLimbRot[i].z = sLungeAttackLimbRot[i].z;
            }

            rotMaxAngularVelocityFrac = limbRotMaxAngularVelocityFrac;
            rotAngularVelocity = limbRotAngularVelocity;
        }

        Math_ApproachS(&this->lilyPadWithHeadLimbRot[i].x, targetLimbRot[i].x, limbRotMaxAngularVelocityFrac,
                       limbRotAngularVelocity);
        Math_ApproachS(&this->lilyPadWithHeadLimbRot[i].y, targetLimbRot[i].y, limbRotMaxAngularVelocityFrac,
                       limbRotAngularVelocity);
        Math_ApproachS(&this->lilyPadWithHeadLimbRot[i].z, targetLimbRot[i].z, limbRotMaxAngularVelocityFrac,
                       limbRotAngularVelocity);
    }

    diffY = (player->actor.world.pos.y - this->dyna.actor.world.pos.y) + 10.0f;
    if (diffY < 0.0f) {
        Math_ApproachS(&this->dyna.actor.shape.rot.y, this->dyna.actor.yawTowardsPlayer, rotMaxAngularVelocityFrac,
                       rotAngularVelocity);
    }

    SkelAnime_Update(&this->headSkelAnime);
    Math_ApproachF(&this->lowerJawScaleXZ, 1.0f, 0.1f, 0.1f);

    if (this->lilyPadWithHeadAttackState != BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_FLIP_ATTACK) {
        Math_ApproachS(&this->dyna.actor.shape.rot.x, 0, 0x14, 0x800);
    }

    switch (this->lilyPadWithHeadAttackState) {
        case BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_IDLE:
            this->lilyPadWithHeadMovementState = BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_IDLE;
            this->headSkelAnime.playSpeed = 1.0f;

            if (diffY < 0.0f) {
                if (player->actor.speed > 10.0f) {
                    attackRange = 220.0f;
                } else {
                    attackRange = 150.0f;
                }

                if ((this->timers[TIMER_CURRENT_ACTION] == 0) &&
                    (sqrtf(this->dyna.actor.xyzDistToPlayerSq) <= attackRange)) {
                    this->lilyPadWithHeadAttackState = BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_WIND_UP;
                    this->timers[TIMER_CURRENT_ACTION] = 10;
                }

                this->flipAttackFrameCounter = 0;
            } else {
                disableATCollisions = true;

                if (sqrtf(this->dyna.actor.xyzDistToPlayerSq) <= 40.0f) {
                    this->flipAttackFrameCounter++;

                    if (this->flipAttackFrameCounter > 30) {
                        this->lilyPadWithHeadAttackState = 10;
                        this->timers[TIMER_CURRENT_ACTION] = 30;
                        Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_ATTACK);
                    }
                } else {
                    this->flipAttackFrameCounter = 0;
                }
            }

            if (((this->frameCounter % 4) == 0) && (Rand_ZeroOne() < 0.5f)) {
                Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_MOUTH);
            }
            break;

        case BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_FLIP_ATTACK:
            if ((this->frameCounter % 2) == 0) {
                Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_MOUTH);
            }

            this->lilyPadWithHeadStemRotX = (this->timers[TIMER_CURRENT_ACTION] & 1) * 0x200;
            this->lilyPadWithHeadMovementState = BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_FLIP_ATTACK;
            this->headSkelAnime.playSpeed = 4.0f;
            Math_ApproachS(&this->dyna.actor.shape.rot.x, -0x8000, 2, 0x2000);
            Math_ApproachS(&this->dyna.actor.shape.rot.y, this->dyna.actor.yawTowardsPlayer, 2, 0x2000);

            if (this->timers[TIMER_CURRENT_ACTION] == 0) {
                this->flipAttackFrameCounter = 0;
                this->lilyPadWithHeadAttackState = BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_IDLE;
                this->timers[TIMER_CURRENT_ACTION] = 100;
            }

            if (this->timers[TIMER_CURRENT_ACTION] == 27) {
                Math_Vec3f_Copy(&splashPos, &this->dyna.actor.world.pos);
                splashPos.y += 40.0f;
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 1, 2000);
                Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_OUT_OF_WATER);
            }
            break;

        case BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_WIND_UP:
            if (Animation_OnFrame(&this->headSkelAnime, this->animEndFrame)) {
                this->headSkelAnime.playSpeed = 0.0f;
            }

            this->lilyPadWithHeadMovementState = BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_WIND_UP;

            if (this->timers[TIMER_CURRENT_ACTION] == 0) {
                this->lilyPadWithHeadAttackState = BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_LUNGE_ATTACK;
                this->timers[TIMER_CURRENT_ACTION] = 20;
                Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_ATTACK);
            }
            break;

        case BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_LUNGE_ATTACK:
            if ((this->frameCounter % 2) == 0) {
                Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_MOUTH);
            }

            this->lilyPadWithHeadStemRotX = (this->timers[TIMER_CURRENT_ACTION] & 1) * 0x200;
            this->headSkelAnime.playSpeed = 4.0f;
            Math_ApproachF(&this->lowerJawScaleXZ, 1.5f, 1.0f, 0.7f);
            this->lilyPadWithHeadMovementState = BIO_BABA_LILY_PAD_WITH_HEAD_MOVEMENT_STATE_LUNGE_ATTACK;

            if (this->timers[TIMER_CURRENT_ACTION] == 0) {
                this->lilyPadWithHeadAttackState = BIO_BABA_LILY_PAD_WITH_HEAD_ATTACK_STATE_IDLE;
                this->timers[TIMER_CURRENT_ACTION] = 30;
            }
            break;

        default:
            break;
    }

    hitReaction = Boss05_LilyPadWithHead_UpdateDamage(this, play);
    if ((hitReaction != BIO_BABA_HEAD_HIT_REACTION_NONE_OR_DAMAGED) || (this->forceDetachTimer == 1)) {
        Boss05* child;

        this->dyna.actor.params = BIO_BABA_TYPE_LILY_PAD;
        this->actionFunc = Boss05_LilyPad_Idle;
        this->dyna.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        Actor_ChangeCategory(play, &play->actorCtx, &this->dyna.actor, ACTORCAT_BG);
        if (this->forceDetachTimer != 0) {
            hitReaction = BIO_BABA_HEAD_HIT_REACTION_DEATCH;
        }

        if (hitReaction >= BIO_BABA_HEAD_HIT_REACTION_DEATCH) {
            child = (Boss05*)Actor_SpawnAsChild(
                &play->actorCtx, &this->dyna.actor, play, ACTOR_BOSS_05, this->dyna.actor.world.pos.x,
                this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, this->dyna.actor.shape.rot.x,
                this->dyna.actor.shape.rot.y, this->dyna.actor.shape.rot.z, BIO_BABA_TYPE_FALLING_HEAD);

            if (child != NULL) {
                Player* player2 = GET_PLAYER(play);

                if (player2->focusActor == &this->dyna.actor) {
                    player2->focusActor = &child->dyna.actor;
                    play->actorCtx.targetCtx.arrowPointedActor = &child->dyna.actor;
                    play->actorCtx.targetCtx.targetedActor = &child->dyna.actor;
                }

                for (i = 0; i < BIO_DEKU_BABA_LILY_PAD_LIMB_MAX; i++) {
                    child->lilyPadSkelAnime.jointTable[i] = this->lilyPadSkelAnime.jointTable[i];
                }

                for (i = 0; i < BIO_DEKU_BABA_HEAD_LIMB_MAX; i++) {
                    child->headSkelAnime.jointTable[i] = this->headSkelAnime.jointTable[i];
                }

                Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_DAMAGE);

                if (hitReaction > BIO_BABA_HEAD_HIT_REACTION_DEATCH) {
                    child->drawDmgEffState = hitReaction - BIO_BABA_HEAD_HIT_REACTION_DEATCH;
                }
            }
        } else if (hitReaction == BIO_BABA_HEAD_HIT_REACTION_DEAD) {
            for (i = 0; i < 2; i++) {
                child = (Boss05*)Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_BOSS_05,
                                                    this->headPos.x, this->headPos.y, this->headPos.z, this->headRot.x,
                                                    this->headRot.y, this->headRot.z, BIO_BABA_TYPE_FRAGMENT_BASE + i);

                if (child != NULL) {
                    for (j = 0; j < BIO_DEKU_BABA_HEAD_LIMB_MAX; j++) {
                        child->headSkelAnime.jointTable[j] = this->headSkelAnime.jointTable[j];
                    }

                    child->timers[TIMER_CURRENT_ACTION] = Rand_ZeroFloat(20.0f) + 20.0f;
                }
            }

            Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA_DEAD);
        }
    }

    if (!disableATCollisions) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->lilyPadCollider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->headCollider.base);
    }

    CollisionCheck_SetAC(play, &play->colChkCtx, &this->lilyPadCollider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->headCollider.base);
}

void Boss05_LilyPad_Idle(Boss05* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 distToPlayer = sqrtf(this->dyna.actor.xyzDistToPlayerSq);
    s16 targetRotX;

    if ((distToPlayer < 50.0f) && (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        targetRotX = distToPlayer * 80.0f;
        targetRotX = CLAMP_MAX(targetRotX, 0x7D0);

        Math_ApproachS(&this->lilyPadRotX, targetRotX, 0x14, 0x7D0);
        Math_ApproachS(&this->lilyPadRotY, this->dyna.actor.yawTowardsPlayer, 0x14, 0xFA0);
        if ((this->frameCounter % 16) == 0) {
            EffectSsGRipple_Spawn(play, &this->dyna.actor.world.pos, 500, 1000, 0);
        }
    } else {
        Math_ApproachS(&this->lilyPadRotX, 0, 0x14, 0x100);
    }
}

void Boss05_FallingHead_Fall(Boss05* this, PlayState* play) {
    s32 i;
    Vec3f unusedPos;
    Vec3f bubblePos;
    Boss05* walkingHead;

    Actor_MoveWithGravity(&this->dyna.actor);
    this->dyna.actor.world.pos.y -= 85.0f;
    this->dyna.actor.prevPos.y -= 85.0f;
    Actor_UpdateBgCheckInfo(play, &this->dyna.actor, 20.0f, 50.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_40);
    this->dyna.actor.world.pos.y += 85.0f;
    this->dyna.actor.prevPos.y += 85.0f;

    if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_WATER) {
        this->dyna.actor.gravity = -0.3f;
        this->dyna.actor.minVelocityY = -5.0f;
    } else {
        this->dyna.actor.gravity = -2.0f;
        this->dyna.actor.minVelocityY = -20.0f;
    }

    if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
        this->dyna.actor.velocity.y = -3.0f;
        this->dyna.actor.gravity = -0.3f;
        this->dyna.actor.minVelocityY = -5.0f;
        Math_Vec3f_Copy(&unusedPos, &this->dyna.actor.world.pos);
        unusedPos.y += 20.0f;
        EffectSsGSplash_Spawn(play, &this->dyna.actor.world.pos, NULL, NULL, 1, 1000);
        this->timers[TIMER_FALLING_HEAD_FALL] = 20;
    }

    if (this->timers[TIMER_FALLING_HEAD_FALL] != 0) {
        for (i = 0; i < 3; i++) {
            bubblePos.x = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.x;
            bubblePos.y = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.y;
            bubblePos.z = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.z;
            EffectSsBubble_Spawn(play, &bubblePos, 20.0f, 10.0f, 20.0f, 0.13f);
        }
    }

    if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) && (this->drawDmgEffTimer != 0)) {
            this->drawDmgEffTimer = 0;
        }

        Math_ApproachZeroF(&this->fallingHeadLilyPadLimbScale, 1.0f, 0.05f);

        if (this->fallingHeadLilyPadLimbScale == 0.0f) {
            walkingHead = (Boss05*)Actor_SpawnAsChild(
                &play->actorCtx, &this->dyna.actor, play, ACTOR_BOSS_05, this->headPos.x, this->headPos.y,
                this->headPos.z, this->headRot.x, this->headRot.y, this->headRot.z, BIO_BABA_TYPE_WALKING_HEAD);

            if (walkingHead != NULL) {
                Player* player = GET_PLAYER(play);

                if (player->focusActor == &this->dyna.actor) {
                    player->focusActor = &walkingHead->dyna.actor;
                    play->actorCtx.targetCtx.arrowPointedActor = &walkingHead->dyna.actor;
                    play->actorCtx.targetCtx.targetedActor = &walkingHead->dyna.actor;
                }

                for (i = 0; i < BIO_DEKU_BABA_HEAD_LIMB_MAX; i++) {
                    walkingHead->headSkelAnime.jointTable[i] = this->headSkelAnime.jointTable[i];
                }

                Actor_PlaySfx(&walkingHead->dyna.actor, NA_SE_EN_MIZUBABA_TRANSFORM);
            }

            Actor_Kill(&this->dyna.actor);
        }
    }
}

void Boss05_WalkingHead_UpdateDamage(Boss05* this, PlayState* play) {
    u8 attackDealsDamage = false;
    ColliderElement* acHitElem;

    if ((this->damagedTimer == 0) &&
        (this->headCollider.elements[BIO_BABA_HEAD_COLLIDER_HEAD].base.acElemFlags & ACELEM_HIT)) {
        this->headCollider.elements[BIO_BABA_HEAD_COLLIDER_HEAD].base.acElemFlags &= ~ACELEM_HIT;
        acHitElem = this->headCollider.elements[BIO_BABA_HEAD_COLLIDER_HEAD].base.acHitElem;
        if (acHitElem->atDmgInfo.dmgFlags & 0x300000) {
            this->knockbackMagnitude = -12.0f;
            this->knockbackAngle = this->dyna.actor.yawTowardsPlayer;
            this->damagedTimer = 6;
            return;
        }

        this->damagedTimer = 10;
        this->dyna.actor.speed = 0.0f;
        this->knockbackMagnitude = -20.0f;
        this->knockbackAngle = this->dyna.actor.yawTowardsPlayer;

        Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_DAMAGE);

        switch (this->dyna.actor.colChkInfo.damageEffect) {
            case BIO_BABA_DMGEFF_STUN:
                Boss05_WalkingHead_SetupStunned(this, play);
                break;

            case BIO_BABA_DMGEFF_FIRE:
                this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_FIRE_INIT;
                attackDealsDamage = true;
                break;

            case BIO_BABA_DMGEFF_FREEZE:
                Boss05_WalkingHead_SetupFreeze(this, play);
                this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_INIT;
                break;

            case BIO_BABA_DMGEFF_LIGHT_ORB:
                this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT;
                attackDealsDamage = true;
                break;

            default:
                attackDealsDamage = true;
                break;
        }

        if (attackDealsDamage) {
            u8 damage;

            if ((this->actionFunc == Boss05_WalkingHead_Stunned) &&
                (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) && (this->drawDmgEffTimer != 0)) {
                Boss05_WalkingHead_Thaw(this, play);
                this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_NONE;
            }

            damage = this->dyna.actor.colChkInfo.damage;
            this->dyna.actor.colChkInfo.health -= damage;
            if ((s8)this->dyna.actor.colChkInfo.health <= 0) {
                Enemy_StartFinishingBlow(play, &this->dyna.actor);
            }

            Boss05_WalkingHead_SetupDamaged(this, play);
            this->damagedFlashTimer = 15;
        }
    }
}

s32 Boss05_WalkingHead_IsLookingAtPlayer(Boss05* this, PlayState* play) {
    s16 yawDiff = this->dyna.actor.yawTowardsPlayer - this->dyna.actor.shape.rot.y;

    if (ABS_ALT(yawDiff) < 0x3000) {
        return true;
    } else {
        return false;
    }
}

void Boss05_WalkingHead_TrySpottingPlayer(Boss05* this, PlayState* play) {
    if (Boss05_WalkingHead_IsLookingAtPlayer(this, play) && (this->dyna.actor.xyzDistToPlayerSq <= SQ(200.0f)) &&
        (fabsf(this->dyna.actor.yDistanceFromLink) < 70.0f)) {
        Boss05_WalkingHead_SetupSpottedPlayer(this, play);
    }
}

void Boss05_WalkingHead_SetupTransform(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Transform;
    Animation_MorphToPlayOnce(&this->headSkelAnime, &gBioDekuBabaHeadTransformAnim, -5.0f);
}

void Boss05_WalkingHead_Transform(Boss05* this, PlayState* play) {
    SkelAnime_Update(&this->headSkelAnime);
    Math_ApproachS(&this->dyna.actor.shape.rot.x, 0, 2, 0x400);
    Math_ApproachS(&this->dyna.actor.shape.rot.z, 0, 2, 0x400);
    Math_ApproachF(&this->headBodyScale, 1.0f, 1.0f, 0.5f);
    Math_ApproachF(&this->headLimbScale, 1.0f, 1.0f, 0.14f);

    if (this->headLimbScale == 1.0f) {
        Boss05_WalkingHead_SetupIdle(this, play);
    }
}

void Boss05_WalkingHead_SetupIdle(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Idle;
    Animation_MorphToLoop(&this->headSkelAnime, &gBioDekuBabaHeadIdleAnim, -10.0f);
    this->timers[TIMER_CURRENT_ACTION] = Rand_ZeroFloat(25.0f) + 25.0f;
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA1_MOUTH);
}

void Boss05_WalkingHead_Idle(Boss05* this, PlayState* play) {
    SkelAnime_Update(&this->headSkelAnime);
    Math_ApproachZeroF(&this->dyna.actor.speed, 1.0f, 2.5f);
    Math_ApproachS(&this->dyna.actor.shape.rot.x, 0, 2, 0x400);
    Math_ApproachS(&this->dyna.actor.shape.rot.z, 0, 2, 0x400);

    if (this->timers[TIMER_CURRENT_ACTION] == 0) {
        Boss05_WalkingHead_SetupWalk(this, play);
    }

    Boss05_WalkingHead_TrySpottingPlayer(this, play);
}

void Boss05_WalkingHead_SetupWalk(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Walk;
    Animation_MorphToLoop(&this->headSkelAnime, &gBioDekuBabaHeadWalkAnim, 0.0f);
    this->timers[TIMER_CURRENT_ACTION] = Rand_ZeroFloat(80.0f) + 60.0f;
    this->walkTargetPos.x = Rand_CenteredFloat(400.0f) + this->dyna.actor.world.pos.x;
    this->walkTargetPos.z = Rand_CenteredFloat(400.0f) + this->dyna.actor.world.pos.z;
    this->walkAngularVelocityY = 0.0f;
}

void Boss05_WalkingHead_Walk(Boss05* this, PlayState* play) {
    f32 diffX;
    f32 diffZ;

    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_WALK - SFX_FLAG);
    SkelAnime_Update(&this->headSkelAnime);
    Math_ApproachF(&this->dyna.actor.speed, 5.0f, 1.0f, 2.0f);
    diffX = this->walkTargetPos.x - this->dyna.actor.world.pos.x;
    diffZ = this->walkTargetPos.z - this->dyna.actor.world.pos.z;
    Math_ApproachS(&this->dyna.actor.world.rot.y, Math_Atan2S(diffX, diffZ), 5, this->walkAngularVelocityY);
    Math_ApproachF(&this->walkAngularVelocityY, 2000.0f, 1.0f, 100.0f);

    if ((this->timers[TIMER_CURRENT_ACTION] == 0) || ((SQ(diffX) + SQ(diffZ)) < SQ(50.0f))) {
        Boss05_WalkingHead_SetupIdle(this, play);
    }

    Boss05_WalkingHead_TrySpottingPlayer(this, play);
}

void Boss05_WalkingHead_SetupSpottedPlayer(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_SpottedPlayer;
    Animation_MorphToPlayOnce(&this->headSkelAnime, &gBioDekuBabaHeadSpotAnim, 0.0f);
    this->timers[TIMER_CURRENT_ACTION] = 20;
}

void Boss05_WalkingHead_SpottedPlayer(Boss05* this, PlayState* play) {
    SkelAnime_Update(&this->headSkelAnime);
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_VOICE - SFX_FLAG);
    Math_ApproachZeroF(&this->dyna.actor.speed, 1.0f, 2.5f);
    Math_ApproachS(&this->dyna.actor.world.rot.y, this->dyna.actor.yawTowardsPlayer, 5, 0x1000);

    if (this->timers[TIMER_CURRENT_ACTION] == 0) {
        Boss05_WalkingHead_SetupCharge(this, play);
    }
}

void Boss05_WalkingHead_SetupCharge(Boss05* this, PlayState* arg1) {
    this->actionFunc = Boss05_WalkingHead_Charge;
    Animation_MorphToLoop(&this->headSkelAnime, &gBioDekuBabaHeadChargeAnim, 0.0f);
    this->timers[TIMER_CURRENT_ACTION] = 60;
    this->walkAngularVelocityY = 0.0f;
}

void Boss05_WalkingHead_Charge(Boss05* this, PlayState* play) {
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_VOICE - SFX_FLAG);
    SkelAnime_Update(&this->headSkelAnime);
    Math_ApproachF(&this->dyna.actor.speed, 8.0f, 1.0f, 4.0f);
    Math_ApproachS(&this->dyna.actor.world.rot.y, this->dyna.actor.yawTowardsPlayer, 5, this->walkAngularVelocityY);
    Math_ApproachF(&this->walkAngularVelocityY, 4000.0f, 1.0f, 400.0f);

    if ((this->timers[TIMER_CURRENT_ACTION] == 0) || (this->dyna.actor.xyzDistToPlayerSq <= SQ(150.0f))) {
        Boss05_WalkingHead_SetupAttack(this, play);
    }
}

void Boss05_WalkingHead_SetupAttack(Boss05* this, PlayState* arg1) {
    this->actionFunc = Boss05_WalkingHead_Attack;
    Animation_MorphToPlayOnce(&this->headSkelAnime, &gBioDekuBabaHeadAttackAnim, 0.0f);
    this->animEndFrame = Animation_GetLastFrame(&gBioDekuBabaHeadAttackAnim);
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_ATTACK);
}

void Boss05_WalkingHead_Attack(Boss05* this, PlayState* play) {
    SkelAnime_Update(&this->headSkelAnime);

    if (Animation_OnFrame(&this->headSkelAnime, this->animEndFrame)) {
        Boss05_WalkingHead_SetupIdle(this, play);
    }
}

void Boss05_WalkingHead_SetupDamaged(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Damaged;
    Animation_MorphToPlayOnce(&this->headSkelAnime, &gBioDekuBabaHeadDamagedAnim, 0.0f);
    this->animEndFrame = Animation_GetLastFrame(&gBioDekuBabaHeadAttackAnim);
    Actor_SetColorFilter(&this->dyna.actor, COLORFILTER_COLORFLAG_RED, 120, COLORFILTER_BUFFLAG_OPA, 30);
}

void Boss05_WalkingHead_Damaged(Boss05* this, PlayState* play) {
    s32 i;
    s32 j;
    Vec3f bubblePos;
    Boss05* fragment;

    SkelAnime_Update(&this->headSkelAnime);

    if ((s8)this->dyna.actor.colChkInfo.health <= 0) {
        if (Animation_OnFrame(&this->headSkelAnime, 22.0f)) {
            for (i = 0; i < BIO_BABA_TYPE_MAX - BIO_BABA_TYPE_FRAGMENT_BASE; i++) {
                fragment = (Boss05*)Actor_SpawnAsChild(
                    &play->actorCtx, &this->dyna.actor, play, ACTOR_BOSS_05, this->dyna.actor.world.pos.x,
                    this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z, this->dyna.actor.shape.rot.x,
                    this->dyna.actor.shape.rot.y, this->dyna.actor.shape.rot.z, BIO_BABA_TYPE_FRAGMENT_BASE + i);

                if (fragment != NULL) {
                    for (j = 0; j < BIO_DEKU_BABA_HEAD_LIMB_MAX; j++) {
                        fragment->headSkelAnime.jointTable[j] = this->headSkelAnime.jointTable[j];
                    }
                }
            }

            for (i = 0; i < 20; i++) {
                bubblePos.x = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.x;
                bubblePos.y = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.y;
                bubblePos.z = Rand_CenteredFloat(40.0f) + this->dyna.actor.world.pos.z;
                EffectSsBubble_Spawn(play, &bubblePos, 20.0f, 10.0f, 20.0f, 0.13f);
            }

            SoundSource_PlaySfxAtFixedWorldPos(play, &this->dyna.actor.world.pos, 40, NA_SE_EN_MIZUBABA_DEAD);
            Actor_Kill(&this->dyna.actor);
            Item_DropCollectibleRandom(play, NULL, &this->dyna.actor.world.pos, 0xE0);
        }
    } else if (Animation_OnFrame(&this->headSkelAnime, this->animEndFrame)) {
        Boss05_WalkingHead_SetupSpottedPlayer(this, play);
    }
}

void Boss05_WalkingHead_SetupStunned(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Stunned;
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_DAMAGE);
    this->timers[TIMER_CURRENT_ACTION] = 40;
    Actor_SetColorFilter(&this->dyna.actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 40);
}

void Boss05_WalkingHead_SetupFreeze(Boss05* this, PlayState* play) {
    this->actionFunc = Boss05_WalkingHead_Stunned;
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_MIZUBABA2_DAMAGE);
    this->timers[TIMER_CURRENT_ACTION] = 80;
    Actor_SetColorFilter(&this->dyna.actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);
}

void Boss05_WalkingHead_Stunned(Boss05* this, PlayState* play) {
    Math_ApproachZeroF(&this->dyna.actor.speed, 1.0f, 2.5f);

    if (this->timers[TIMER_CURRENT_ACTION] == 0) {
        Boss05_WalkingHead_SetupIdle(this, play);
    }
}

void Boss05_Fragment_Move(Boss05* this, PlayState* play) {
    Actor_MoveWithGravity(&this->dyna.actor);

    if (this->fragmentState == BIO_BABA_FRAGMENT_STATE_SPAWNED) {
        Actor_UpdateBgCheckInfo(play, &this->dyna.actor, 20.0f, 50.0f, 40.0f,
                                UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_40);
        if (this->dyna.actor.bgCheckFlags & BGCHECKFLAG_WATER) {
            this->fragmentState = BIO_BABA_FRAGMENT_STATE_UNDERWATER;
        } else {
            this->fragmentState = BIO_BABA_FRAGMENT_STATE_ABOVE_WATER;
        }
    }

    if (this->fragmentState == BIO_BABA_FRAGMENT_STATE_UNDERWATER) {
        Math_ApproachF(&this->dyna.actor.velocity.y, 1.0f, 1.0f, 0.1f);
        Math_ApproachZeroF(&this->dyna.actor.speed, 0.5f, 0.5f);
        this->dyna.actor.shape.rot.x += this->fragmentAngularVelocity.x;
        this->dyna.actor.shape.rot.y += this->fragmentAngularVelocity.y;

        if (this->timers[TIMER_CURRENT_ACTION] == 0) {
            Actor_Kill(&this->dyna.actor);
        }
    } else {
        switch (this->fragmentState) {
            case BIO_BABA_FRAGMENT_STATE_ABOVE_WATER:
                this->dyna.actor.velocity.y = Rand_ZeroFloat(3.0f) + 3.0f;
                this->dyna.actor.speed = Rand_CenteredFloat(5.0f) + 5.0f;
                this->dyna.actor.world.rot.y = Rand_ZeroFloat(0x10000);
                this->dyna.actor.gravity = -1.0f;
                this->fragmentState = BIO_BABA_FRAGMENT_STATE_FLYING_THROUGH_AIR;
                break;

            case BIO_BABA_FRAGMENT_STATE_FLYING_THROUGH_AIR:
                Actor_MoveWithGravity(&this->dyna.actor);

                if (this->fragmentPos.y < (this->dyna.actor.floorHeight - 30.0f)) {
                    Actor_Kill(&this->dyna.actor);
                }
                break;

            default:
                break;
        }
    }
}

void Boss05_Update(Actor* thisx, PlayState* play) {
    Boss05* this = (Boss05*)thisx;
    s16 i;

    this->frameCounter++;

    for (i = 0; i < ARRAY_COUNT(this->timers); i++) {
        DECR(this->timers[i]);
    }

    DECR(this->damagedTimer);
    DECR(this->damagedFlashTimer);
    DECR(this->forceDetachTimer);
    DECR(this->drawDmgEffTimer);

    this->actionFunc(this, play);

    if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_WALKING_HEAD) {
        Actor_MoveWithGravity(&this->dyna.actor);
        Matrix_RotateYS(this->knockbackAngle, MTXMODE_NEW);
        Matrix_MultVecZ(this->knockbackMagnitude, &this->knockbackVelocity);
        this->dyna.actor.world.pos.x += this->knockbackVelocity.x;
        this->dyna.actor.world.pos.z += this->knockbackVelocity.z;
        Math_ApproachZeroF(&this->knockbackMagnitude, 1.0f, 1.0f);
        Actor_UpdateBgCheckInfo(play, &this->dyna.actor, 20.0f, 50.0f, 40.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_40);
        Boss05_WalkingHead_UpdateDamage(this, play);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->headCollider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->headCollider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->headCollider.base);
        this->dyna.actor.shape.rot.y = this->dyna.actor.world.rot.y;
    }

    switch (this->drawDmgEffState) {
        case BIO_BABA_DRAW_DMGEFF_STATE_NONE:
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
            this->drawDmgEffTimer = 0;
            this->drawDmgEffAlpha = 0.0f;
            break;

        case BIO_BABA_DRAW_DMGEFF_STATE_FIRE_INIT:
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
            this->drawDmgEffTimer = 80;
            this->drawDmgEffAlpha = 1.0f;
            this->drawDmgEffState++;
            this->drawDmgEffScale = 0.0f;
            FALLTHROUGH;
        case BIO_BABA_DRAW_DMGEFF_STATE_FIRE_ACTIVE:
            if (this->drawDmgEffTimer == 0) {
                Math_ApproachZeroF(&this->drawDmgEffAlpha, 1.0f, 0.02f);
                if (this->drawDmgEffAlpha == 0.0f) {
                    this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_NONE;
                }
            } else {
                Math_ApproachF(&this->drawDmgEffScale, 1.0f, 0.1f, 0.5f);
            }
            break;

        case BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_INIT:
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
            this->drawDmgEffTimer = 80;
            this->drawDmgEffAlpha = 1.0f;
            this->drawDmgEffState++;
            this->drawDmgEffScale = 0.0f;
            this->drawDmgEffFrozenSteamScale = 2.0f;
            FALLTHROUGH;
        case BIO_BABA_DRAW_DMGEFF_STATE_FROZEN_ACTIVE:
            if (this->drawDmgEffTimer == 0) {
                Boss05_WalkingHead_Thaw(this, play);
                this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_NONE;
            } else {
                Math_ApproachF(&this->drawDmgEffScale, 1.0f, 1.0f, 0.25f);
                Math_ApproachF(&this->drawDmgEffFrozenSteamScale, 1.0f, 0.1f, 0.1f);
            }
            break;

        case BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT:
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
            this->drawDmgEffTimer = 80;
            this->drawDmgEffAlpha = 1.0f;
            this->drawDmgEffState++;
            this->drawDmgEffScale = 0.0f;
            break;

        case BIO_BABA_DRAW_DMGEFF_STATE_LIGHT_ORB_ACTIVE:
            if (this->drawDmgEffTimer == 0) {
                Math_ApproachZeroF(&this->drawDmgEffScale, 1.0f, 0.03f);

                if (this->drawDmgEffScale == 0.0f) {
                    this->drawDmgEffState = BIO_BABA_DRAW_DMGEFF_STATE_NONE;
                    this->drawDmgEffAlpha = 0.0f;
                }
            } else {
                Math_ApproachF(&this->drawDmgEffScale, 1.0f, 0.5f, 0.5f);
            }
            break;

        default:
            break;
    }
}

static s8 sLimbIndexToLimbRotIndex[] = {
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_NONE,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_NONE,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_UPPER_STEM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_MIDDLE_STEM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_LOWER_STEM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_LEFT_UPPER_ARM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_LEFT_LOWER_ARM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_RIGHT_UPPER_ARM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_RIGHT_LOWER_ARM,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_NONE,
    BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_NONE,
};

s32 Boss05_LilyPadWithHead_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                            Actor* thisx) {
    Boss05* this = (Boss05*)thisx;
    s8 limbRotIndex;

    if (limbIndex == KREG(32)) {
        if ((this->frameCounter % 4) == 0) {
            *dList = NULL;
        }

        rot->x += KREG(33) * 0x100;
        rot->y += KREG(34) * 0x100;
        rot->z += KREG(35) * 0x100;
    }

    if ((BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_NO_LEAF_LILY_PAD_WITH_HEAD) &&
        (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_LEAF)) {
        *dList = NULL;
    }

    if (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_UPPER_STEM) {
        rot->x += this->lilyPadWithHeadStemRotX;
    }

    limbRotIndex = sLimbIndexToLimbRotIndex[limbIndex];
    if (limbRotIndex > BIO_BABA_LILY_PAD_WITH_HEAD_LIMB_ROT_INDEX_NONE) {
        rot->x = rot->x + this->lilyPadWithHeadLimbRot[limbRotIndex].x;
        rot->y = rot->y + this->lilyPadWithHeadLimbRot[limbRotIndex].y;
        rot->z = rot->z + this->lilyPadWithHeadLimbRot[limbRotIndex].z;
    }

    return false;
}

void Boss05_LilyPad_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Vec3f sHeadOffset = { 0.0f, -1400.0f, 600.0f };
    Boss05* this = (Boss05*)thisx;
    MtxF mf;
    Vec3f upperStemPos;
    Vec3f lowerStemPos;

    if (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_LOWER_STEM) {
        Matrix_MultVec3f(&sHeadOffset, &this->headPos);

        if (this->actionFunc == Boss05_FallingHead_Fall) {
            Matrix_MultVec3f(&sHeadOffset, &this->dyna.actor.focus.pos);
        }

        Matrix_Get(&mf);
        Matrix_MtxFToYXZRot(&mf, &this->headRot, false);
        this->headRot.x += 0xF00;
    }

    if (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_UPPER_STEM) {
        Matrix_MultZero(&upperStemPos);
        Boss05_SetColliderSphere(BIO_BABA_LILY_PAD_COLLIDER_UPPER_STEM, &this->lilyPadCollider, &upperStemPos);
    }

    if (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_MIDDLE_STEM) {
        Matrix_MultVecY(-500.0f, &lowerStemPos);
        Boss05_SetColliderSphere(BIO_BABA_LILY_PAD_COLLIDER_MIDDLE_STEM, &this->lilyPadCollider, &lowerStemPos);

        if (this->actionFunc == Boss05_LilyPadWithHead_Move) {
            Matrix_MultVecY(1500.0f, &this->dyna.actor.focus.pos);
        }
    }
}

s32 Boss05_Head_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    return false;
}

Vec3f sBioDekuBabaHeadColliderPos;

void Boss05_Head_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Vec3f sHeadColliderOffset = { 1600.0f, -300.0f, 0.0f };
    static Vec3f sHeadOffset = { 700.0f, 0.0f, 0.0f };
    Boss05* this = (Boss05*)thisx;

    if (limbIndex == BIO_DEKU_BABA_HEAD_LIMB_BODY) {
        Matrix_MultVec3f(&sHeadColliderOffset, &sBioDekuBabaHeadColliderPos);
        Boss05_SetColliderSphere(BIO_BABA_HEAD_COLLIDER_HEAD, &this->headCollider, &sBioDekuBabaHeadColliderPos);

        if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_WALKING_HEAD) {
            Matrix_MultVec3f(&sHeadOffset, &this->dyna.actor.focus.pos);
        }

        if (this->drawDmgEffState != BIO_BABA_DRAW_DMGEFF_STATE_NONE) {
            Matrix_MultVec3f(&sHeadOffset, &this->bodyPartsPos[BIO_BABA_BODYPART_HEAD]);
            if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FIRE) {
                this->bodyPartsPos[BIO_BABA_BODYPART_HEAD].y -= 15.0f;
            }
        }
    }
}

void Boss05_Head_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx) {
    Boss05* this = (Boss05*)thisx;

    if ((limbIndex == BIO_DEKU_BABA_HEAD_LIMB_LOWER_JAW) || (limbIndex == BIO_DEKU_BABA_HEAD_LIMB_UPPER_JAW)) {
        Matrix_Scale(this->lowerJawScaleXZ, 1.0f, this->lowerJawScaleXZ, MTXMODE_APPLY);
    }

    if (limbIndex == BIO_DEKU_BABA_HEAD_LIMB_BODY) {
        Matrix_Scale(this->headBodyScale, this->headBodyScale, this->headBodyScale, MTXMODE_APPLY);
    }

    if ((limbIndex >= BIO_DEKU_BABA_HEAD_LIMB_BACK_UPPER_LEG) && (limbIndex <= BIO_DEKU_BABA_HEAD_LIMB_LEAVES)) {
        Matrix_Scale(this->headLimbScale, this->headLimbScale, this->headLimbScale, MTXMODE_APPLY);
    }
}

s32 Boss05_LilyPad_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    if ((limbIndex >= BIO_DEKU_BABA_LILY_PAD_LIMB_MIDDLE_STEM) &&
        (limbIndex <= BIO_DEKU_BABA_LILY_PAD_LIMB_RIGHT_LOWER_ARM)) {
        *dList = NULL;
    }

    return false;
}

s32 Boss05_FallingHeadLilyPad_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                               Actor* thisx) {
    if ((limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_ROOTS) || (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_UPPER_STEM) ||
        (limbIndex == BIO_DEKU_BABA_LILY_PAD_LIMB_LEAF)) {
        *dList = NULL;
    }

    return false;
}

void Boss05_FallingHeadLilyPad_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx) {
    Boss05* this = (Boss05*)thisx;

    if ((limbIndex >= BIO_DEKU_BABA_LILY_PAD_LIMB_MIDDLE_STEM) &&
        (limbIndex <= BIO_DEKU_BABA_LILY_PAD_LIMB_RIGHT_LOWER_ARM)) {
        Matrix_Scale(this->fallingHeadLilyPadLimbScale, this->fallingHeadLilyPadLimbScale,
                     this->fallingHeadLilyPadLimbScale, MTXMODE_APPLY);
    }
}

static BioDekuBabaHeadLimb sFragmentIndexToLimbIndex[BIO_BABA_TYPE_MAX - BIO_BABA_TYPE_FRAGMENT_BASE] = {
    BIO_DEKU_BABA_HEAD_LIMB_LOWER_JAW,
    BIO_DEKU_BABA_HEAD_LIMB_UPPER_JAW,
    BIO_DEKU_BABA_HEAD_LIMB_BODY,
    BIO_DEKU_BABA_HEAD_LIMB_BACK_UPPER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_BACK_LOWER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_LEFT_UPPER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_LEFT_LOWER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_LEFT_LOWER_EYESTALK,
    BIO_DEKU_BABA_HEAD_LIMB_LEFT_UPPER_EYESTALK,
    BIO_DEKU_BABA_HEAD_LIMB_RIGHT_LOWER_EYESTALK,
    BIO_DEKU_BABA_HEAD_LIMB_RIGHT_UPPER_EYESTALK,
    BIO_DEKU_BABA_HEAD_LIMB_RIGHT_UPPER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_RIGHT_LOWER_LEG,
    BIO_DEKU_BABA_HEAD_LIMB_LEAVES,
};

s32 Boss05_Fragment_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                     Actor* thisx) {
    Boss05* this = (Boss05*)thisx;

    if (limbIndex != sFragmentIndexToLimbIndex[BIO_BABA_GET_FRAGMENT_INDEX(&this->dyna.actor)]) {
        *dList = NULL;
    } else if (this->fragmentState >= BIO_BABA_FRAGMENT_STATE_ABOVE_WATER) {
        rot->x += this->frameCounter * 0x3000;
        rot->y += this->frameCounter * 0x1A00;
        rot->z += this->frameCounter * 0x2000;
    }

    return false;
}

void Boss05_Fragment_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    Boss05* this = (Boss05*)thisx;

    if (limbIndex != sFragmentIndexToLimbIndex[BIO_BABA_GET_FRAGMENT_INDEX(&this->dyna.actor)]) {
        Matrix_MultZero(&this->fragmentPos);
    }
}

void Boss05_Draw(Actor* thisx, PlayState* play) {
    Boss05* this = (Boss05*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    if (this->actionFunc == Boss05_LilyPadWithHead_Move) {
        SkelAnime_DrawFlexOpa(play, this->lilyPadSkelAnime.skeleton, this->lilyPadSkelAnime.jointTable,
                              this->lilyPadSkelAnime.dListCount, Boss05_LilyPadWithHead_OverrideLimbDraw,
                              Boss05_LilyPad_PostLimbDraw, &this->dyna.actor);

        if ((this->damagedFlashTimer % 2) != 0) {
            POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 900, 1099);
        }

        Matrix_Translate(this->headPos.x, this->headPos.y, this->headPos.z, MTXMODE_NEW);
        Matrix_RotateYS(this->headRot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->headRot.x, MTXMODE_APPLY);
        Matrix_RotateZS(this->headRot.z, MTXMODE_APPLY);
        Matrix_Scale(this->dyna.actor.scale.x, this->dyna.actor.scale.y, this->dyna.actor.scale.z, MTXMODE_APPLY);

        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gBioDekuBabaHeadEyeFlashTexAnim));

        SkelAnime_DrawTransformFlexOpa(play, this->headSkelAnime.skeleton, this->headSkelAnime.jointTable,
                                       this->headSkelAnime.dListCount, Boss05_Head_OverrideLimbDraw,
                                       Boss05_Head_PostLimbDraw, Boss05_Head_TransformLimbDraw, &this->dyna.actor);
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_LILY_PAD) {
        Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
                         MTXMODE_NEW);
        Matrix_RotateYS(this->lilyPadRotY, MTXMODE_APPLY);
        Matrix_RotateXS(this->lilyPadRotX, MTXMODE_APPLY);
        Matrix_RotateYS(-this->lilyPadRotY, MTXMODE_APPLY);
        Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->dyna.actor.shape.rot.x, MTXMODE_APPLY);
        Matrix_RotateZS(this->dyna.actor.shape.rot.z, MTXMODE_APPLY);
        Matrix_Scale(this->dyna.actor.scale.x, this->dyna.actor.scale.y, this->dyna.actor.scale.z, MTXMODE_APPLY);

        SkelAnime_DrawFlexOpa(play, this->lilyPadSkelAnime.skeleton, this->lilyPadSkelAnime.jointTable,
                              this->lilyPadSkelAnime.dListCount, Boss05_LilyPad_OverrideLimbDraw, NULL,
                              &this->dyna.actor);
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_FALLING_HEAD) {
        SkelAnime_DrawTransformFlexOpa(play, this->lilyPadSkelAnime.skeleton, this->lilyPadSkelAnime.jointTable,
                                       this->lilyPadSkelAnime.dListCount, Boss05_FallingHeadLilyPad_OverrideLimbDraw,
                                       Boss05_LilyPad_PostLimbDraw, Boss05_FallingHeadLilyPad_TransformLimbDraw,
                                       &this->dyna.actor);

        Matrix_Translate(this->headPos.x, this->headPos.y, this->headPos.z, MTXMODE_NEW);
        Matrix_RotateYS(this->headRot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->headRot.x, MTXMODE_APPLY);
        Matrix_RotateZS(this->headRot.z, MTXMODE_APPLY);
        Matrix_Scale(this->dyna.actor.scale.x, this->dyna.actor.scale.y, this->dyna.actor.scale.z, MTXMODE_APPLY);

        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gBioDekuBabaHeadEyeFlashTexAnim));

        SkelAnime_DrawTransformFlexOpa(play, this->headSkelAnime.skeleton, this->headSkelAnime.jointTable,
                                       this->headSkelAnime.dListCount, Boss05_Head_OverrideLimbDraw,
                                       Boss05_Head_PostLimbDraw, Boss05_Head_TransformLimbDraw, &this->dyna.actor);

        Actor_DrawDamageEffects(play, &this->dyna.actor, this->bodyPartsPos, BIO_BABA_BODYPART_MAX,
                                this->drawDmgEffScale, this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha,
                                this->drawDmgEffType);
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) == BIO_BABA_TYPE_WALKING_HEAD) {
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gBioDekuBabaHeadEyeFlashTexAnim));

        if ((this->damagedFlashTimer % 2) != 0) {
            POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 900, 1099);
        }

        SkelAnime_DrawTransformFlexOpa(play, this->headSkelAnime.skeleton, this->headSkelAnime.jointTable,
                                       this->headSkelAnime.dListCount, Boss05_Head_OverrideLimbDraw,
                                       Boss05_Head_PostLimbDraw, Boss05_Head_TransformLimbDraw, &this->dyna.actor);

        Actor_DrawDamageEffects(play, &this->dyna.actor, this->bodyPartsPos, BIO_BABA_BODYPART_MAX,
                                this->drawDmgEffScale, this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha,
                                this->drawDmgEffType);
    } else if (BIO_BABA_GET_TYPE(&this->dyna.actor) >= BIO_BABA_TYPE_FRAGMENT_LOWER_JAW) {
        AnimatedMat_Draw(play, Lib_SegmentedToVirtual(gBioDekuBabaHeadEyeFlashTexAnim));

        SkelAnime_DrawFlexOpa(play, this->headSkelAnime.skeleton, this->headSkelAnime.jointTable,
                              this->headSkelAnime.dListCount, Boss05_Fragment_OverrideLimbDraw,
                              Boss05_Fragment_PostLimbDraw, &this->dyna.actor);
    }

    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x12d, Boss_05_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_BOSS_05, Boss_05_Profile, Boss05_Init, Boss05_Destroy, Boss05_Update,
                     Boss05_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x12du, 0x15eu, 0xeadbe8d6u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_BOSS_05_ORIGIN, generatedNativeOrigin);
