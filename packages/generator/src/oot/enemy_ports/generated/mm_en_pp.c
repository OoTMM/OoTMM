#include "mm_native_compat.h"
#define gCircleShadowDL (*((Gfx (*)[1])0x04076bc0))
#define gHiploopAttackAnim (*(AnimationHeader*)0x06000e14)
#define gHiploopChargeAnim (*(AnimationHeader*)0x06001354)
#define gHiploopDamageAnim (*(AnimationHeader*)0x0600178c)
#define gHiploopRoarAnim (*(AnimationHeader*)0x06009a74)
#define gHiploopWindUpAnim (*(AnimationHeader*)0x0600a304)
#define gHiploopIdleAnim (*(AnimationHeader*)0x0600a844)
#define gHiploopSkel (*(FlexSkeletonHeader*)0x06012768)
#define gHiploopWalkAnim (*(AnimationHeader*)0x06012ae4)

#include "types/en_pp.h"
#define TATL_HINT_ID_HIPLOOP 0xff
#define TATL_HINT_ID_MASKED_HIPLOOP 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_PP
#define ACTOR_EN_PP 0x1e9
#undef OBJECT_PP
#define OBJECT_PP 0x11c6
#undef NA_SE_IT_SHIELD_BOUND
#define NA_SE_IT_SHIELD_BOUND 0x1806
#undef NA_SE_EV_BOMB_DROP_WATER
#define NA_SE_EV_BOMB_DROP_WATER 0x2817
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_HIPLOOP_RUN
#define NA_SE_EN_HIPLOOP_RUN 0x3979
#undef NA_SE_EN_HIPLOOP_PAUSE
#define NA_SE_EN_HIPLOOP_PAUSE 0x397a
#undef NA_SE_EN_HIPLOOP_MASC_OFF
#define NA_SE_EN_HIPLOOP_MASC_OFF 0x397b
#undef NA_SE_EN_HIPLOOP_FOOT
#define NA_SE_EN_HIPLOOP_FOOT 0x397c
#undef NA_SE_EN_HIPLOOP_DAMAGE
#define NA_SE_EN_HIPLOOP_DAMAGE 0x397d
#undef NA_SE_EN_HIPLOOP_DEAD
#define NA_SE_EN_HIPLOOP_DEAD 0x397e
#undef NA_SE_EN_HIPLOOP_FOOTSTEP
#define NA_SE_EN_HIPLOOP_FOOTSTEP 0x397f
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Collider_DestroyQuad
#define Collider_DestroyQuad ((s32 (*)(struct PlayState* play, ColliderQuad* quad))(void*)FOREIGN_ENEMY_API_QUAD_DESTROY)
#undef Effect_Destroy
#define Effect_Destroy ((void (*)(struct PlayState* play, s32 index))(void*)FOREIGN_ENEMY_API_HIPLOOP_EFFECT_DESTROY)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef BgCheck_SphVsFirstWall
extern u8 FOREIGN_ENEMY_API_SPH_FIRST_WALL[];
#define BgCheck_SphVsFirstWall ((s32 (*)(CollisionContext* colCtx, Vec3f* center, f32 radius))(void*)FOREIGN_ENEMY_API_SPH_FIRST_WALL)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef func_800C0094
#define func_800C0094 ((void (*)(CollisionPoly* poly, f32 tx, f32 ty, f32 tz, MtxF* dest))(void*)FOREIGN_ENEMY_API_POLY_SHADOW_MATRIX)
#undef Matrix_Mult
#define Matrix_Mult ((void (*)(MtxF* mf, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_MULT)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Collider_InitQuad
#define Collider_InitQuad ((s32 (*)(struct PlayState* play, ColliderQuad* quad))0x8004b858u)
#undef Collider_SetQuad
#define Collider_SetQuad ((s32 (*)(struct PlayState* play, ColliderQuad* dest, struct Actor* actor, ColliderQuadInit* src))0x8004b960u)
#undef Effect_Add
#define Effect_Add ((void (*)(struct PlayState* play, s32* pIndex, EffectType type, u8 arg3, u8 arg4, void* initParams))0x8001aa5cu)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef func_800B0EB0
#define func_800B0EB0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life))0x8001be64u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef BgCheck_SphVsFirstPoly
#define BgCheck_SphVsFirstPoly ((s32 (*)(CollisionContext* colCtx, Vec3f* center, f32 radius))0x80030d08u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x800aa6ecu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Effect_GetByIndex
#define Effect_GetByIndex ((void* (*)(s32 index))0x8001a890u)
#undef EffectBlure_AddSpace
#define EffectBlure_AddSpace ((void (*)(EffectBlure* this))0x80014254u)
#undef Collider_SetQuadVertices
#define Collider_SetQuadVertices ((void (*)(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d))0x80050b64u)
#undef EffectBlure_AddVertex
#define EffectBlure_AddVertex ((void (*)(EffectBlure* this, Vec3f* p1, Vec3f* p2))0x80013f30u)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x800aa724u)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef Gfx_SetupDL44_Xlu
#define Gfx_SetupDL44_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e404u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnPp_Init(Actor* thisx, PlayState* play);
void EnPp_Destroy(Actor* thisx, PlayState* play);
void EnPp_Update(Actor* thisx, PlayState* play);
void EnPp_Draw(Actor* thisx, PlayState* play);

void EnPp_SetupIdle(EnPp* this);
void EnPp_Idle(EnPp* this, PlayState* play);
void EnPp_SetupCharge(EnPp* this);
void EnPp_Charge(EnPp* this, PlayState* play);
void EnPp_SetupAttack(EnPp* this);
void EnPp_Attack(EnPp* this, PlayState* play);
void EnPp_SetupBounced(EnPp* this);
void EnPp_Bounced(EnPp* this, PlayState* play);
void EnPp_SetupRoar(EnPp* this);
void EnPp_Roar(EnPp* this, PlayState* play);
void EnPp_SetupJump(EnPp* this);
void EnPp_Jump(EnPp* this, PlayState* play);
void EnPp_StunnedOrFrozen(EnPp* this, PlayState* play);
void EnPp_Damaged(EnPp* this, PlayState* play);
void EnPp_SetupDead(EnPp* this, PlayState* play);
void EnPp_Dead(EnPp* this, PlayState* play);
void EnPp_Mask_Detach(EnPp* this, PlayState* play);
void EnPp_Fragment_SetupMove(EnPp* this);
void EnPp_Fragment_Move(EnPp* this, PlayState* play);

typedef enum {
    EN_PP_COLLISION_RESULT_OK,
    EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_INTO_WALL,
    EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_OFF_LEDGE
} EnPpCollisionResult;

typedef enum {
    EN_PP_ACTION_IDLE,
    EN_PP_ACTION_CHARGE,
    EN_PP_ACTION_ATTACK,
    EN_PP_ACTION_BOUNCED,
    EN_PP_ACTION_ROAR,
    EN_PP_ACTION_JUMP,
    EN_PP_ACTION_STUNNED_OR_FROZEN,
    EN_PP_ACTION_MASK_DETACH,
    EN_PP_ACTION_DAMAGED,
    EN_PP_ACTION_DEAD,
    EN_PP_ACTION_MASK_DEAD,
    EN_PP_ACTION_SPAWN_BODY_PARTS,
    EN_PP_ACTION_DONE_SPAWNING_BODY_PARTS,
    EN_PP_ACTION_BODY_PART_MOVE
} EnPpAction;

typedef enum {
    EN_PP_MASK_DETACH_STATE_START,
    EN_PP_MASK_DETACH_STATE_FALL,
    EN_PP_MASK_DETACH_STATE_DIE
} EnPpMaskDetachState;

static s32 sCurrentFragmentIndex = 0;

typedef enum {
    EN_PP_DMGEFF_JUMP,
    EN_PP_DMGEFF_STUN,
    EN_PP_DMGEFF_FIRE,
    EN_PP_DMGEFF_FREEZE,
    EN_PP_DMGEFF_LIGHT_ORB,
    EN_PP_DMGEFF_ELECTRIC_STUN,
    EN_PP_DMGEFF_KNOCK_OFF_MASK = 0xC,
    EN_PP_DMGEFF_HOOKSHOT,
    EN_PP_DMGEFF_GORON_POUND,
    EN_PP_DMGEFF_DAMAGE
} EnPpDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_PP_DMGEFF_STUN),           DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(1, EN_PP_DMGEFF_KNOCK_OFF_MASK),
    DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),         DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(1, EN_PP_DMGEFF_HOOKSHOT),
    DMG_ENTRY(1, EN_PP_DMGEFF_KNOCK_OFF_MASK), DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),
    DMG_ENTRY(0, EN_PP_DMGEFF_GORON_POUND),    DMG_ENTRY(1, EN_PP_DMGEFF_FIRE),
    DMG_ENTRY(1, EN_PP_DMGEFF_FREEZE),         DMG_ENTRY(2, EN_PP_DMGEFF_LIGHT_ORB),
    DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),         DMG_ENTRY(0, EN_PP_DMGEFF_STUN),
    DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),         DMG_ENTRY(2, EN_PP_DMGEFF_KNOCK_OFF_MASK),
    DMG_ENTRY(0, EN_PP_DMGEFF_STUN),           DMG_ENTRY(0, EN_PP_DMGEFF_ELECTRIC_STUN),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),
    DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),         DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),
    DMG_ENTRY(1, EN_PP_DMGEFF_DAMAGE),         DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),
    DMG_ENTRY(0, EN_PP_DMGEFF_JUMP),           DMG_ENTRY(1, EN_PP_DMGEFF_KNOCK_OFF_MASK),
} };

ActorProfile En_Pp_Profile = {
    0xde9, ACTORCAT_ENEMY, FLAGS, 0x11c6, sizeof(EnPp), EnPp_Init, EnPp_Destroy, EnPp_Update, EnPp_Draw,
};

static ColliderJntSphElementInit sMaskColliderJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sMaskColliderJntSphInit = {
    {
        COL_MATERIAL_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sMaskColliderJntSphElementsInit),
    sMaskColliderJntSphElementsInit,
};

static ColliderJntSphElementInit sBodyColliderJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sBodyColliderJntSphInit = {
    {
        COL_MATERIAL_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sBodyColliderJntSphElementsInit),
    sBodyColliderJntSphElementsInit,
};

static ColliderQuadInit sQuadInit = {
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
        { 0xF7CFFFFF, 0x04, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static Color_RGBA8 sDustPrimColor = { 60, 50, 20, 255 };

static Color_RGBA8 sDustEnvColor = { 40, 30, 30, 255 };

void EnPp_Init(Actor* thisx, PlayState* play) {
    EnPp* this = (EnPp*)thisx;
    EffectBlureInit1 blureInit;

    this->actor.attentionRangeType = ATTENTION_RANGE_4;
    this->actor.colChkInfo.mass = 60;
    this->actor.colChkInfo.health = 3;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    SkelAnime_InitFlex(play, &this->skelAnime, &gHiploopSkel, &gHiploopIdleAnim, this->jointTable, this->morphTable,
                       HIPLOOP_LIMB_MAX);

    if (EN_PP_GET_ATTACK_RANGE(&this->actor) == 0) {
        this->attackRange = 200.0f;
    } else {
        this->attackRange = EN_PP_GET_ATTACK_RANGE(&this->actor) * 40.0f;
    }

    this->chargesInStraightLines = EN_PP_DOES_CHARGE_IN_STRAIGHT_LINES(&this->actor);
    if (this->chargesInStraightLines) {
        this->actor.params &= 1;
        this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    }

    if (this->actor.params < 0) {
        this->actor.params = EN_PP_TYPE_MASKED;
    }

    if (EN_PP_GET_TYPE(&this->actor) >= EN_PP_TYPE_FRAGMENT_BASE) {
        this->fragmentIndex = sCurrentFragmentIndex;
        sCurrentFragmentIndex++;
        this->actor.shape.rot.y = this->actor.world.rot.y;
        Actor_SetScale(&this->actor, 0.03f);
        EnPp_Fragment_SetupMove(this);
    } else {
        Collider_InitAndSetJntSph(play, &this->maskCollider, &this->actor, &sMaskColliderJntSphInit,
                                  this->maskColliderElements);
        Collider_InitAndSetJntSph(play, &this->bodyCollider, &this->actor, &sBodyColliderJntSphInit,
                                  this->bodyColliderElements);

        this->bodyCollider.elements[0].dim.scale = 1.0f;
        if (EN_PP_GET_TYPE(&this->actor) > EN_PP_TYPE_MASKED) {
            this->actor.hintId = TATL_HINT_ID_HIPLOOP;
            this->maskColliderElements[0].base.atElemFlags &= ~ATELEM_ON;
            this->maskColliderElements[0].base.acElemFlags &= ~ACELEM_ON;
            this->maskColliderElements[0].base.ocElemFlags &= ~OCELEM_ON;
            this->maskCollider.base.colMaterial = COL_MATERIAL_HIT2;
            this->maskCollider.elements[0].dim.modelSphere.radius = 42;
            this->maskCollider.elements[0].dim.scale = 1.0f;
            this->maskCollider.elements[0].dim.modelSphere.center.x = 400;
            this->maskCollider.elements[0].dim.modelSphere.center.y = -400;
            this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
            this->bodyCollider.elements[0].dim.scale = 1.0f;
            this->bodyCollider.elements[0].dim.modelSphere.center.x = 400;
            this->bodyCollider.elements[0].dim.modelSphere.center.y = -400;
        } else {
            this->actor.hintId = TATL_HINT_ID_MASKED_HIPLOOP;
            this->maskCollider.elements[0].dim.modelSphere.radius = 10;
            this->maskCollider.elements[0].dim.scale = 1.0f;
            this->maskCollider.elements[0].dim.modelSphere.center.x = 1000;
            this->maskCollider.elements[0].dim.modelSphere.center.y = -500;
            this->bodyCollider.elements[0].dim.modelSphere.radius = 25;
            this->bodyCollider.elements[0].dim.scale = 1.0f;
            this->bodyCollider.elements[0].dim.modelSphere.center.x = 400;
            this->bodyCollider.elements[0].dim.modelSphere.center.y = -400;
            this->bodyColliderElements[0].base.acElemFlags |= ACELEM_HOOKABLE;
            this->maskCollider.elements[0].base.atDmgInfo.damage = 0x10;
        }

        Collider_InitQuad(play, &this->hornCollider);
        Collider_SetQuad(play, &this->hornCollider, &this->actor, &sQuadInit);

        blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] = blureInit.p1StartColor[3] =
            blureInit.p2StartColor[0] = blureInit.p2StartColor[1] = blureInit.p2StartColor[2] =
                blureInit.p1EndColor[0] = blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] =
                    blureInit.p2EndColor[1] = blureInit.p2EndColor[2] = 255;
        blureInit.p2StartColor[3] = 64;
        blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
        blureInit.elemDuration = 8;
        blureInit.unkFlag = false;
        blureInit.calcMode = 2;
        Effect_Add(play, &this->blureIndex, EFFECT_BLURE1, 0, 0, &blureInit);

        Actor_SetScale(&this->actor, 0.03f);
        this->floorPolyForCircleShadow = NULL;
        this->actor.gravity = -3.0f;

        if ((EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED) ||
            (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_NO_MASK)) {
            EnPp_SetupIdle(this);
        } else {
            EnPp_SetupJump(this);
        }
    }
}

void EnPp_Destroy(Actor* thisx, PlayState* play) {
    EnPp* this = (EnPp*)thisx;

    if (EN_PP_GET_TYPE(&this->actor) < EN_PP_TYPE_FRAGMENT_BASE) {
        Collider_DestroyJntSph(play, &this->maskCollider);
        Collider_DestroyJntSph(play, &this->bodyCollider);
        Collider_DestroyQuad(play, &this->hornCollider);
        if (this->blureIndex >= 0 && Effect_GetByIndex(this->blureIndex))
            Effect_Destroy(play, this->blureIndex);
    }
}

s32 EnPp_PointlessPosCheck(EnPp* this) {
    f32 diffX = this->actor.home.pos.x - this->actor.world.pos.x;
    f32 diffZ = this->actor.home.pos.z - this->actor.world.pos.z;
    f32 radius;
    s16 angle;
    Vec3f nearDuplicateWorldPos;

    radius = sqrtf(SQ(diffX) + SQ(diffZ));
    angle = Math_Vec3f_Yaw(&this->actor.home.pos, &this->actor.world.pos);

    Math_Vec3f_Copy(&nearDuplicateWorldPos, &this->actor.home.pos);
    nearDuplicateWorldPos.x += Math_SinS(angle) * radius;
    nearDuplicateWorldPos.z += Math_CosS(angle) * radius;

    Math_ApproachF(&this->actor.world.pos.x, nearDuplicateWorldPos.x, 0.3f, 2.0f);
    Math_ApproachF(&this->actor.world.pos.z, nearDuplicateWorldPos.z, 0.3f, 2.0f);

    diffX = this->actor.world.pos.x - nearDuplicateWorldPos.x;
    diffZ = this->actor.world.pos.z - nearDuplicateWorldPos.z;

    if (sqrtf(SQ(diffX) + SQ(diffZ)) < 2.0f) {
        this->actor.world.pos.x = nearDuplicateWorldPos.x;
        this->actor.world.pos.z = nearDuplicateWorldPos.z;
        return true;
    }

    return false;
}

void EnPp_SpawnDust(EnPp* this, PlayState* play) {
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    for (i = 0; i < ARRAY_COUNT(this->backFootPos); i++) {
        velocity.x = Rand_CenteredFloat(2.0f);
        velocity.y = Rand_ZeroFloat(2.0f) + 1.0f;
        velocity.z = Rand_CenteredFloat(2.0f);

        accel.y = -0.1f;
        accel.z = 0.0f;
        accel.x = 0.0f;

        pos.x = this->backFootPos[i].x + Rand_CenteredFloat(10.0f);
        pos.y = Rand_ZeroFloat(3.0f) + this->actor.floorHeight + 1.0f;
        pos.z = this->backFootPos[i].z + Rand_CenteredFloat(10.0f);

        func_800B0EB0(play, &pos, &velocity, &accel, &sDustPrimColor, &sDustEnvColor, (Rand_ZeroFloat(50.0f) + 60.0f),
                      30, (Rand_ZeroFloat(5.0f) + 20.0f));
    }
}

typedef enum {
    EN_PP_ANIM_IDLE,
    EN_PP_ANIM_WALK,
    EN_PP_ANIM_WIND_UP,
    EN_PP_ANIM_CHARGE,
    EN_PP_ANIM_ATTACK,
    EN_PP_ANIM_DAMAGE,
    EN_PP_ANIM_ROAR,
    EN_PP_ANIM_TURN_TO_FACE_PLAYER,
    EN_PP_ANIM_JUMP,
    EN_PP_ANIM_LAND,
    EN_PP_ANIM_MAX
} EnPpAnimation;

static AnimationHeader* sAnimations[EN_PP_ANIM_MAX] = {
    &gHiploopIdleAnim,   &gHiploopWalkAnim, &gHiploopWindUpAnim, &gHiploopChargeAnim, &gHiploopAttackAnim,
    &gHiploopDamageAnim, &gHiploopRoarAnim, &gHiploopWalkAnim,   &gHiploopWindUpAnim, &gHiploopWalkAnim,
};

static u8 sAnimationModes[EN_PP_ANIM_MAX] = {
    ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE,
    ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE,
};

void EnPp_ChangeAnim(EnPp* this, s32 animIndex) {
    f32 morphFrames = -10.0f;
    f32 playSpeed;
    f32 startFrame;

    this->animIndex = animIndex;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[this->animIndex]);

    if (this->animIndex >= EN_PP_ANIM_WIND_UP) {
        morphFrames = 0.0f;
    }

    playSpeed = 1.0f;
    if (this->animIndex == EN_PP_ANIM_TURN_TO_FACE_PLAYER) {
        playSpeed = 2.0f;
    }

    startFrame = 0.0f;
    if (this->action == EN_PP_ACTION_BODY_PART_MOVE) {
        startFrame = this->animEndFrame;
    }

    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], playSpeed, startFrame, this->animEndFrame,
                     sAnimationModes[this->animIndex], morphFrames);
}

void EnPp_PlaySfxForAnimation(EnPp* this) {
    if (this->action != EN_PP_ACTION_STUNNED_OR_FROZEN) {
        switch (this->animIndex) {
            case EN_PP_ANIM_WALK:
            case EN_PP_ANIM_TURN_TO_FACE_PLAYER:
                if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_FOOTSTEP);
                }
                break;

            case EN_PP_ANIM_WIND_UP:
                Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_FOOT - SFX_FLAG);
                break;

            case EN_PP_ANIM_CHARGE:
                Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_RUN - SFX_FLAG);
                break;

            case EN_PP_ANIM_ATTACK:
                if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_PAUSE);
                }
                break;

            case EN_PP_ANIM_ROAR:
                if (Animation_OnFrame(&this->skelAnime, 6.0f)) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_PAUSE);
                }
                break;

            default:
                break;
        }
    }
}

s32 EnPp_CheckCollision(EnPp* this, PlayState* play) {
    static s16 sLedgeCheckAngles[] = { 0x0000, 0x1000, 0xF000 };
    s16 angle;
    s32 i;

    angle = this->actor.world.rot.y;
    if (this->action == EN_PP_ACTION_DAMAGED) {
        angle = this->targetRotY;
    }

    Math_Vec3f_Copy(&this->wallCheckPos, &this->actor.world.pos);
    for (i = 0; i < ARRAY_COUNT(this->ledgeCheckPos); i++) {
        Math_Vec3f_Copy(&this->ledgeCheckPos[i], &this->actor.world.pos);
        this->ledgeCheckPos[i].x += Math_SinS(sLedgeCheckAngles[i] + angle) * 70.0f;
        this->ledgeCheckPos[i].y = this->actor.floorHeight - 10.0f;
        this->ledgeCheckPos[i].z += Math_CosS(sLedgeCheckAngles[i] + angle) * 70.0f;
        if (!BgCheck_SphVsFirstPoly(&play->colCtx, &this->ledgeCheckPos[i], 20.0f)) {
            return EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_OFF_LEDGE;
        }
    }

    this->wallCheckPos.x += Math_SinS(angle) * 40.0f;
    this->wallCheckPos.y = this->actor.world.pos.y + 20.0f;
    if (this->chargesInStraightLines) {
        this->wallCheckPos.y += 20.0f;
    }

    this->wallCheckPos.z += Math_CosS(angle) * 40.0f;
    if (BgCheck_SphVsFirstWall(&play->colCtx, &this->wallCheckPos, 10.0f)) {
        return EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_INTO_WALL;
    }

    return EN_PP_COLLISION_RESULT_OK;
}

void EnPp_SetupIdle(EnPp* this) {
    this->action = EN_PP_ACTION_IDLE;
    this->actionFunc = EnPp_Idle;
}

void EnPp_Idle(EnPp* this, PlayState* play) {
    Vec3f posToLookAt;

    SkelAnime_Update(&this->skelAnime);
    if (this->hasBeenDamaged || (this->actor.xzDistToPlayer < this->attackRange)) {
        EnPp_SetupCharge(this);
    } else {
        if ((this->timer == 0) && (this->secondaryTimer == 0)) {
            this->secondaryTimer = Rand_ZeroFloat(20.0f) + 20.0f;
            Math_Vec3f_Copy(&posToLookAt, &this->actor.home.pos);
            posToLookAt.x += Rand_CenteredFloat(50.0f);
            posToLookAt.z += Rand_CenteredFloat(50.0f);
            this->targetRotY = Math_Vec3f_Yaw(&this->actor.world.pos, &posToLookAt);
            this->actor.speed = 0.0f;
            if (this->animIndex != EN_PP_ANIM_IDLE) {
                EnPp_ChangeAnim(this, EN_PP_ANIM_IDLE);
            }
        }

        if (this->secondaryTimer == 1) {
            this->timer = Rand_ZeroFloat(40.0f) + 40.0f;
        }

        if (EnPp_CheckCollision(this, play) != EN_PP_COLLISION_RESULT_OK) {
            this->timer = 0;
            if (this->animIndex != EN_PP_ANIM_WALK) {
                EnPp_ChangeAnim(this, EN_PP_ANIM_WALK);
            }

            Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRotY, 1, 0x258, 0);
        } else if ((this->secondaryTimer == 0) && (this->timer != 0)) {
            if (this->animIndex == EN_PP_ANIM_IDLE) {
                EnPp_ChangeAnim(this, EN_PP_ANIM_WALK);
            }

            if ((this->maskBounceAngularVelocity < 0x64) &&
                (fabsf(this->actor.world.rot.y - this->targetRotY) < 100.0f)) {
                Math_ApproachF(&this->actor.speed, 1.0f, 0.3f, 1.0f);
            }

            Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRotY, 1, this->maskBounceAngularVelocity + 0x258,
                               0);
            Math_SmoothStepToS(&this->maskBounceAngularVelocity, 0, 1, 0x1F4, 0);
        }
    }
}

void EnPp_SetupCharge(EnPp* this) {
    this->secondaryTimer = this->timer = 0;
    this->actionVar.isCharging = false;
    EnPp_ChangeAnim(this, EN_PP_ANIM_TURN_TO_FACE_PLAYER);
    this->action = EN_PP_ACTION_CHARGE;
    this->actionFunc = EnPp_Charge;
}

void EnPp_Charge(EnPp* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s32 yawDiff;
    Vec3f distanceFromWorldPos;

    SkelAnime_Update(&this->skelAnime);

    if (!this->actionVar.isCharging || (this->animIndex == EN_PP_ANIM_WIND_UP)) {
        this->targetRotY = this->actor.yawTowardsPlayer;
        if (this->chargesInStraightLines) {
            yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
            this->targetRotY = this->actor.home.rot.y + 0x8000;
            if (yawDiff < 0x3000) {
                this->targetRotY = this->actor.home.rot.y;
            }

            Math_Vec3f_Copy(&this->targetPos, &this->actor.world.pos);
            Matrix_RotateYS(this->targetRotY, MTXMODE_NEW);
            Matrix_MultVecZ(300.0f, &distanceFromWorldPos);
            this->targetPos.x += distanceFromWorldPos.x;
            this->targetPos.z += distanceFromWorldPos.z;
        }

        Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRotY, 1, this->maskBounceAngularVelocity + 0x7D0, 0);
    }

    Math_SmoothStepToS(&this->maskBounceAngularVelocity, 0, 1, 0x1F4, 0);
    if (!this->actionVar.isCharging) {
        Math_ApproachZeroF(&this->actor.speed, 0.5f, 1.0f);
        if (fabsf(this->actor.world.rot.y - this->targetRotY) < 100.0f) {
            if (this->chargesInStraightLines) {
                this->actor.world.rot.y = this->targetRotY;
            }

            this->actionVar.isCharging = true;
        }
    } else if (this->animIndex == EN_PP_ANIM_TURN_TO_FACE_PLAYER) {
        EnPp_ChangeAnim(this, EN_PP_ANIM_WIND_UP);
    } else if (this->animIndex == EN_PP_ANIM_WIND_UP) {
        if (curFrame >= this->animEndFrame) {
            this->chargeAndBounceSpeed = 14.0f;
            this->timer = 20;
            EnPp_ChangeAnim(this, EN_PP_ANIM_CHARGE);
        }
    } else if (this->animIndex == EN_PP_ANIM_CHARGE) {
        if (EnPp_CheckCollision(this, play) != EN_PP_COLLISION_RESULT_OK) {
            this->actor.speed = 0.0f;
            EnPp_SetupRoar(this);
            return;
        }

        if (!this->chargesInStraightLines) {
            Math_ApproachF(&this->actor.speed, 10.0f, 0.3f, 1.0f);
        } else {
            Math_ApproachF(&this->actor.world.pos.x, this->targetPos.x, 0.5f,
                           fabsf(Math_SinS(this->targetRotY) * this->chargeAndBounceSpeed));
            Math_ApproachF(&this->actor.world.pos.z, this->targetPos.z, 0.5f,
                           fabsf(Math_CosS(this->targetRotY) * this->chargeAndBounceSpeed));
            if (this->timer < 10) {
                Math_ApproachZeroF(&this->chargeAndBounceSpeed, 0.3f, 0.2f);
            }
        }

        if (this->timer == 0) {
            EnPp_SetupRoar(this);
            return;
        }

        if (!(this->maskCollider.base.atFlags & AT_BOUNCED) && !(this->bodyCollider.base.atFlags & AT_BOUNCED)) {
            if ((this->maskCollider.base.atFlags & AT_HIT) || (this->bodyCollider.base.atFlags & AT_HIT)) {
                EnPp_SetupAttack(this);
                return;
            }
        } else if (this->chargesInStraightLines) {
            EnPp_SetupBounced(this);
            return;
        } else {
            if (EN_PP_GET_TYPE(&this->actor) != EN_PP_TYPE_MASKED) {
                this->actor.speed *= -1.0f;
            } else {
                this->actor.speed *= -0.5f;
            }

            EnPp_SetupRoar(this);
            return;
        }
    }

    if (((this->animIndex == EN_PP_ANIM_WIND_UP) || (this->animIndex == EN_PP_ANIM_CHARGE)) &&
        (this->secondaryTimer == 0)) {
        EnPp_SpawnDust(this, play);
        this->secondaryTimer = 3;
    }
}

void EnPp_SetupAttack(EnPp* this) {
    EnPp_ChangeAnim(this, EN_PP_ANIM_ATTACK);
    this->hornColliderOn = true;
    this->action = EN_PP_ACTION_ATTACK;
    this->actor.speed = 0.0f;
    this->actionFunc = EnPp_Attack;
}

void EnPp_Attack(EnPp* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    SkelAnime_Update(&this->skelAnime);
    if (curFrame >= this->animEndFrame) {
        EnPp_SetupCharge(this);
    }
}

void EnPp_SetupBounced(EnPp* this) {
    Vec3f distanceFromWorldPos;
    s32 yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));

    this->targetRotY = this->actor.home.rot.y + 0x8000;
    if (yawDiff < 0x3000) {
        this->targetRotY = this->actor.home.rot.y;
    }

    Math_Vec3f_Copy(&this->targetPos, &this->actor.world.pos);
    Matrix_RotateYS(this->targetRotY, MTXMODE_NEW);
    Matrix_MultVecZ(-200.0f, &distanceFromWorldPos);
    this->timer = 10;
    this->targetPos.x += distanceFromWorldPos.x;
    this->targetPos.z += distanceFromWorldPos.z;
    EnPp_ChangeAnim(this, EN_PP_ANIM_DAMAGE);
    this->actor.speed = 0.0f;
    this->action = EN_PP_ACTION_BOUNCED;
    this->chargeAndBounceSpeed = 14.0f;
    this->actionFunc = EnPp_Bounced;
}

void EnPp_Bounced(EnPp* this, PlayState* play) {
    Math_ApproachF(&this->actor.world.pos.x, this->targetPos.x, 0.5f,
                   fabsf(Math_SinS(this->targetRotY) * this->chargeAndBounceSpeed));
    Math_ApproachF(&this->actor.world.pos.z, this->targetPos.z, 0.5f,
                   fabsf(Math_CosS(this->targetRotY) * this->chargeAndBounceSpeed));
    Math_ApproachZeroF(&this->chargeAndBounceSpeed, 0.3f, 0.2f);
    if ((this->timer == 0) && (EnPp_PointlessPosCheck(this))) {
        EnPp_SetupCharge(this);
    }
}

void EnPp_SetupRoar(EnPp* this) {
    this->actionVar.hasDoneFirstRoar = false;
    this->timer = 20;
    this->secondaryTimer = 0;
    this->chargeAndBounceSpeed = 14.0f;
    EnPp_ChangeAnim(this, EN_PP_ANIM_ROAR);
    this->action = EN_PP_ACTION_ROAR;
    this->actionFunc = EnPp_Roar;
}

void EnPp_Roar(EnPp* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;

    SkelAnime_Update(&this->skelAnime);

    if (EnPp_CheckCollision(this, play) == EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_OFF_LEDGE) {
        this->actor.speed = 0.0f;
        this->chargeAndBounceSpeed = 0.0f;
    }

    if (!this->chargesInStraightLines) {
        if ((fabsf(this->actor.home.pos.y - this->actor.world.pos.y) > 100.0f) &&
            (fabsf(this->actor.floorHeight - player->actor.floorHeight) > 100.0f)) {
            EnPp_SetupDead(this, play);
            return;
        }
    } else if (this->chargeAndBounceSpeed) {
        Math_ApproachF(&this->actor.world.pos.x, this->targetPos.x, 0.5f,
                       fabsf(Math_SinS(this->targetRotY) * this->chargeAndBounceSpeed));
        Math_ApproachF(&this->actor.world.pos.z, this->targetPos.z, 0.5f,
                       fabsf(Math_CosS(this->targetRotY) * this->chargeAndBounceSpeed));
        if (this->timer < 10) {
            Math_ApproachZeroF(&this->chargeAndBounceSpeed, 0.3f, 0.2f);
        }
    }

    Math_ApproachZeroF(&this->actor.speed, 0.5f, 1.0f);
    if ((this->actor.speed > 0.3f) && (this->secondaryTimer == 0)) {
        EnPp_SpawnDust(this, play);
        this->secondaryTimer = 3;
    }

    if (!this->actionVar.hasDoneFirstRoar && (curFrame >= this->animEndFrame)) {
        this->skelAnime.startFrame = 6.0f;
        this->actionVar.hasDoneFirstRoar = true;
    }

    if (this->timer == 0) {
        EnPp_SetupCharge(this);
    }
}

void EnPp_SetupJump(EnPp* this) {
    s32 yawDiff;

    this->secondaryTimer = 0;
    this->actionVar.hasLandedFromJump = false;
    this->timer = this->secondaryTimer;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 20.0f;
    this->actor.gravity = -3.0f;
    EnPp_ChangeAnim(this, EN_PP_ANIM_JUMP);

    if (this->chargesInStraightLines) {
        yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
        this->actor.world.rot.y = this->actor.home.rot.y + 0x8000;
        if (yawDiff < 0x3000) {
            this->actor.world.rot.y = this->actor.home.rot.y;
        }

        this->targetRotY = this->actor.world.rot.y;
    }

    this->actionVar.hasLandedFromJump = false;
    this->action = EN_PP_ACTION_JUMP;
    this->actionFunc = EnPp_Jump;
}

void EnPp_Jump(EnPp* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s32 yawDiff;

    SkelAnime_Update(&this->skelAnime);

    if (this->chargesInStraightLines) {
        yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
        this->targetRotY = this->actor.home.rot.y + 0x8000;
        if (yawDiff < 0x3000) {
            this->targetRotY = this->actor.home.rot.y;
        }

        EnPp_PointlessPosCheck(this);
    } else {
        this->targetRotY = this->actor.yawTowardsPlayer;
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, this->targetRotY, 1, 0x1388, 0);
    if (!this->actionVar.hasLandedFromJump) {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
            this->actionVar.hasLandedFromJump = true;
            EnPp_ChangeAnim(this, EN_PP_ANIM_LAND);
        }
    } else if (curFrame >= this->animEndFrame) {
        EnPp_SetupIdle(this);
    }
}

void EnPp_SetupStunnedOrFrozen(EnPp* this) {
    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer == 0)) {
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    this->actor.speed = 0.0f;
    this->action = EN_PP_ACTION_STUNNED_OR_FROZEN;
    this->actionFunc = EnPp_StunnedOrFrozen;
}

void EnPp_StunnedOrFrozen(EnPp* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
        if ((this->drawDmgEffTimer != 0) && (this->drawDmgEffTimer < 60)) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
        }
    }

    if ((this->secondaryTimer == 0) && (this->drawDmgEffTimer == 0)) {
        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
            (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
            Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_PP_BODYPART_MAX, 2, 0.7f, 0.4f);
            this->drawDmgEffTimer = 0;
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        }

        EnPp_SetupCharge(this);
    }
}

void EnPp_SetupDamaged(EnPp* this, PlayState* play) {
    Vec3f temp;
    s32 yawDiff;

    if (!this->chargesInStraightLines) {
        Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
        Matrix_MultVecZ(-30.0f, &temp);
        Math_Vec3f_Copy(&this->damagedVelocity, &temp);
    } else {
        yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
        this->targetRotY = this->actor.home.rot.y + 0x8000;
        if (yawDiff < 0x3000) {
            this->targetRotY = this->actor.home.rot.y;
        }

        Math_Vec3f_Copy(&this->targetPos, &this->actor.world.pos);
        Matrix_RotateYS(this->targetRotY, MTXMODE_NEW);
        Matrix_MultVecZ(-200.0f, &temp);
        this->timer = 10;
        this->targetPos.x += temp.x;
        this->targetPos.z += temp.z;
    }

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer != 0)) {
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_PP_BODYPART_MAX, 2, 0.7f, 0.4f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->secondaryTimer = 0;
    EnPp_ChangeAnim(this, EN_PP_ANIM_DAMAGE);
    this->targetRotY = this->actor.yawTowardsPlayer + 0x8000;
    this->action = EN_PP_ACTION_DAMAGED;
    if (EnPp_CheckCollision(this, play) == EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_OFF_LEDGE) {
        this->damagedVelocity.z = 0.0f;
        this->damagedVelocity.x = 0.0f;
        this->actor.speed = 0.0f;
    }

    Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_DAMAGE);
    this->actionFunc = EnPp_Damaged;
}

void EnPp_Damaged(EnPp* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    this->actor.world.rot.y += 0x1000;
    if (this->secondaryTimer == 0) {
        EnPp_SpawnDust(this, play);
        this->secondaryTimer = 3;
    }

    if (this->chargesInStraightLines) {
        Math_ApproachF(&this->actor.world.pos.x, this->targetPos.x, 0.5f, 25.0f);
        Math_ApproachF(&this->actor.world.pos.z, this->targetPos.z, 0.5f, 25.0f);
        this->actor.world.rot.y += 0x1000;
        if ((this->timer == 0) && (EnPp_PointlessPosCheck(this))) {
            EnPp_SetupCharge(this);
        }
    } else if ((this->actor.colorFilterTimer == 0) && (this->damagedVelocity.x < 1.0f) &&
               (this->damagedVelocity.z < 1.0f)) {
        EnPp_SetupCharge(this);
    }

    if (EnPp_CheckCollision(this, play) == EN_PP_COLLISION_RESULT_ABOUT_TO_RUN_OFF_LEDGE) {
        this->damagedVelocity.z = 0.0f;
        this->damagedVelocity.x = 0.0f;
        this->actor.speed = 0.0f;
    }

    if ((fabsf(this->actor.home.pos.y - this->actor.world.pos.y) > 100.0f) &&
        (fabsf(this->actor.floorHeight - player->actor.floorHeight) > 100.0f)) {
        EnPp_SetupDead(this, play);
    }
}

void EnPp_SetupDead(EnPp* this, PlayState* play) {
    Vec3f deadVelocity;
    s32 yawDiff;

    this->secondaryTimer = this->timer = 0;
    if (!this->chargesInStraightLines) {
        Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
        Matrix_MultVecZ(-30.0f, &deadVelocity);
        Math_Vec3f_Copy(&this->damagedVelocity, &deadVelocity);
    } else {
        yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
        this->targetRotY = this->actor.home.rot.y + 0x8000;
        if (yawDiff < 0x3000) {
            this->targetRotY = this->actor.home.rot.y;
        }

        Math_Vec3f_Copy(&this->targetPos, &this->actor.world.pos);
        Matrix_RotateYS(this->targetRotY, MTXMODE_NEW);
        Matrix_MultVecZ(-200.0f, &deadVelocity);
        this->targetPos.x += deadVelocity.x;
        this->targetPos.z += deadVelocity.z;
    }

    this->maskBounceAngularVelocity = this->actionVar.isCharging = 0;
    EnPp_ChangeAnim(this, EN_PP_ANIM_DAMAGE);
    this->timer = 15;
    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer == 0)) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 25);
    Enemy_StartFinishingBlow(play, &this->actor);
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_HIPLOOP_DEAD);
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->action = EN_PP_ACTION_DEAD;
    this->actionFunc = EnPp_Dead;
}

void EnPp_Dead(EnPp* this, PlayState* play) {
    Vec3f splashPos;
    WaterBox* waterBox;
    f32 waterSurface;
    s32 isUnderWater;
    s32 i;

    SkelAnime_Update(&this->skelAnime);
    this->actor.world.rot.y += 0x3000;
    waterSurface = this->actor.world.pos.y - 30.0f;
    if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
        (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
        if (this->drawDmgEffTimer == 0) {
            return;
        }

        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_PP_BODYPART_MAX, 2, 0.7f, 0.4f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    if (this->secondaryTimer == 0) {
        EnPp_SpawnDust(this, play);
        this->secondaryTimer = 3;
    }

    isUnderWater = false;
    if ((this->action == EN_PP_ACTION_DEAD) &&
        (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                          &waterSurface, &waterBox))) {
        if (this->actor.world.pos.y < waterSurface) {
            for (i = 0; i < 5; i++) {
                Math_Vec3f_Copy(&splashPos, &this->actor.world.pos);
                splashPos.x += Rand_CenteredFloat(10 + (5 * i));
                splashPos.z += Rand_CenteredFloat(40 + (5 * i));
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (Rand_ZeroOne() * 100.0f) + 400.0f);
            }

            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EV_BOMB_DROP_WATER);
            isUnderWater = true;
        }
    }

    if (this->chargesInStraightLines) {
        Math_ApproachF(&this->actor.world.pos.x, this->targetPos.x, 0.5f, 25.0f);
        Math_ApproachF(&this->actor.world.pos.z, this->targetPos.z, 0.5f, 25.0f);
    }

    if ((this->action == EN_PP_ACTION_DEAD) && (isUnderWater || (this->timer == 1))) {
        this->action = EN_PP_ACTION_SPAWN_BODY_PARTS;
    }

    if (this->action == EN_PP_ACTION_DONE_SPAWNING_BODY_PARTS) {
        if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_UNMASKED) {
            Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0x70);
        } else {
            Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0xE0);
        }

        Actor_Kill(&this->actor);
    }
}

void EnPp_Mask_SetupDetach(EnPp* this, PlayState* play) {
    s32 yawDiff;
    EnPp* pp = (EnPp*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PP, this->actor.world.pos.x, this->actor.world.pos.y,
                                  this->actor.world.pos.z, this->actor.world.rot.x, this->actor.shape.rot.y,
                                  this->actor.world.rot.z, EN_PP_TYPE_UNMASKED);

    if (pp != NULL) {
        if (this->chargesInStraightLines) {
            yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.home.rot.y));
            pp->chargesInStraightLines = true;
            pp->actor.world.rot.y = this->actor.home.rot.y + 0x8000;
            if (yawDiff < 0x3000) {
                pp->actor.world.rot.y = this->actor.home.rot.y;
            }
        }

        this->actor.gravity = 0.0f;
        this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        this->actionVar.maskDetachState = EN_PP_MASK_DETACH_STATE_START;
        EnPp_ChangeAnim(this, EN_PP_ANIM_IDLE);
        SkelAnime_Update(&this->skelAnime);
        this->action = EN_PP_ACTION_MASK_DETACH;
        this->actionFunc = EnPp_Mask_Detach;
    }
}

void EnPp_Mask_Detach(EnPp* this, PlayState* play) {
    if (!CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_HOOKSHOT_ATTACHED) || (this->action == EN_PP_ACTION_MASK_DEAD)) {
        switch (this->actionVar.maskDetachState) {
            case EN_PP_MASK_DETACH_STATE_START:
                this->action = EN_PP_ACTION_MASK_DEAD;
                this->maskAccelY = 50.0f;
                this->maskVelocity.y = -230.0f;
                this->maskVelocity.x = -150.0f;
                Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_MASC_OFF);
                this->actionVar.maskDetachState = EN_PP_MASK_DETACH_STATE_FALL;
                break;

            case EN_PP_MASK_DETACH_STATE_FALL:
                if (this->maskPos.y > 800.0f) {
                    this->maskAccelY = 50.0f;
                    this->actionVar.maskDetachState++;
                    this->maskVelocity.y = -200.0f;
                    this->maskVelocity.x = -150.0f;
                }
                break;

            case EN_PP_MASK_DETACH_STATE_DIE:
                if (this->maskPos.y > 900.0f) {
                    if (this->maskVelocity.y > 0.0f) {
                        Vec3f maskFirePos;
                        Vec3f sMaskFireVelocityAndAccel[] = {
                            { 1.0f, 0.0f, 0.5f },   { 1.0f, 0.0f, -0.5f },  { -1.0f, 0.0f, 0.5f },
                            { -1.0f, 0.0f, -0.5f }, { 0.5f, 0.0f, 1.0f },   { -0.5f, 0.0f, 1.0f },
                            { 0.5f, 0.0f, -1.0f },  { -0.5f, 0.0f, -1.0f }, { 0.0f, 0.0f, 0.0f },
                        };
                        s32 i;

                        for (i = 0; i < ARRAY_COUNT(sMaskFireVelocityAndAccel); i++) {
                            Math_Vec3f_Copy(&maskFirePos, &this->maskFlamesBasePos);
                            maskFirePos.x += Rand_CenteredFloat(20.0f);
                            maskFirePos.y = this->actor.floorHeight;
                            maskFirePos.z += Rand_CenteredFloat(20.0f);
                            func_800B3030(play, &maskFirePos, &sMaskFireVelocityAndAccel[i],
                                          &sMaskFireVelocityAndAccel[i], 70, 0, 2);
                        }

                        Actor_Kill(&this->actor);
                    }
                }
                break;
        }

        this->maskVelocity.y += this->maskAccelY;
        this->maskRot.z += 0x2000;
        this->maskPos.x += this->maskVelocity.x;
        this->maskPos.y += this->maskVelocity.y;
        this->maskPos.z += this->maskVelocity.z;
    }
}

void EnPp_Fragment_SetupMove(EnPp* this) {
    EnPp_ChangeAnim(this, EN_PP_ANIM_DAMAGE);
    this->actor.velocity.y = Rand_ZeroFloat(5.0f) + 13.0f;
    this->actor.gravity = -2.0f;
    this->timer = Rand_S16Offset(30, 30);
    this->fragmentAngularVelocity.x = (this->fragmentIndex * 0x2E) + 0xFF00;
    this->fragmentAngularVelocity.z = (this->fragmentIndex * 0x2E) + 0xFF00;
    if (EN_PP_GET_TYPE(&this->actor) != EN_PP_TYPE_FRAGMENT_BODY) {
        this->actor.speed = Rand_ZeroFloat(4.0f) + 4.0f;
        this->actor.world.rot.y = ((s32)Rand_CenteredFloat(223.0f) + 0x1999) * this->fragmentIndex;
    }

    this->action = EN_PP_ACTION_BODY_PART_MOVE;
    this->actionFunc = EnPp_Fragment_Move;
}

void EnPp_Fragment_Move(EnPp* this, PlayState* play) {
    Vec3f splashPos;
    WaterBox* waterBox;
    f32 waterSurface;
    s32 i;

    SkelAnime_Update(&this->skelAnime);

    if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_FRAGMENT_BODY) {
        this->fragmentFlameCount = ARRAY_COUNT(this->fragmentFlamesPos);
        for (i = 0; i < ARRAY_COUNT(this->fragmentFlamesPos); i++) {
            Math_Vec3f_Copy(&this->fragmentFlamesPos[i], &this->fragmentPos);
            this->fragmentFlamesPos[i].x += Math_SinS(0xCCC * i) * 15.0f;
            this->fragmentFlamesPos[i].y += -5.0f;
            this->fragmentFlamesPos[i].z += Math_CosS(0xCCC * i) * 15.0f;
        }
    } else {
        Math_Vec3f_Copy(&this->fragmentFlamesPos[0], &this->fragmentPos);
        this->fragmentFlameCount = 1;
        this->actor.shape.rot.x += this->fragmentAngularVelocity.x;
        this->actor.shape.rot.z += this->fragmentAngularVelocity.z;
    }

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &waterSurface, &waterBox) &&
        (this->actor.world.pos.y < (waterSurface + 5.0f))) {
        this->timer = 0;
        if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_FRAGMENT_BODY) {
            for (i = 0; i < 6; i++) {
                Math_Vec3f_Copy(&splashPos, &this->actor.world.pos);
                splashPos.x += Rand_CenteredFloat(10 + (5 * i));
                splashPos.z += Rand_CenteredFloat(40 + (5 * i));
                EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 0, (Rand_ZeroOne() * 100.0f) + 400.0f);
            }
        } else {
            EffectSsGSplash_Spawn(play, &this->actor.world.pos, NULL, NULL, 0, 400);
        }

        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EV_BOMB_DROP_WATER);
    }

    if ((this->timer == 0) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        Actor_Kill(&this->actor);
    }
}

void EnPp_UpdateDamage(EnPp* this, PlayState* play) {
    s16 yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.world.rot.y));
    s32 attackBouncedOffMask = false;
    s32 attackDealsDamage = false;

    if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED) {
        if ((yawDiff < (BREG(2) + 0x4A9C)) || (EN_PP_GET_TYPE(&this->actor) > EN_PP_TYPE_MASKED)) {
            this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
        } else {
            this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
        }
    }

    if ((EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED) && (this->action < EN_PP_ACTION_MASK_DETACH)) {
        if ((yawDiff < (BREG(2) + 0x4A9C)) && CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_HOOKSHOT_ATTACHED)) {
            EnPp_Mask_SetupDetach(this, play);
        } else if (this->maskCollider.elements[0].base.acElemFlags & ACELEM_HIT) {
            if (yawDiff < (BREG(2) + 0x4A9C)) {
                if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_HOOKSHOT) {
                    EnPp_Mask_SetupDetach(this, play);
                } else if ((this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_GORON_POUND) ||
                           (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_KNOCK_OFF_MASK)) {
                    EnPp_Mask_SetupDetach(this, play);
                    this->action = EN_PP_ACTION_MASK_DEAD;
                }
            } else {
                attackBouncedOffMask = true;
            }
        } else if (this->maskCollider.elements[0].base.acElemFlags & ACELEM_HIT) {
            attackBouncedOffMask = true;
        }
    }

    if (this->bodyCollider.elements[0].base.acElemFlags & ACELEM_HIT) {
        if (EN_PP_GET_TYPE(&this->actor) != EN_PP_TYPE_MASKED) {
            if ((this->action < EN_PP_ACTION_DAMAGED) && (this->action != EN_PP_ACTION_JUMP)) {
                if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_HOOKSHOT) {
                    if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED) {
                        return;
                    }
                    attackDealsDamage = true;
                }

                if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_ELECTRIC_STUN) {
                    if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                         (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                        (this->drawDmgEffTimer == 0)) {
                        this->drawDmgEffTimer = 40;
                        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
                        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA,
                                             40);
                        EnPp_SetupStunnedOrFrozen(this);
                    }
                    return;
                } else if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_STUN) {
                    if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                         (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                        (this->drawDmgEffTimer == 0)) {
                        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA,
                                             40);
                        this->secondaryTimer = 40;
                        EnPp_SetupStunnedOrFrozen(this);
                    }
                    return;
                } else if ((this->actor.colChkInfo.damageEffect != EN_PP_DMGEFF_GORON_POUND) &&
                           (this->actor.colChkInfo.damageEffect != EN_PP_DMGEFF_JUMP)) {
                    attackDealsDamage = true;
                    this->hasBeenDamaged = true;
                    this->actor.speed = 0.0f;
                    if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_FIRE) {
                        this->drawDmgEffTimer = 40;
                        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                    } else if (this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_FREEZE) {
                        if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                             (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                            (this->drawDmgEffTimer == 0)) {
                            Actor_ApplyDamage(&this->actor);
                            this->drawDmgEffTimer = 80;
                            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_SFX;
                            this->drawDmgEffScale = 0.0f;
                            this->drawDmgEffFrozenSteamScale = 1.5f;
                        }

                        if (this->actor.colChkInfo.health <= 0) {
                            EnPp_SetupDead(this, play);
                        } else {
                            EnPp_SetupStunnedOrFrozen(this);
                        }
                        return;
                    } else if ((this->actor.colChkInfo.damageEffect == EN_PP_DMGEFF_LIGHT_ORB) &&
                               ((((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX)) &&
                                 (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                                (this->drawDmgEffTimer == 0))) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x,
                                    this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0,
                                    CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, 255, COLORFILTER_BUFFLAG_OPA,
                                             25);
                        this->drawDmgEffTimer = 20;
                        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                    }
                } else if (this->action != EN_PP_ACTION_JUMP) {
                    EnPp_SetupJump(this);
                }
            }

            if (attackDealsDamage) {
                Actor_ApplyDamage(&this->actor);
                if (this->actor.colChkInfo.health > 0) {
                    EnPp_SetupDamaged(this, play);
                } else {
                    EnPp_SetupDead(this, play);
                }
            }
        } else {
            attackBouncedOffMask = true;
        }
    }

    if (attackBouncedOffMask && (((this->action == EN_PP_ACTION_IDLE)) || (this->action == EN_PP_ACTION_CHARGE) ||
                                 (this->action == EN_PP_ACTION_ROAR))) {
        this->secondaryTimer = 0;
        this->timer = 10;
        this->maskBounceAngularVelocity = 0x20D0;
        this->actor.speed = 0.0f;
        if (this->action == EN_PP_ACTION_CHARGE) {
            this->actionVar.isCharging = false;
            EnPp_ChangeAnim(this, EN_PP_ANIM_TURN_TO_FACE_PLAYER);
            this->maskBounceAngularVelocity = 0x1B58;
        }

        this->targetRotY = this->actor.yawTowardsPlayer;
        if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED) {
            Actor_PlaySfx(&this->actor, NA_SE_IT_SHIELD_BOUND);
        }
    }
}

void EnPp_Update(Actor* thisx, PlayState* play) {
    EnPp* this = (EnPp*)thisx;

    DECR(this->timer);

    if (this->secondaryTimer != 0) {
        this->secondaryTimer--;
    }

    if (this->drawDmgEffTimer != 0) {
        this->drawDmgEffTimer--;
    }

    this->actionFunc(this, play);
    EnPp_PlaySfxForAnimation(this);
    Actor_MoveWithGravity(&this->actor);

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        if (EN_PP_GET_TYPE(&this->actor) > EN_PP_TYPE_MASKED) {
            this->maskCollider.elements[0].dim.modelSphere.radius = 42;
            this->maskCollider.elements[0].dim.modelSphere.center.x = 400;
            this->maskCollider.elements[0].dim.modelSphere.center.y = -400;
            this->bodyCollider.elements[0].dim.modelSphere.radius = 20;
            this->bodyCollider.elements[0].dim.modelSphere.center.x = 400;
            this->bodyCollider.elements[0].dim.modelSphere.center.y = -400;
        } else {
            this->maskCollider.elements[0].dim.modelSphere.radius = 25;
            this->maskCollider.elements[0].dim.modelSphere.center.x = 780;
            this->maskCollider.elements[0].dim.modelSphere.center.y = -500;
            this->bodyCollider.elements[0].dim.modelSphere.radius = 25;
            this->bodyCollider.elements[0].dim.modelSphere.center.x = 400;
            this->bodyCollider.elements[0].dim.modelSphere.center.y = -400;
        }
    }

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        EnPp_UpdateDamage(this, play);
        Actor_SetFocus(&this->actor, 40.0f);
        if ((this->action == EN_PP_ACTION_DEAD) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            this->actor.world.pos.x += this->damagedVelocity.x;
            this->actor.world.pos.z += this->damagedVelocity.z;
            Math_ApproachZeroF(&this->damagedVelocity.x, 1.0f, 2.0f);
            Math_ApproachZeroF(&this->damagedVelocity.z, 1.0f, 2.0f);
        }
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 40.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    if (this->action == EN_PP_ACTION_SPAWN_BODY_PARTS) {
        static const s16 limbs[] = {
            HIPLOOP_LIMB_BODY,
            HIPLOOP_LIMB_FRONT_LEFT_LOWER_LEG,
            HIPLOOP_LIMB_FRONT_RIGHT_LOWER_LEG,
            HIPLOOP_LIMB_LEFT_WING_MIDDLE,
            HIPLOOP_LIMB_RIGHT_WING_MIDDLE,
            HIPLOOP_LIMB_CENTER_WING_MIDDLE,
        };
        for (s32 i = 0; i < ARRAY_COUNT(limbs); ++i) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PP, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, this->actor.world.rot.x, this->actor.world.rot.y,
                        this->actor.world.rot.z, limbs[i] + EN_PP_TYPE_FRAGMENT_BASE);
        }
        this->action = EN_PP_ACTION_DONE_SPAWNING_BODY_PARTS;
    }

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        if (this->floorPolyForCircleShadow == NULL) {
            this->floorPolyForCircleShadow = this->actor.floorPoly;
        }

        if ((this->action < EN_PP_ACTION_DEAD) || (this->action == EN_PP_ACTION_JUMP)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->maskCollider.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }

        if ((this->action < EN_PP_ACTION_MASK_DETACH) && (this->action != EN_PP_ACTION_STUNNED_OR_FROZEN)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->maskCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->bodyCollider.base);
        }

        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
        if (this->hornColliderOn) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->hornCollider.base);
        }
    }
}

s32 EnPp_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnPp* this = (EnPp*)thisx;

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        if ((limbIndex != HIPLOOP_LIMB_MASK) &&
            ((this->action == EN_PP_ACTION_MASK_DETACH) || (this->action == EN_PP_ACTION_MASK_DEAD))) {
            *dList = NULL;
        }

        if ((limbIndex == HIPLOOP_LIMB_MASK) && (EN_PP_GET_TYPE(&this->actor) != EN_PP_TYPE_MASKED)) {
            *dList = NULL;
        }

        if (limbIndex == HIPLOOP_LIMB_MASK) {
            rot->x += TRUNCF_BINANG(this->maskRot.x);
            rot->y += TRUNCF_BINANG(this->maskRot.y);
            rot->z += TRUNCF_BINANG(this->maskRot.z);
            pos->x += this->maskPos.x;
            pos->y += this->maskPos.y;
            pos->z += this->maskPos.z;
        }
    } else if ((limbIndex + EN_PP_TYPE_FRAGMENT_BASE) != EN_PP_GET_TYPE(&this->actor)) {
        *dList = NULL;
    }

    return false;
}

void EnPp_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Vec3f sVertexOffset1 = { 0.0f, 0.0f, 0.0f };
    static Vec3f sVertexOffset2 = { 0.0f, 0.0f, 0.0f };
    EnPp* this = (EnPp*)thisx;
    Vec3f blureVertex1;
    Vec3f blureVertex2;

    Matrix_Push();

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        if (limbIndex == HIPLOOP_LIMB_BACK_RIGHT_FOOT) {
            Matrix_MultVec3f(&gZeroVec3f, &this->backFootPos[0]);
        }

        if (limbIndex == HIPLOOP_LIMB_BACK_LEFT_FOOT) {
            Matrix_MultVec3f(&gZeroVec3f, &this->backFootPos[1]);
        }

        if ((limbIndex == HIPLOOP_LIMB_MASK) && (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_MASKED)) {
            Matrix_MultVec3f(&gZeroVec3f, &this->maskFlamesBasePos);
            if (this->hornColliderOn) {
                if ((this->skelAnime.curFrame > 26.0f) || (this->action >= EN_PP_ACTION_MASK_DETACH)) {
                    this->hornColliderOn = false;
                    EffectBlure* trail = this->blureIndex >= 0 ? Effect_GetByIndex(this->blureIndex) : NULL;
                    if (trail)
                        EffectBlure_AddSpace(trail);
                } else if ((this->skelAnime.curFrame > 0.0f) && (this->skelAnime.curFrame < 10.0f)) {
                    Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
                    sVertexOffset1.x = 1160.0f;
                    sVertexOffset1.y = -900.0f;
                    sVertexOffset1.z = 0.0f;
                    sVertexOffset2.x = 100.0f;
                    sVertexOffset2.y = 300.0f;
                    sVertexOffset2.z = 0.0f;
                    Math_Vec3f_Copy(&this->hornCollider.dim.quad[3], &this->hornCollider.dim.quad[1]);
                    Math_Vec3f_Copy(&this->hornCollider.dim.quad[2], &this->hornCollider.dim.quad[0]);
                    Matrix_MultVec3f(&sVertexOffset1, &this->hornCollider.dim.quad[1]);
                    Matrix_MultVec3f(&sVertexOffset2, &this->hornCollider.dim.quad[0]);
                    Collider_SetQuadVertices(&this->hornCollider, &this->hornCollider.dim.quad[0],
                                             &this->hornCollider.dim.quad[1], &this->hornCollider.dim.quad[2],
                                             &this->hornCollider.dim.quad[3]);
                    Matrix_MultVec3f(&sVertexOffset1, &blureVertex1);
                    Matrix_MultVec3f(&sVertexOffset2, &blureVertex2);
                    EffectBlure* trail = this->blureIndex >= 0 ? Effect_GetByIndex(this->blureIndex) : NULL;
                    if (trail)
                        EffectBlure_AddVertex(trail, &blureVertex1, &blureVertex2);
                }
            }
        }
    } else {
        if ((EN_PP_GET_TYPE(&this->actor) >= EN_PP_TYPE_FRAGMENT_BASE) &&
            ((limbIndex + EN_PP_TYPE_FRAGMENT_BASE) == EN_PP_GET_TYPE(&this->actor))) {
            Matrix_MultVec3f(&gZeroVec3f, &this->fragmentPos);
        }
    }

    Matrix_Pop();

    if (this->action != EN_PP_ACTION_BODY_PART_MOVE) {
        Collider_UpdateSpheres(limbIndex, &this->maskCollider);
        Collider_UpdateSpheres(limbIndex, &this->bodyCollider);
        if ((limbIndex == HIPLOOP_LIMB_BODY) || (limbIndex == HIPLOOP_LIMB_FRONT_LEFT_LOWER_LEG) ||
            (limbIndex == HIPLOOP_LIMB_FRONT_RIGHT_LOWER_LEG) || (limbIndex == HIPLOOP_LIMB_LEFT_WING_MIDDLE) ||
            (limbIndex == HIPLOOP_LIMB_BACK_RIGHT_LOWER_LEG) || (limbIndex == HIPLOOP_LIMB_RIGHT_WING_MIDDLE) ||
            (limbIndex == HIPLOOP_LIMB_CENTER_WING_BASE) || (limbIndex == HIPLOOP_LIMB_CENTER_WING_MIDDLE) ||
            (limbIndex == HIPLOOP_LIMB_BACK_LEFT_LOWER_LEG) || (limbIndex == HIPLOOP_LIMB_RIGHT_EYE) ||
            (limbIndex == HIPLOOP_LIMB_LEFT_EYE)) {
            Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
            this->bodyPartIndex++;
            if (this->bodyPartIndex >= EN_PP_BODYPART_MAX) {
                this->bodyPartIndex = 0;
            }
        }
    }
}

void EnPp_Draw(Actor* thisx, PlayState* play) {
    EnPp* this = (EnPp*)thisx;
    MtxF mtxF;
    Vec3f pos;
    f32 scale;
    f32 alpha;

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnPp_OverrideLimbDraw, EnPp_PostLimbDraw, &this->actor);

    if (this->fragmentFlameCount != 0) {
        scale = 0.4f;
        if (EN_PP_GET_TYPE(&this->actor) == EN_PP_TYPE_FRAGMENT_BODY) {
            scale = 0.6f;
        }

        Actor_DrawDamageEffects(play, &this->actor, this->fragmentFlamesPos, this->fragmentFlameCount, scale, scale,
                                1.0f, ACTOR_DRAW_DMGEFF_BLUE_FIRE);
    }

    if (this->drawDmgEffTimer != 0) {
        alpha = this->drawDmgEffTimer * 0.05f;
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

        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EN_PP_BODYPART_MAX, this->drawDmgEffScale,
                                this->drawDmgEffFrozenSteamScale, alpha, this->drawDmgEffType);
    }

    if (this->floorPolyForCircleShadow != NULL) {
        if ((this->action != EN_PP_ACTION_MASK_DETACH) && (this->action < EN_PP_ACTION_DEAD)) {
            OPEN_DISPS(play->state.gfxCtx);

            Gfx_SetupDL44_Xlu(play->state.gfxCtx);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 255);

            Math_Vec3f_Copy(&pos, &this->actor.world.pos);
            pos.x += (Math_SinS(this->actor.world.rot.y) * -13.0f);
            pos.y = this->actor.floorHeight;
            pos.z += (Math_CosS(this->actor.world.rot.y) * -13.0f);
            func_800C0094(this->actor.floorPoly, pos.x, pos.y, pos.z, &mtxF);

            Matrix_Mult(&mtxF, MTXMODE_NEW);
            Matrix_Scale(0.5f, 1.0f, 0.5f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, gCircleShadowDL);

            CLOSE_DISPS();
        }
    }
}

OVL_INFO_NATIVE_ENEMY(0x1e9, En_Pp_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_PP, En_Pp_Profile, EnPp_Init, EnPp_Destroy, EnPp_Update, EnPp_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x1e9u, 0x1c6u, 0xdcfc8d6du };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_PP_ORIGIN, generatedNativeOrigin);
_Static_assert(__builtin_offsetof(EnPp, action) == 0x2c0, "Hiploop support phase guard");
