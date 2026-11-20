#include "mm_native_compat.h"
#include <combo/enemy_flight.h>
#define gDragonflyAttackAnim (*(AnimationHeader*)0x0600066c)
#define gDragonflyDeadAnim (*(AnimationHeader*)0x060008d4)
#define gDragonflyFallAnim (*(AnimationHeader*)0x060009e0)
#define gDragonflyFlyAnim (*(AnimationHeader*)0x06000f9c)
#define gDragonflySkel (*(SkeletonHeader*)0x06003f00)
#define gDragonflyRaiseTailAnim (*(AnimationHeader*)0x060042f0)
#define gDragonflyDamageAnim (*(AnimationHeader*)0x060047ac)
#define gDragonflyLowerTailAnim (*(AnimationHeader*)0x06004bc8)
#define gDragonflyHoverAnim (*(AnimationHeader*)0x0600524c)

#include "types/en_grasshopper.h"
#define TATL_HINT_ID_DRAGONFLY 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_GRASSHOPPER
#define ACTOR_EN_GRASSHOPPER 0x109
#undef OBJECT_GRASSHOPPER
#define OBJECT_GRASSHOPPER 0x114e
#undef NA_SE_EV_BOMB_DROP_WATER
#define NA_SE_EV_BOMB_DROP_WATER 0x2817
#undef NA_SE_EN_BATTA_FLY
#define NA_SE_EN_BATTA_FLY 0x38a0
#undef NA_SE_EN_BATTA_ATTACK
#define NA_SE_EN_BATTA_ATTACK 0x38a1
#undef NA_SE_EN_BATTA_DAMAGE
#define NA_SE_EN_BATTA_DAMAGE 0x38a2
#undef NA_SE_EN_BATTA_DEAD
#define NA_SE_EN_BATTA_DEAD 0x38a3
#undef NA_SE_EN_COMMON_EXTINCT_LEV
#define NA_SE_EN_COMMON_EXTINCT_LEV 0x3a1f
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Math_Atan2S
#define Math_Atan2S ((s16 (*)(f32 y, f32 x))(void*)FOREIGN_ENEMY_API_ATAN2S)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef SoundSource_PlaySfxEachFrameAtFixedWorldPos
#define SoundSource_PlaySfxEachFrameAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD_LOOP)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef SubS_GenShadowTex
extern u8 FOREIGN_ENEMY_API_SHADOW_GENERATE[];
#define SubS_GenShadowTex ((void (*)(Vec3f bodyPartsPos[], Vec3f* worldPos, u8* tex, f32 weight, u8 bodyPartsNum, u8 sizes[], s8 parentBodyParts[]))(void*)FOREIGN_ENEMY_API_SHADOW_GENERATE)
#undef SubS_DrawShadowTex
extern u8 FOREIGN_ENEMY_API_SHADOW_DRAW[];
#define SubS_DrawShadowTex ((void (*)(Actor* actor, struct GameState* gameState, u8* tex))(void*)FOREIGN_ENEMY_API_SHADOW_DRAW)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(struct PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c684u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef BgCheck_SphVsFirstPoly
#define BgCheck_SphVsFirstPoly ((s32 (*)(CollisionContext* colCtx, Vec3f* center, f32 radius))0x80030d08u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x800aa6ecu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x800aa724u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x8008993cu)
#undef SkinMatrix_SetTranslate
#define SkinMatrix_SetTranslate ((void (*)(MtxF* mf, f32 x, f32 y, f32 z))0x8008f82cu)
#undef SkinMatrix_SetScale
#define SkinMatrix_SetScale ((void (*)(MtxF* mf, f32 x, f32 y, f32 z))0x8008f4a8u)
#undef SkinMatrix_SetRotateRPY
#define SkinMatrix_SetRotateRPY ((void (*)(MtxF* mf, s16 roll, s16 pitch, s16 yaw))0x8008f50cu)
#undef SkinMatrix_MtxFMtxFMult
#define SkinMatrix_MtxFMtxFMult ((void (*)(MtxF* mfB, MtxF* mfA, MtxF* dest))0x8008edb8u)
#undef SkinMatrix_MtxFToNewMtx
#define SkinMatrix_MtxFToNewMtx ((Mtx* (*)(struct GraphicsContext* gfxCtx, MtxF* src))0x8008fc98u)
#undef Gfx_SetupDL61_Xlu
#define Gfx_SetupDL61_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e978u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnGrasshopper_Init(Actor* thisx, PlayState* play);
void EnGrasshopper_Destroy(Actor* thisx, PlayState* play);
void EnGrasshopper_Update(Actor* thisx, PlayState* play);
void EnGrasshopper_Draw(Actor* thisx, PlayState* play);

void EnGrasshopper_ChangeAnim(EnGrasshopper* this, s32 animIndex);
void EnGrasshopper_RaiseTail(EnGrasshopper* this);
void EnGrasshopper_LowerTail(EnGrasshopper* this);
void EnGrasshopper_DecideAction(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupFly(EnGrasshopper* this);
void EnGrasshopper_Fly(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_RoamInCircles(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupBank(EnGrasshopper* this);
void EnGrasshopper_Bank(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupBounced(EnGrasshopper* this);
void EnGrasshopper_Bounced(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupApproachPlayer(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_ApproachPlayer(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupAttack(EnGrasshopper* this);
void EnGrasshopper_Attack(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupWaitAfterAttack(EnGrasshopper* this);
void EnGrasshopper_WaitAfterAttack(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_Damaged(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_Dead(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_SetupFall(EnGrasshopper* this);
void EnGrasshopper_Fall(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_InitializeEffect(EnGrasshopper* this, Vec3f* pos);
void EnGrasshopper_UpdateEffects(EnGrasshopper* this, PlayState* play);
void EnGrasshopper_DrawEffects(EnGrasshopper* this, PlayState* play);

typedef enum {
    DRAGONFLY_ACTION_FLY,
    DRAGONFLY_ACTION_ROAM_IN_CIRCLES,
    DRAGONFLY_ACTION_BANK,
    DRAGONFLY_ACTION_BOUNCED,
    DRAGONFLY_ACTION_DECIDE_ACTION,
    DRAGONFLY_ACTION_APPROACH_PLAYER,
    DRAGONFLY_ACTION_ATTACK,
    DRAGONFLY_ACTION_WAIT_AFTER_ATTACK,
    DRAGONFLY_ACTION_DAMAGED,
    DRAGONFLY_ACTION_DEAD,
    DRAGONFLY_ACTION_FALL
} EnGrasshopperAction;

typedef enum {
    DRAGONFLY_DECISION_ATTACK,
    DRAGONFLY_DECISION_FLY,
    DRAGONFLY_DECISION_ROAM_IN_CIRCLES
} EnGrasshopperNextAction;

typedef enum { DRAGONFLY_BANK_STATE_BANKING, DRAGONFLY_BANK_STATE_DONE } EnGrasshopperBankState;

typedef enum {
    DRAGONFLY_ANIM_RAISE_TAIL,
    DRAGONFLY_ANIM_LOWER_TAIL,
    DRAGONFLY_ANIM_FLY,
    DRAGONFLY_ANIM_ATTACK,
    DRAGONFLY_ANIM_HOVER,
    DRAGONFLY_ANIM_DAMAGE,
    DRAGONFLY_ANIM_DEAD,
    DRAGONFLY_ANIM_FALL,
    DRAGONFLY_ANIM_MAX
} DragonflyAnimation;

static s32 sOccupiedIndices[] = {
    false, false, false, false, false,
};

static s8 sLimbToShadowBodyParts[DRAGONFLY_LIMB_MAX] = {
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_0,
    DRAGONFLY_SHADOW_BODYPART_1,
    DRAGONFLY_SHADOW_BODYPART_2,
    DRAGONFLY_SHADOW_BODYPART_3,
    DRAGONFLY_SHADOW_BODYPART_4,
    DRAGONFLY_SHADOW_BODYPART_5,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_6,
    DRAGONFLY_SHADOW_BODYPART_7,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_8,
    DRAGONFLY_SHADOW_BODYPART_9,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_10,
    DRAGONFLY_SHADOW_BODYPART_11,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_12,
    DRAGONFLY_SHADOW_BODYPART_13,
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
};

static s8 sParentShadowBodyParts[DRAGONFLY_LIMB_MAX] = {
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_0,
    DRAGONFLY_SHADOW_BODYPART_1,
    DRAGONFLY_SHADOW_BODYPART_2,
    DRAGONFLY_SHADOW_BODYPART_3,
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_6,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_8,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_10,
    BODYPART_NONE,
    BODYPART_NONE,
    DRAGONFLY_SHADOW_BODYPART_12,
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
};

static u8 sShadowSizes[DRAGONFLY_LIMB_MAX] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

typedef enum {
    DRAGONFLY_DMGEFF_IMMUNE,
    DRAGONFLY_DMGEFF_FIRE = 0x2,
    DRAGONFLY_DMGEFF_FREEZE,
    DRAGONFLY_DMGEFF_LIGHT_ORB,
    DRAGONFLY_DMGEFF_HOOK = 0xE,
    DRAGONFLY_DMGEFF_NONE
} EnDragonflyDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(0, DRAGONFLY_DMGEFF_HOOK),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(2, DRAGONFLY_DMGEFF_FIRE),
    DMG_ENTRY(2, DRAGONFLY_DMGEFF_FREEZE), DMG_ENTRY(2, DRAGONFLY_DMGEFF_LIGHT_ORB),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(2, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),
    DMG_ENTRY(1, DRAGONFLY_DMGEFF_NONE),   DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE),
    DMG_ENTRY(0, DRAGONFLY_DMGEFF_IMMUNE), DMG_ENTRY(1, DRAGONFLY_DMGEFF_HOOK),
} };

ActorProfile En_Grasshopper_Profile = {
    0xd09,
    ACTORCAT_ENEMY,
    FLAGS,
    0x114e,
    sizeof(EnGrasshopper),
    EnGrasshopper_Init,
    EnGrasshopper_Destroy,
    EnGrasshopper_Update,
    EnGrasshopper_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { DRAGONFLY_LIMB_THORAX, { { 0, 0, 0 }, 0 }, 0 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, HIT_SPECIAL_EFFECT_ELECTRIC, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { DRAGONFLY_LIMB_TAIL_TIP, { { 0, 0, 0 }, 0 }, 0 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT2,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_1,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

void EnGrasshopper_Init(Actor* thisx, PlayState* play) {
    EnGrasshopper* this = (EnGrasshopper*)thisx;
    s32 i;

    this->actor.hintId = TATL_HINT_ID_DRAGONFLY;
    this->actor.attentionRangeType = ATTENTION_RANGE_4;
    this->actor.colChkInfo.mass = 60;
    this->actor.colChkInfo.health = 2;

    Collider_InitAndSetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    this->collider.elements[0].dim.modelSphere.radius = 1;
    this->collider.elements[0].dim.scale = 22.0f;
    this->collider.elements[1].dim.modelSphere.radius = 1;
    this->collider.elements[1].dim.scale = 16.0f;
    this->collider.elements[1].dim.modelSphere.center.x = 1000;
    this->collider.elements[0].dim.modelSphere.center.x = this->collider.elements[0].dim.modelSphere.center.y =
        this->collider.elements[0].dim.modelSphere.center.z = this->collider.elements[1].dim.modelSphere.center.y =
            this->collider.elements[1].dim.modelSphere.center.z = 0;

    this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 1.0f);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    Math_Vec3f_Copy(&this->flyingHomePos, &this->actor.home.pos);
    this->action = DRAGONFLY_ACTION_FLY;
    this->index = -1;

    for (i = 0; i < ARRAY_COUNT(sOccupiedIndices); i++) {
        if (!sOccupiedIndices[i]) {
            this->index = i;
            sOccupiedIndices[i] = true;
            break;
        }
    }

    if (this->index < 0) {
        this->index = Rand_ZeroFloat(4.99f);
    }

    SkelAnime_Init(play, &this->skelAnime, &gDragonflySkel, &gDragonflyFlyAnim, this->jointTable, this->morphTable,
                   DRAGONFLY_LIMB_MAX);
    this->type = DRAGONFLY_GET_TYPE(&this->actor);
    if (this->type < DRAGONFLY_TYPE_UNUSED_NORMAL) {
        this->type = DRAGONFLY_TYPE_NORMAL;
    }

    if (this->type != DRAGONFLY_TYPE_GROWS_WHEN_SPAWNED) {
        this->dragonflyScale = 0.01f;
    } else {
        this->dragonflyScale = 0.0f;
        Actor_SetScale(&this->actor, 0.0f);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        Math_Vec3f_Copy(&this->flyingHomePos, &this->actor.world.pos);
        this->flyingHomePos.y += 90.0f;
        this->collider.elements[0].dim.modelSphere.radius = this->collider.elements[1].dim.modelSphere.radius =
            this->type;
    }

    this->baseFlyHeight = Rand_CenteredFloat(50.0f) + this->flyingHomePos.y;
    EnGrasshopper_SetupFly(this);
}

void EnGrasshopper_Destroy(Actor* thisx, PlayState* play) {
    EnGrasshopper* this = (EnGrasshopper*)thisx;

    Collider_DestroyJntSph(play, &this->collider);

    sOccupiedIndices[this->index] = false;
}

static AnimationHeader* sAnimations[DRAGONFLY_ANIM_MAX] = {
    &gDragonflyRaiseTailAnim, &gDragonflyLowerTailAnim, &gDragonflyFlyAnim,  &gDragonflyAttackAnim,
    &gDragonflyHoverAnim,     &gDragonflyDamageAnim,    &gDragonflyDeadAnim, &gDragonflyFallAnim,
};

static u8 sAnimationModes[DRAGONFLY_ANIM_MAX] = {
    ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE,
    ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE,
};

void EnGrasshopper_ChangeAnim(EnGrasshopper* this, s32 animIndex) {
    f32 morphFrames;

    this->animEndFrame = Animation_GetLastFrame(sAnimations[animIndex]);
    morphFrames = 0.0f;
    if ((animIndex == DRAGONFLY_ANIM_ATTACK) || (animIndex == DRAGONFLY_ANIM_HOVER) ||
        (animIndex == DRAGONFLY_ANIM_DAMAGE)) {
        morphFrames = -3.0f;
    }

    Animation_Change(&this->skelAnime, sAnimations[animIndex], 1.0f, 0.0f, this->animEndFrame,
                     sAnimationModes[animIndex], morphFrames);
}

void EnGrasshopper_RaiseTail(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_RAISE_TAIL);
    if (this->decision != DRAGONFLY_DECISION_ROAM_IN_CIRCLES) {
        this->decision = DRAGONFLY_DECISION_FLY;
    }

    this->action = DRAGONFLY_ACTION_DECIDE_ACTION;
    this->actionFunc = EnGrasshopper_DecideAction;
}

void EnGrasshopper_LowerTail(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_LOWER_TAIL);
    this->action = DRAGONFLY_ACTION_DECIDE_ACTION;
    this->decision = DRAGONFLY_DECISION_ATTACK;
    this->actionFunc = EnGrasshopper_DecideAction;
}

void EnGrasshopper_DecideAction(EnGrasshopper* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        if (this->decision == DRAGONFLY_DECISION_ATTACK) {
            EnGrasshopper_SetupAttack(this);
        } else {
            Math_Vec3f_Copy(&this->flyingHomePos, &this->actor.world.pos);
            this->flyingHomePos.y = this->actor.floorHeight + 90.0f;
            EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_FLY);
            if (this->decision != DRAGONFLY_DECISION_ROAM_IN_CIRCLES) {
                if (Player_GetMask(play) == PLAYER_MASK_STONE) {
                    EnGrasshopper_SetupFly(this);
                } else {
                    EnGrasshopper_SetupApproachPlayer(this, play);
                }
            } else {
                this->timer = 0;
                this->action = DRAGONFLY_ACTION_ROAM_IN_CIRCLES;
                this->decision = DRAGONFLY_DECISION_ATTACK;
                this->waitTimer = this->timer;
                this->actionFunc = EnGrasshopper_RoamInCircles;
            }
        }
    }
}

void EnGrasshopper_SetupFly(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_FLY);
    this->baseFlyHeight = Rand_CenteredFloat(50.0f) + this->flyingHomePos.y;
    this->action = DRAGONFLY_ACTION_FLY;
    this->actionFunc = EnGrasshopper_Fly;
}

void EnGrasshopper_Fly(EnGrasshopper* this, PlayState* play) {
    f32 diffX;
    f32 diffZ;
    f32 targetSpeed;
    Vec3f collisionCheckPos;

    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    diffX = this->flyingHomePos.x - this->actor.world.pos.x;
    diffZ = this->flyingHomePos.z - this->actor.world.pos.z;
    if ((this->type != DRAGONFLY_TYPE_WOODFALL_TEMPLE_FINAL_ROOM) && (this->type != DRAGONFLY_TYPE_WOODFALL)) {
        this->bobPhase += 0xAF0;
        this->targetPosY = (Math_SinS(this->bobPhase) * 10.0f) + this->baseFlyHeight;
        Math_ApproachF(&this->actor.world.pos.y, this->targetPosY, 0.1f, 10.0f);
        collisionCheckPos.x = (Math_SinS(this->actor.shape.rot.y) * 100.0f) + this->actor.world.pos.x;
        collisionCheckPos.y = this->actor.world.pos.y;
        collisionCheckPos.z = (Math_CosS(this->actor.shape.rot.y) * 100.0f) + this->actor.world.pos.z;

        if (this->collider.elements[0].base.ocElemFlags & OCELEM_HIT) {
            this->shouldTurn = true;
        }

        if (BgCheck_SphVsFirstPoly(&play->colCtx, &collisionCheckPos, 10.0f)) {
            this->shouldTurn = true;
        } else {
            this->shouldTurn = false;
        }

        if (this->type == DRAGONFLY_TYPE_WOODFALL) {
            if (sqrtf(SQ(this->actor.world.pos.x) + SQ(this->actor.world.pos.z)) < 600.0f) {
                this->shouldTurn = true;
            }
        }

        if (this->shouldTurn) {
            this->baseFlyHeight = Rand_CenteredFloat(50.0f) + this->flyingHomePos.y;
            this->targetRot.y = Math_Atan2S(diffX, diffZ);
            this->timer = Rand_S16Offset(30, 30);
        }
    }

    if ((Player_GetMask(play) != PLAYER_MASK_STONE) && !0 && !this->shouldTurn &&
        (this->actor.xzDistToPlayer < 200.0f)) {
        EnGrasshopper_SetupApproachPlayer(this, play);
    } else {
        Math_SmoothStepToS(&this->actor.world.rot.z, this->targetRot.z, 5, 0x3E8, 5);
        this->targetRot.z *= 0.8f;
        if (this->waitTimer != 0) {
            Math_ApproachZeroF(&this->actor.speed, 0.2f, 0.5f);
        } else {
            this->targetRot.z = (this->actor.world.rot.y - this->targetRot.y) * 0.2f;
            targetSpeed = (this->index * 0.1f) + 4.0f;
            Math_ApproachF(&this->actor.speed, targetSpeed, 0.4f, 0.7f);
            Math_ApproachF(&this->angularVelocity, 2000.0f, 1.0f, 50.0f);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRot.y, 5, this->angularVelocity, 5);
            if (this->timer == 0) {
                if (Rand_ZeroFloat(1.0f) < 0.3f) {
                    this->waitTimer = Rand_S16Offset(10, 10);
                    this->angularVelocity = 0.0f;
                }

                this->targetRot.y = Math_Atan2S(diffX, diffZ);
                this->timer = Rand_S16Offset(30, 70);
                this->baseFlyHeight = Rand_CenteredFloat(50.0f) + this->flyingHomePos.y;
            }
        }
    }
}

void EnGrasshopper_RoamInCircles(EnGrasshopper* this, PlayState* play) {
    f32 diffX;
    f32 diffZ;
    f32 targetSpeed;
    s16 rotationSpeed;
    Vec3f collisionCheckPos;
    Player* player = GET_PLAYER(play);

    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    if (Player_GetMask(play) == PLAYER_MASK_STONE) {
        EnGrasshopper_SetupFly(this);
    } else {
        diffX = player->actor.world.pos.x - this->actor.world.pos.x;
        diffZ = player->actor.world.pos.z - this->actor.world.pos.z;
        this->bobPhase += 0xAF0;
        this->targetPosY = player->actor.world.pos.y + 60.0f;
        this->baseFlyHeight = Math_SinS(this->bobPhase) * 10.0f;

        if (this->timer == 0) {
            this->baseFlyHeight = Rand_CenteredFloat(10.0f);
            this->timer = Rand_S16Offset(30, 30);
        }

        Math_ApproachF(&this->actor.world.pos.y, this->targetPosY, 0.1f, 10.0f);
        collisionCheckPos.x = (Math_SinS(this->actor.shape.rot.y) * 100.0f) + this->actor.world.pos.x;
        collisionCheckPos.y = this->actor.world.pos.y;
        collisionCheckPos.z = (Math_CosS(this->actor.shape.rot.y) * 100.0f) + this->actor.world.pos.z;

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
            BgCheck_SphVsFirstPoly(&play->colCtx, &collisionCheckPos, 10.0f)) {
            EnGrasshopper_SetupBank(this);
        } else if (player->stateFlags1 & PLAYER_STATE1_8000000) {
            this->collider.elements[0].base.atElemFlags |= (ATELEM_ON | ATELEM_SFX_WOOD);
            EnGrasshopper_RaiseTail(this);
        } else if (this->collider.base.atFlags & AT_BOUNCED) {
            this->collider.elements[0].base.atElemFlags &= ~(ATELEM_ON | ATELEM_SFX_WOOD);
            EnGrasshopper_SetupBounced(this);
        } else {
            this->targetRot.z = (this->actor.world.rot.y - this->targetRot.y) * 0.2f;
            Math_SmoothStepToS(&this->actor.world.rot.z, this->targetRot.z, 5, 0x3E8, 5);
            rotationSpeed = this->index + 70;
            targetSpeed = (this->index * 0.05f) + 4.0f;
            this->targetRot.y = Math_Atan2S(diffX, diffZ);
            Math_ApproachF(&this->actor.speed, targetSpeed, 0.4f, 0.8f);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRot.y, rotationSpeed, 0xFA0, 0xA);
        }
    }
}

void EnGrasshopper_SetupBank(EnGrasshopper* this) {
    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    this->targetBankRot.y = this->actor.world.rot.y + 0x8000;
    this->action = DRAGONFLY_ACTION_BANK;
    this->bankState = DRAGONFLY_BANK_STATE_BANKING;
    this->actor.speed = 2.0f;
    this->actionFunc = EnGrasshopper_Bank;
}

void EnGrasshopper_Bank(EnGrasshopper* this, PlayState* play) {
    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    switch (this->bankState) {
        case DRAGONFLY_BANK_STATE_BANKING:
            Math_SmoothStepToS(&this->actor.world.rot.y, this->targetBankRot.y, 0x64, 0x3E8, 0x3E8);
            Math_SmoothStepToS(&this->actor.world.rot.z, 0x4000, 0x64, 0x1F40, 0xBB8);
            if (fabsf(this->actor.world.rot.y - (f32)this->targetBankRot.y) < 10.0f) {
                this->postBankTimer = 100;
                this->bankState = DRAGONFLY_BANK_STATE_DONE;
            }
            break;

        case DRAGONFLY_BANK_STATE_DONE:
            Math_SmoothStepToS(&this->actor.world.rot.z, 0, 0x64, 0x1F40, 0xBB8);
            if (this->postBankTimer == 0) {
                this->timer = 0;
                this->action = DRAGONFLY_ACTION_ROAM_IN_CIRCLES;
                this->decision = DRAGONFLY_DECISION_ATTACK;
                this->waitTimer = this->timer;
                this->actionFunc = EnGrasshopper_RoamInCircles;
            }
            break;

        default:
            break;
    }
}

void EnGrasshopper_SetupBounced(EnGrasshopper* this) {
    this->targetRot.y = -this->actor.yawTowardsPlayer;
    this->timer = Rand_S16Offset(30, 30);
    this->targetRot.z = (this->actor.world.rot.y - this->targetRot.y) * 0.2f;
    this->action = DRAGONFLY_ACTION_BOUNCED;
    this->actionFunc = EnGrasshopper_Bounced;
}

void EnGrasshopper_Bounced(EnGrasshopper* this, PlayState* play) {
    f32 targetSpeed;

    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    targetSpeed = (this->index * 0.05f) + 7.0f;
    Math_ApproachF(&this->actor.speed, targetSpeed, 0.4f, 0.8f);
    Math_SmoothStepToS(&this->actor.world.rot.z, this->targetRot.z, 5, 0x3E8, 5);
    this->targetRot.z *= 0.8f;
    Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRot.y, 5, this->angularVelocity, 5);
    if (this->timer == 0) {
        this->collider.elements[0].base.atElemFlags |= (ATELEM_ON | ATELEM_SFX_WOOD);
        this->timer = 0;
        this->action = DRAGONFLY_ACTION_ROAM_IN_CIRCLES;
        this->waitTimer = this->timer;
        this->actionFunc = EnGrasshopper_RoamInCircles;
    }
}

void EnGrasshopper_SetupApproachPlayer(EnGrasshopper* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->timer = 50;
    this->action = DRAGONFLY_ACTION_APPROACH_PLAYER;
    this->approachSpeed = 0.0f;
    this->targetApproachPos.x = (Math_SinS(player->actor.shape.rot.y) * 130.0f) + player->actor.world.pos.x;
    this->targetApproachPos.z = (Math_CosS(player->actor.shape.rot.y) * 130.0f) + player->actor.world.pos.z;
    this->actionFunc = EnGrasshopper_ApproachPlayer;
}

void EnGrasshopper_ApproachPlayer(EnGrasshopper* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    WaterBox* waterBox;
    Vec3f splashPos;

    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_FLY - SFX_FLAG);
    this->bobPhase += 0xAF0;
    this->targetApproachPos.y = (Math_SinS(this->bobPhase) * 10.0f) + (player->actor.world.pos.y + 120.0f);

    Math_SmoothStepToS(&this->actor.world.rot.z, 0, 5, 0x3E8, 5);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.shape.rot.y, 0xA, 0xFA0, 0xA);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0xA);
    Math_ApproachF(&this->actor.world.pos.x, this->targetApproachPos.x, 0.3f, this->approachSpeed);
    Math_ApproachF(&this->actor.world.pos.y, this->targetApproachPos.y, 0.1f, this->approachSpeed);
    Math_ApproachF(&this->actor.world.pos.z, this->targetApproachPos.z, 0.3f, this->approachSpeed);
    Math_ApproachF(&this->approachSpeed, 7.0f, 0.3f, 1.0f);

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->tailTipPos.x, this->tailTipPos.z,
                                         &this->waterSurface, &waterBox)) {
        if (this->tailTipPos.y < this->waterSurface) {
            if ((this->splashCount < 3) || !(play->gameplayFrames % 8)) {
                this->splashCount++;
                Math_Vec3f_Copy(&splashPos, &this->tailTipPos);
                splashPos.x += Rand_CenteredFloat(20.0f);
                splashPos.z += Rand_CenteredFloat(20.0f);
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (((s32)Rand_ZeroOne() * 100) + 400));
                SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EV_BOMB_DROP_WATER);
            }
        } else {
            this->splashCount = 0;
        }
    }

    if ((this->timer == 0) || ((fabsf(this->targetApproachPos.x - this->actor.world.pos.x) <= 10.0f) &&
                               (fabsf(this->targetApproachPos.y - this->actor.world.pos.y) <= 20.0f) &&
                               (fabsf(this->targetApproachPos.z - this->actor.world.pos.z) <= 10.0f))) {
        EnGrasshopper_LowerTail(this);
    }
}

void EnGrasshopper_SetupAttack(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_ATTACK);
    this->approachSpeed = 0.0f;
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0xA);
    this->actor.speed = 3.0f;
    this->baseFlyHeight = this->actor.world.pos.y;
    this->collider.elements[0].base.atElemFlags &= ~(ATELEM_ON | ATELEM_SFX_WOOD);
    this->collider.elements[1].base.atElemFlags |= (ATELEM_ON | ATELEM_SFX_WOOD);
    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_ATTACK);
    this->action = DRAGONFLY_ACTION_ATTACK;
    this->actionFunc = EnGrasshopper_Attack;
}

void EnGrasshopper_Attack(EnGrasshopper* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;
    Vec3f hitPos;
    Vec3f diff;
    f32 playerToHitPosDist;
    s32 i;
    WaterBox* waterBox;
    Vec3f splashPos;

    for (i = 0; i < 6; i++) {
        EnGrasshopper_InitializeEffect(this, &this->tailTipPos);
    }

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->tailTipPos.x, this->tailTipPos.z,
                                         &this->waterSurface, &waterBox)) {
        if (this->tailTipPos.y < this->waterSurface) {
            if ((this->splashCount < 3) || !(play->gameplayFrames % 8)) {
                this->splashCount++;
                Math_Vec3f_Copy(&splashPos, &this->tailTipPos);
                splashPos.x += Rand_CenteredFloat(20.0f);
                splashPos.z += Rand_CenteredFloat(20.0f);
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, ((s32)Rand_ZeroOne() * 100) + 400);
                SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EV_BOMB_DROP_WATER);
            }
        } else {
            this->splashCount = 0;
        }
    }

    this->bobPhase += 0xAF0;
    this->targetApproachPos.y = (Math_SinS(this->bobPhase) * 10.0f) + (player->actor.world.pos.y + 60.0f);

    hitPos.x = this->collider.elements[1].base.acDmgInfo.hitPos.x;
    hitPos.y = this->collider.elements[1].base.acDmgInfo.hitPos.y;
    hitPos.z = this->collider.elements[1].base.acDmgInfo.hitPos.z;
    diff.x = hitPos.x - player->actor.world.pos.x;
    diff.y = hitPos.y - player->actor.world.pos.y;
    diff.z = hitPos.z - player->actor.world.pos.z;
    playerToHitPosDist = sqrtf(SQXYZ(diff));

    if ((this->collider.base.atFlags & AT_BOUNCED) ||
        ((player->stateFlags1 & PLAYER_STATE1_400000) && (playerToHitPosDist <= 60.0f) &&
         ((s16)((player->actor.shape.rot.y - this->actor.shape.rot.y) + 0x8000) < 0x2000) &&
         ((s16)((player->actor.shape.rot.y - this->actor.shape.rot.y) + 0x8000) > -0x2000))) {
        this->collider.elements[1].base.atElemFlags &= ~(ATELEM_ON | ATELEM_SFX_WOOD);
    }

    Math_ApproachF(&this->actor.world.pos.y, this->targetApproachPos.y, 0.1f, this->approachSpeed);
    Math_ApproachF(&this->approachSpeed, 10.0f, 0.1f, 1.0f);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0xA);
    if (curFrame >= this->animEndFrame) {
        EnGrasshopper_SetupWaitAfterAttack(this);
    }
}

void EnGrasshopper_SetupWaitAfterAttack(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_HOVER);
    this->bobPhase += 0xAF0;
    this->targetPosY = (Math_SinS(this->bobPhase) * 10.0f) + this->baseFlyHeight;
    Math_ApproachF(&this->actor.world.pos.y, this->targetPosY, 0.1f, 10.0f);
    this->action = DRAGONFLY_ACTION_WAIT_AFTER_ATTACK;
    this->waitTimer = 20;
    this->actor.speed = 0.0f;
    this->collider.elements[1].base.atElemFlags &= ~(ATELEM_ON | ATELEM_SFX_WOOD);
    this->actionFunc = EnGrasshopper_WaitAfterAttack;
}

void EnGrasshopper_WaitAfterAttack(EnGrasshopper* this, PlayState* play) {
    this->bobPhase += 0xAF0;
    this->targetPosY = (Math_SinS(this->bobPhase) * 10.0f) + this->baseFlyHeight;
    Math_ApproachF(&this->actor.world.pos.y, this->targetPosY, 0.1f, 10.0f);
    if (this->waitTimer == 0) {
        this->collider.elements[0].base.atElemFlags |= (ATELEM_ON | ATELEM_SFX_WOOD);
        EnGrasshopper_RaiseTail(this);
    }
}

void EnGrasshopper_SetupDamaged(EnGrasshopper* this, PlayState* play) {
    Vec3f damagedVelocity;

    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_DAMAGE);
    this->actor.speed = 0.0f;
    this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    this->approachSpeed = 0.0f;
    this->collider.elements[1].base.atElemFlags &= ~(ATELEM_ON | ATELEM_SFX_WOOD);
    Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-20.0f, &damagedVelocity);
    Math_Vec3f_Copy(&this->damagedVelocity, &damagedVelocity);
    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer != 0)) {
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, DRAGONFLY_BODYPART_MAX, 2, 0.3f, 0.2f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->action = DRAGONFLY_ACTION_DAMAGED;
    this->actionFunc = EnGrasshopper_Damaged;
}

void EnGrasshopper_Damaged(EnGrasshopper* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.world.rot.z, 0, 5, 0x3E8, 5);
    if (this->actor.colorFilterTimer == 0) {
        EnGrasshopper_RaiseTail(this);
    }
}

void EnGrasshopper_SetupDead(EnGrasshopper* this, PlayState* play) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_DEAD);
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.speed = 0.0f;
    this->approachSpeed = 0.0f;
    this->actor.velocity.y = 5.0f;
    this->actor.gravity = -0.5f;
    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer == 0)) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 25);
    Enemy_StartFinishingBlow(play, &this->actor);
    Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_DEAD);
    this->action = DRAGONFLY_ACTION_DEAD;
    this->actionFunc = EnGrasshopper_Dead;
}

void EnGrasshopper_Dead(EnGrasshopper* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->actor.world.rot.z, 0, 5, 0x3E8, 5);
    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer < 2)) {
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, DRAGONFLY_BODYPART_MAX, 2, 0.3f, 0.2f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    if (curFrame >= this->animEndFrame) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        EnGrasshopper_SetupFall(this);
    }
}

void EnGrasshopper_SetupFall(EnGrasshopper* this) {
    EnGrasshopper_ChangeAnim(this, DRAGONFLY_ANIM_FALL);
    this->action = DRAGONFLY_ACTION_FALL;
    this->actor.speed = 0.0f;
    this->approachSpeed = 0.0f;
    this->actionFunc = EnGrasshopper_Fall;
}

void EnGrasshopper_Fall(EnGrasshopper* this, PlayState* play) {
    WaterBox* waterBox;
    f32 waterSurface;
    s32 isUnderWater = false;

    this->actor.shape.rot.y += 0x1388;
    if ((this->actor.floorHeight <= BGCHECK_Y_MIN) || (this->actor.floorHeight >= BGCHECK_Y_MAX)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &waterSurface, &waterBox)) {
        if (this->actor.world.pos.y < waterSurface) {
            Vec3f splashPos;
            s32 i;

            for (i = 0; i < 3; i++) {
                Math_Vec3f_Copy(&splashPos, &this->actor.world.pos);
                splashPos.x += Rand_CenteredFloat((i * 5.0f) + 20.0f);
                splashPos.z += Rand_CenteredFloat((i * 5.0f) + 20.0f);
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (((s32)Rand_ZeroOne() * 100) + 400));
            }

            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EV_BOMB_DROP_WATER);
            isUnderWater = true;
        }
    }

    if ((BgCheck_SphVsFirstPoly(&play->colCtx, &this->tailTipPos, 40.0f)) &&
        (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX)) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer != 0)) {
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, DRAGONFLY_BODYPART_MAX, 2, 0.3f, 0.2f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    if (isUnderWater || (BgCheck_SphVsFirstPoly(&play->colCtx, &this->tailTipPos, 10.0f))) {
        Vec3f firePos;
        Vec3f sFireVelocityAndAccel[] = {
            { 1.0f, 0.0f, 0.5f },   { 1.0f, 0.0f, -0.5f },  { -1.0f, 0.0f, 0.5f },
            { -1.0f, 0.0f, -0.5f }, { 0.5f, 0.0f, 1.0f },   { -0.5f, 0.0f, 1.0f },
            { 0.5f, 0.0f, -1.0f },  { -0.5f, 0.0f, -1.0f }, { 0.0f, 0.0f, 0.0f },
        };
        s32 i;

        for (i = 0; i < ARRAY_COUNT(sFireVelocityAndAccel); i++) {
            Math_Vec3f_Copy(&firePos, &this->actor.world.pos);
            firePos.x += Rand_CenteredFloat(30.0f);
            if (!isUnderWater) {
                firePos.y = this->actor.floorHeight;
            } else if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x,
                                                        this->actor.world.pos.z, &waterSurface, &waterBox)) {
                firePos.y = waterSurface;
            }

            firePos.z += Rand_CenteredFloat(30.0f);
            func_800B3030(play, &firePos, &sFireVelocityAndAccel[i], &sFireVelocityAndAccel[i], 100, 0, 2);
        }

        SoundSource_PlaySfxEachFrameAtFixedWorldPos(play, &this->actor.world.pos, 10,
                                                    NA_SE_EN_COMMON_EXTINCT_LEV - SFX_FLAG);
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0x60);
        Actor_Kill(&this->actor);
    }
}

void EnGrasshopper_UpdateDamage(EnGrasshopper* this, PlayState* play) {
    s16 attackDealsDamage = false;

    if ((this->collider.elements[0].base.acElemFlags & ACELEM_HIT) ||
        (this->collider.elements[1].base.acElemFlags & ACELEM_HIT)) {
        this->collider.base.acFlags &= ~AC_HIT;
        if ((this->action != DRAGONFLY_ACTION_DAMAGED) && (this->action != DRAGONFLY_ACTION_DEAD) &&
            (this->action != DRAGONFLY_ACTION_FALL)) {
            if (this->actor.colChkInfo.damageEffect == DRAGONFLY_DMGEFF_NONE) {
                attackDealsDamage = true;
            } else if (this->actor.colChkInfo.damageEffect == DRAGONFLY_DMGEFF_FIRE) {
                this->drawDmgEffTimer = 40;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                attackDealsDamage = true;
            } else if (this->actor.colChkInfo.damageEffect == DRAGONFLY_DMGEFF_FREEZE) {
                if ((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_BLUE_FIRE) || (this->drawDmgEffTimer == 0)) {
                    Actor_ApplyDamage(&this->actor);
                    attackDealsDamage = false;
                    this->drawDmgEffTimer = 80;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
                    this->drawDmgEffScale = 0.0f;
                    this->drawDmgEffFrozenSteamScale = 1.5f;
                }

                if (this->actor.colChkInfo.health <= 0) {
                    EnGrasshopper_SetupDead(this, play);
                    return;
                }
            } else if (this->actor.colChkInfo.damageEffect == DRAGONFLY_DMGEFF_LIGHT_ORB) {
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, 255, COLORFILTER_BUFFLAG_OPA, 25);
                this->drawDmgEffTimer = 20;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x, this->actor.focus.pos.y,
                            this->actor.focus.pos.z, 0, 0, 0, CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                attackDealsDamage = true;
            }
        }
    }

    if (attackDealsDamage) {
        Actor_ApplyDamage(&this->actor);
        if (this->actor.colChkInfo.health > 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BATTA_DAMAGE);
            EnGrasshopper_SetupDamaged(this, play);
        } else {
            EnGrasshopper_SetupDead(this, play);
        }
    }
}

void EnGrasshopper_Update(Actor* thisx, PlayState* play) {
    EnGrasshopper* this = (EnGrasshopper*)thisx;
    Vec3f previous = this->actor.world.pos;

    SkelAnime_Update(&this->skelAnime);
    EnGrasshopper_UpdateDamage(this, play);
    this->actionFunc(this, play);

    Actor_SetFocus(&this->actor, 0.0f);
    Actor_SetScale(&this->actor, this->dragonflyScale);

    DECR(this->timer);
    DECR(this->waitTimer);
    DECR(this->postBankTimer);
    DECR(this->drawDmgEffTimer);

    Actor_MoveWithGravity(&this->actor);
    this->actor.world.pos.x += this->damagedVelocity.x;
    this->actor.world.pos.y += this->damagedVelocity.y;
    this->actor.world.pos.z += this->damagedVelocity.z;
    Math_ApproachZeroF(&this->damagedVelocity.x, 1.0f, 2.0f);
    Math_ApproachZeroF(&this->damagedVelocity.y, 1.0f, 2.0f);
    Math_ApproachZeroF(&this->damagedVelocity.z, 1.0f, 2.0f);
    if (this->action != DRAGONFLY_ACTION_FALL) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 60.0f, 60.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                    UPDBGCHECKINFO_FLAG_10);
        /* Woodfall's open-air shortcut is unsafe in shuffled rooms. Also
         * sweep the whole movement, including action/knockback offsets. */
        enemyFlightCheckWall(play, &this->actor, previous);
    }

    this->actor.shape.rot.z = this->actor.world.rot.z;
    if (this->type == DRAGONFLY_TYPE_GROWS_WHEN_SPAWNED) {
        f32 targetScale = this->type * 0.01f;

        if (targetScale >= 0.018f) {
            targetScale = 0.018f;
        }

        Math_ApproachF(&this->dragonflyScale, targetScale, 0.1f, 0.01f);
    }

    if ((this->action != DRAGONFLY_ACTION_FALL) && (this->action != DRAGONFLY_ACTION_APPROACH_PLAYER)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y, 5, 0x3E8, 5);
    }

    EnGrasshopper_UpdateEffects(this, play);
    if ((this->action != DRAGONFLY_ACTION_DEAD) && (this->action != DRAGONFLY_ACTION_FALL)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        if ((this->action == DRAGONFLY_ACTION_APPROACH_PLAYER) || (this->action == DRAGONFLY_ACTION_ATTACK)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void EnGrasshopper_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnGrasshopper* this = (EnGrasshopper*)thisx;
    Vec3f sEffectOffsetFromTailTop = { 500.0f, 0.0f, 0.0f };
    Vec3f sZeroVec3f = { 0.0f, 0.0f, 0.0f };

    Matrix_Push();

    if ((this->type != DRAGONFLY_TYPE_WOODFALL_TEMPLE_FINAL_ROOM) && (this->type != DRAGONFLY_TYPE_WOODFALL)) {
        if ((s8)(sLimbToShadowBodyParts[limbIndex] > BODYPART_NONE)) {
            Matrix_MultVec3f(&sZeroVec3f, &this->shadowBodyPartsPos[sLimbToShadowBodyParts[limbIndex]]);
        }
    }

    if (limbIndex == DRAGONFLY_LIMB_TAIL_TIP) {
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_MultVec3f(&gZeroVec3f, &this->tailTipPos);
        Matrix_MultVec3f(&sEffectOffsetFromTailTop, &this->effectBasePos);
    }

    Collider_UpdateSpheres(limbIndex, &this->collider);

    if ((limbIndex == DRAGONFLY_LIMB_ROOT) || (limbIndex == DRAGONFLY_LIMB_TAIL_SEGMENT_2) ||
        (limbIndex == DRAGONFLY_LIMB_TAIL_TIP) || (limbIndex == DRAGONFLY_LIMB_BACK_LEFT_UPPER_LEG) ||
        (limbIndex == DRAGONFLY_LIMB_BACK_RIGHT_UPPER_LEG) || (limbIndex == DRAGONFLY_LIMB_FRONT_LEFT_UPPER_LEG) ||
        (limbIndex == DRAGONFLY_LIMB_FRONT_LEFT_FOOT) || (limbIndex == DRAGONFLY_LIMB_FRONT_RIGHT_UPPER_LEG) ||
        (limbIndex == DRAGONFLY_LIMB_HEAD) || (limbIndex == 24) || (limbIndex == 25) ||
        (limbIndex == DRAGONFLY_LIMB_ROOT)) {
        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
        this->bodyPartIndex++;
        if (this->bodyPartIndex >= DRAGONFLY_BODYPART_MAX) {
            this->bodyPartIndex = 0;
        }
    }

    Matrix_Pop();
}

void EnGrasshopper_Draw(Actor* thisx, PlayState* play) {
    EnGrasshopper* this = (EnGrasshopper*)thisx;
    s32 i;
    u8* shadowTex = GRAPH_ALLOC(play->state.gfxCtx, SUBS_SHADOW_TEX_SIZE);
    u8* shadowTexIter;

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, EnGrasshopper_PostLimbDraw,
                      &this->actor);
    if ((this->type != DRAGONFLY_TYPE_WOODFALL_TEMPLE_FINAL_ROOM) && (this->type != DRAGONFLY_TYPE_WOODFALL)) {
        Matrix_RotateXS(0, MTXMODE_NEW);

        for (i = 0, shadowTexIter = shadowTex; i < SUBS_SHADOW_TEX_SIZE; i++) {
            *shadowTexIter = 0;
            shadowTexIter++;
        }

        for (i = 0; i < 5; i++) {
            SubS_GenShadowTex(this->shadowBodyPartsPos, &this->actor.world.pos, shadowTex, i / 5.0f,
                              DRAGONFLY_SHADOW_BODYPART_MAX, sShadowSizes, sParentShadowBodyParts);
            if (shadowTex && shadowTex && shadowTex) {
            }
        }

        SubS_DrawShadowTex(&this->actor, &play->state, shadowTex);
    }

    if (this->drawDmgEffTimer != 0) {
        f32 drawDmgEffAlpha = this->drawDmgEffTimer * 0.05f;

        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
            (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
            this->drawDmgEffScale += 0.3f;
            if (this->drawDmgEffScale > 0.5f) {
                this->drawDmgEffScale = 0.5f;
            }

            Math_ApproachF(&this->drawDmgEffFrozenSteamScale, this->drawDmgEffScale, 0.1f, 0.04f);
        } else {
            this->drawDmgEffScale = 0.8f;
            this->drawDmgEffFrozenSteamScale = 0.8f;
        }

        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, DRAGONFLY_BODYPART_MAX, this->drawDmgEffScale,
                                this->drawDmgEffFrozenSteamScale, drawDmgEffAlpha, this->drawDmgEffType);
    }

    EnGrasshopper_DrawEffects(this, play);
}

void EnGrasshopper_InitializeEffect(EnGrasshopper* this, Vec3f* pos) {
    EnGrasshopperEffect* effect = this->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isEnabled) {
            effect->isEnabled = true;
            effect->pos = *pos;
            effect->timer = 10;
            effect->velocity.x = Rand_CenteredFloat(20.0f);
            effect->velocity.y = Rand_CenteredFloat(20.0f);
            effect->velocity.z = Rand_CenteredFloat(20.0f);
            effect->yaw = Rand_CenteredFloat(30000.0f);
            return;
        }
    }
}

void EnGrasshopper_UpdateEffects(EnGrasshopper* this, PlayState* play) {
    EnGrasshopperEffect* effect = this->effects;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isEnabled) {
            effect->pos.x = this->effectBasePos.x + effect->velocity.x;
            effect->pos.y = this->effectBasePos.y + effect->velocity.y;
            effect->pos.z = this->effectBasePos.z + effect->velocity.z;
            if (effect->timer != 0) {
                effect->timer--;
                effect->lightningIndex++;
                Math_ApproachF(&effect->scale.x, 0.1f, 0.1f, 0.05f);
                if (effect->lightningIndex >= 8) {
                    effect->lightningIndex = 0;
                }
            } else {
                Math_ApproachZeroF(&effect->scale.x, 0.1f, 0.1f);
                if (effect->scale.x < 0.2f) {
                    effect->isEnabled = 0;
                }
            }

            effect->scale.y = effect->scale.z = effect->scale.x;
        }
    }
}

#define OOT_LIGHTNING_FIRST_FRAME 0x0402a3d0u
#define OOT_LIGHTNING_FRAME_BYTES 0x600u
#define OOT_LIGHTNING_DL 0x0402d3d0u

void EnGrasshopper_DrawEffects(EnGrasshopper* this, PlayState* play) {
    EnGrasshopperEffect* effect;
    s16 i;
    MtxF mfResult;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfRot;
    MtxF mfTransBillboard;
    MtxF mfTransBillboardRot;
    Mtx* mtx;

    OPEN_DISPS(play->state.gfxCtx);

    effect = this->effects;
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isEnabled) {
            u32 texture = OOT_LIGHTNING_FIRST_FRAME + (effect->lightningIndex & 7) * OOT_LIGHTNING_FRAME_BYTES;
            SkinMatrix_SetTranslate(&mfTrans, effect->pos.x, effect->pos.y, effect->pos.z);
            SkinMatrix_SetScale(&mfScale, effect->scale.x, effect->scale.y, effect->scale.z);
            SkinMatrix_SetRotateRPY(&mfRot, 0, 0, effect->yaw);
            SkinMatrix_MtxFMtxFMult(&mfTrans, &play->billboardMtxF, &mfTransBillboard);
            SkinMatrix_MtxFMtxFMult(&mfTransBillboard, &mfRot, &mfTransBillboardRot);
            SkinMatrix_MtxFMtxFMult(&mfTransBillboardRot, &mfScale, &mfResult);

            gSPMatrix(POLY_XLU_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            mtx = SkinMatrix_MtxFToNewMtx(play->state.gfxCtx, &mfResult);

            if (mtx != NULL) {
                gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                Gfx_SetupDL61_Xlu(play->state.gfxCtx);
                gSPSegment(POLY_XLU_DISP++, 0x08, (void*)(gSegments[4] + (texture & 0xffffffu)));
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(POLY_XLU_DISP++, 200, 255, 255, 255);
                gSPDisplayList(POLY_XLU_DISP++, (void*)(gSegments[4] + (OOT_LIGHTNING_DL & 0xffffffu)));
            }
        }
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x109, En_Grasshopper_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_GRASSHOPPER, En_Grasshopper_Profile, EnGrasshopper_Init,
                     EnGrasshopper_Destroy, EnGrasshopper_Update, EnGrasshopper_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x109u, 0x14eu, 0x64df7e9u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_GRASSHOPPER_ORIGIN, generatedNativeOrigin);
