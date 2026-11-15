#include "mm_native_compat.h"
#define gBombBodyDL ((Gfx*)(gSegments[4] + 0x7860u))
#define gBombCapDL ((Gfx*)(gSegments[4] + 0x7a50u))
#define gEffSparkDL ((Gfx*)(gSegments[4] + 0x259f0u))
#define gElectricSpark1Tex 0x5b4e0u
#define gElectricSpark2Tex 0x5b6e0u
#define gElectricSpark3Tex 0x5b8e0u
#define gElectricSpark4Tex 0x5bae0u
#define gRealBombchuSpotAnim (*(AnimationHeader*)0x06000174)
#define gRealBombchuRunAnim (*(AnimationHeader*)0x0600026c)
#define gRealBombchuSkel (*(FlexSkeletonHeader*)0x06001858)

#include "types/en_rat.h"
#define TATL_HINT_ID_REAL_BOMBCHU 0xff
#undef ACTOR_EN_BOM
#define ACTOR_EN_BOM 0x9
#undef ACTOR_EN_RAT
#define ACTOR_EN_RAT 0x16f
#undef OBJECT_RAT
#define OBJECT_RAT 0x1181
#undef NA_SE_EN_BOMCHU_WALK
#define NA_SE_EN_BOMCHU_WALK 0x3828
#undef NA_SE_EN_BOMCHU_VOICE
#define NA_SE_EN_BOMCHU_VOICE 0x3854
#undef NA_SE_EN_BOMCHU_AIM
#define NA_SE_EN_BOMCHU_AIM 0x3855
#undef NA_SE_EN_BOMCHU_RUN
#define NA_SE_EN_BOMCHU_RUN 0x3856
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef Collider_InitAndSetSphere
#define Collider_InitAndSetSphere ((s32 (*)(struct PlayState* play, ColliderSphere* dest, struct Actor* actor, ColliderSphereInit* src))(void*)FOREIGN_ENEMY_API_SPHERE_INIT_SET)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef Collider_DestroySphere
#define Collider_DestroySphere ((s32 (*)(struct PlayState* play, ColliderSphere* sph))(void*)FOREIGN_ENEMY_API_SPHERE_DESTROY)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef Matrix_MultVecY
#define Matrix_MultVecY ((void (*)(f32 y, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Y)
#undef Matrix_MultVecX
#define Matrix_MultVecX ((void (*)(f32 x, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_X)
#undef Matrix_RotateAxisF
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_AXIS[];
#define Matrix_RotateAxisF ((void (*)(f32 angle, Vec3f* axis, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_AXIS)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZYX)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Effect_Add
#define Effect_Add ((void (*)(struct PlayState* play, s32* pIndex, EffectType type, u8 arg3, u8 arg4, void* initParams))0x8001aa5cu)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef Effect_Destroy
#define Effect_Destroy ((void (*)(struct PlayState* play, s32 index))0x8001ae04u)
#undef Math_FAcosF
#define Math_FAcosF ((f32 (*)(f32 x))0x800cd890u)
#undef Math3D_Vec3f_Cross
#define Math3D_Vec3f_Cross ((void (*)(Vec3f* a, Vec3f* b, Vec3f* ret))0x800a56b8u)
#undef Math3D_Vec3fMagnitude
#define Math3D_Vec3fMagnitude ((f32 (*)(Vec3f* vec))0x800a5510u)
#undef Math_Vec3f_Scale
#define Math_Vec3f_Scale ((void (*)(Vec3f* vec, f32 scale))0x80063ddcu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Matrix_MtxFToYXZRot
#define Matrix_MtxFToYXZRot ((void (*)(MtxF* src, Vec3s* dest, s32 nonUniformScale))0x800abf50u)
#undef Actor_WorldDistXYZToPoint
#define Actor_WorldDistXYZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x800214d8u)
#undef Math_Vec3f_Sum
#define Math_Vec3f_Sum ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d10u)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId))0x800308b4u)
#undef SurfaceType_GetWallFlags
#define SurfaceType_GetWallFlags ((s32 (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x8003444cu)
#undef EffectSsGRipple_Spawn
#define EffectSsGRipple_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life))0x8001cf3cu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef DynaPolyActor_TransformCarriedActor
#define DynaPolyActor_TransformCarriedActor ((u32 (*)(struct CollisionContext* colCtx, s32 bgId, Actor* carriedActor))0x80035180u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef func_800B0EB0
#define func_800B0EB0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life))0x8001be64u)
#undef Animation_PlayLoopSetSpeed
#define Animation_PlayLoopSetSpeed ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed))0x8008d360u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef func_800B1210
#define func_800B1210 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep))0x8001c1c4u)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d2d4u)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 angle))0x80021a28u)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Effect_GetByIndex
#define Effect_GetByIndex ((void* (*)(s32 index))0x8001a890u)
#undef EffectBlure_AddVertex
#define EffectBlure_AddVertex ((void (*)(EffectBlure* this, Vec3f* p1, Vec3f* p2))0x80013f30u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Actor_MoveWithoutGravity
#define Actor_MoveWithoutGravity ((void (*)(Actor* actor))0x800212b8u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef SurfaceType_IsWallDamage
#define SurfaceType_IsWallDamage ((u32 (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x800347c8u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x800abe54u)
#undef Matrix_GetCurrent
#define Matrix_GetCurrent ((MtxF* (*)(void))0x800aa78cu)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 rad))0x800a4650u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetupDL60_XluNoCD
#define Gfx_SetupDL60_XluNoCD ((void (*)(GraphicsContext* gfxCtx))0x8007e930u)
#undef func_800B8050
#define func_800B8050 ((void (*)(Actor* actor, struct PlayState* play, s32 flag))0x80022438u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR)

void EnRat_Init(Actor* thisx, PlayState* play);
void EnRat_Destroy(Actor* thisx, PlayState* play);
void EnRat_Update(Actor* thisx, PlayState* play);
void EnRat_Draw(Actor* thisx, PlayState* play);

void EnRat_InitializeAxes(EnRat* this);
void EnRat_UpdateRotation(EnRat* this);
void EnRat_Revive(EnRat* this, PlayState* play);
void EnRat_SetupIdle(EnRat* this);
void EnRat_Idle(EnRat* this, PlayState* play);
void EnRat_SetupSpottedPlayer(EnRat* this);
void EnRat_SpottedPlayer(EnRat* this, PlayState* play);
void EnRat_SetupChasePlayer(EnRat* this);
void EnRat_ChasePlayer(EnRat* this, PlayState* play);
void EnRat_Bounced(EnRat* this, PlayState* play);
void EnRat_Explode(EnRat* this, PlayState* play);
void EnRat_PostDetonation(EnRat* this, PlayState* play);

typedef enum { EN_RAT_HOOK_STARTED = -2, EN_RAT_HOOKED } EnRatHookedState;

ActorProfile En_Rat_Profile = {
    0xd6f, ACTORCAT_ENEMY, FLAGS, 0x1181, sizeof(EnRat), EnRat_Init, EnRat_Destroy, EnRat_Update, EnRat_Draw,
};

static ColliderSphereInit sSphereInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x08 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_HARD,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 1, { { 0, 0, 0 }, 23 }, 100 },
};

typedef enum { EN_RAT_DMGEFF_NONE, EN_RAT_DMGEFF_STUN, EN_RAT_DMGEFF_HOOKSHOT = 0xF } EnRatDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_RAT_DMGEFF_STUN), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(1, EN_RAT_DMGEFF_NONE), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(0, EN_RAT_DMGEFF_NONE), DMG_ENTRY(0, EN_RAT_DMGEFF_HOOKSHOT), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(1, EN_RAT_DMGEFF_NONE), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(2, EN_RAT_DMGEFF_NONE), DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(1, EN_RAT_DMGEFF_NONE), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(2, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(0, EN_RAT_DMGEFF_STUN), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(0, EN_RAT_DMGEFF_NONE), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(1, EN_RAT_DMGEFF_NONE), DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(0, EN_RAT_DMGEFF_NONE), DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),     DMG_ENTRY(0, EN_RAT_DMGEFF_NONE),
    DMG_ENTRY(0, EN_RAT_DMGEFF_NONE), DMG_ENTRY(1, EN_RAT_DMGEFF_NONE),
} };

static CollisionCheckInfoInit sColChkInfoInit = { 1, 30, 30, 50 };

static const u32 sSparkTextures[] = {
    gElectricSpark1Tex,
    gElectricSpark2Tex,
    gElectricSpark3Tex,
    gElectricSpark4Tex,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_REAL_BOMBCHU, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5000, ICHAIN_STOP),
};

static EffectBlureInit2 sBlureInit = {
    0,
    0,
    0,
    { 250, 0, 0, 250 },
    { 200, 0, 0, 130 },
    { 150, 0, 0, 100 },
    { 100, 0, 0, 50 },
    16,
    0,
    EFF_BLURE_DRAW_MODE_SIMPLE,
    0,
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
};

void EnRat_Init(Actor* thisx, PlayState* play) {
    EnRat* this = (EnRat*)thisx;
    s32 attackRange;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitAndSetSphere(play, &this->collider, &this->actor, &sSphereInit);
    this->collider.dim.worldSphere.radius = sSphereInit.dim.modelSphere.radius;

    attackRange = EN_RAT_GET_ATTACK_RANGE(&this->actor);
    if (EN_RAT_IS_OVERWORLD_TYPE(&this->actor)) {
        this->actor.params = EN_RAT_TYPE_OVERWORLD;
    } else {
        this->actor.params = EN_RAT_TYPE_DUNGEON;
    }

    SkelAnime_InitFlex(play, &this->skelAnime, &gRealBombchuSkel, &gRealBombchuRunAnim, this->jointTable,
                       this->morphTable, REAL_BOMBCHU_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        Effect_Add(play, &this->blure1Index, EFFECT_BLURE2, 0, 0, &sBlureInit);
        Effect_Add(play, &this->blure2Index, EFFECT_BLURE2, 0, 0, &sBlureInit);
        this->timer = 30;
    } else {
        this->timer = 150;
    }

    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    EnRat_InitializeAxes(this);
    EnRat_UpdateRotation(this);

    if ((attackRange == 255) || (attackRange == 0)) {
        this->attackRange = 350.0f;
    } else if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        this->attackRange = attackRange * 0.1f * 40.0f;
    } else {
        this->attackRange = attackRange * 0.5f * 40.0f;
    }

    EnRat_SetupIdle(this);
}

void EnRat_Destroy(Actor* thisx, PlayState* play) {
    EnRat* this = (EnRat*)thisx;

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        Effect_Destroy(play, this->blure1Index);
        Effect_Destroy(play, this->blure2Index);
    }

    Collider_DestroySphere(play, &this->collider);
}

void EnRat_InitializeAxes(EnRat* this) {
    Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_NEW);
    Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
    Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);

    Matrix_MultVecZ(1.0f, &this->axisForwards);
    Matrix_MultVecY(1.0f, &this->axisUp);
    Matrix_MultVecX(1.0f, &this->axisLeft);
}

s32 EnRat_UpdateFloorPoly(EnRat* this, CollisionPoly* floorPoly, PlayState* play) {
    Vec3f normal;
    Vec3f vec;
    f32 angle;
    f32 magnitude;
    f32 normDotUp;

    this->actor.floorPoly = floorPoly;

    if (floorPoly != NULL) {
        normal.x = COLPOLY_GET_NORMAL(floorPoly->normal.x);
        normal.y = COLPOLY_GET_NORMAL(floorPoly->normal.y);
        normal.z = COLPOLY_GET_NORMAL(floorPoly->normal.z);
    } else {
        normal.x = 0.0f;
        normal.z = 0.0f;
        normal.y = 1.0f;
    }

    normDotUp = DOTXYZ(normal, this->axisUp);
    if (fabsf(normDotUp) >= 0.999f) {
        return false;
    }

    angle = Math_FAcosF(normDotUp);
    if (angle < 0.001f) {
        return false;
    }

    Math3D_Vec3f_Cross(&this->axisUp, &normal, &vec);

    magnitude = Math3D_Vec3fMagnitude(&vec);
    if (magnitude < 0.001f) {
        EnRat_Explode(this, play);
        return false;
    }

    Math_Vec3f_Scale(&vec, 1.0f / magnitude);
    Matrix_RotateAxisF(angle, &vec, MTXMODE_NEW);
    Matrix_MultVec3f(&this->axisLeft, &vec);
    Math_Vec3f_Copy(&this->axisLeft, &vec);
    Math3D_Vec3f_Cross(&this->axisLeft, &normal, &this->axisForwards);

    magnitude = Math3D_Vec3fMagnitude(&this->axisForwards);
    if (magnitude < 0.001f) {
        EnRat_Explode(this, play);
        return false;
    }

    Math_Vec3f_Scale(&this->axisForwards, 1.0f / magnitude);
    Math_Vec3f_Copy(&this->axisUp, &normal);
    return true;
}

void EnRat_UpdateRotation(EnRat* this) {
    MtxF mf;

    mf.xx = this->axisLeft.x;
    mf.yx = this->axisLeft.y;
    mf.zx = this->axisLeft.z;

    mf.xy = this->axisUp.x;
    mf.yy = this->axisUp.y;
    mf.zy = this->axisUp.z;

    mf.xz = this->axisForwards.x;
    mf.yz = this->axisForwards.y;
    mf.zz = this->axisForwards.z;

    Matrix_MtxFToYXZRot(&mf, &this->actor.world.rot, false);
    this->actor.world.rot.x = -this->actor.world.rot.x;
}

void EnRat_ChooseDirection(EnRat* this) {
    Vec3f newAxisForwards;
    s16 angle;

    if (this->actionFunc != EnRat_Idle) {
        angle = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        if (this->axisUp.y < -0.25f) {
            angle -= 0x8000;
        }
    } else {
        if (Actor_WorldDistXYZToPoint(&this->actor, &this->actor.home.pos) > 50.0f) {
            Vec3f homeInHome;
            Vec3f worldInHome;
            Vec3f worldPlusForwardInHome;
            Vec3f upInHome;

            Matrix_RotateZS(-this->actor.home.rot.z, MTXMODE_NEW);
            Matrix_RotateXS(-this->actor.home.rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(-this->actor.home.rot.y, MTXMODE_APPLY);

            Matrix_MultVec3f(&this->axisUp, &upInHome);

            Math_Vec3f_Sum(&this->actor.world.pos, &this->axisForwards, &homeInHome);

            Matrix_MultVec3f(&homeInHome, &worldPlusForwardInHome);

            Matrix_MultVec3f(&this->actor.home.pos, &homeInHome);

            Matrix_MultVec3f(&this->actor.world.pos, &worldInHome);

            angle = Math_Vec3f_Yaw(&worldInHome, &homeInHome) - Math_Vec3f_Yaw(&worldInHome, &worldPlusForwardInHome);
            if (upInHome.y < -0.25f) {
                angle -= 0x8000;
            }

            angle += TRUNCF_BINANG(Rand_CenteredFloat(0x800));
        } else {
            angle = (Rand_ZeroOne() < 0.1f) ? TRUNCF_BINANG(Rand_CenteredFloat(0x800)) : 0;
        }
    }

    angle = CLAMP(angle, -0x800, 0x800);
    Matrix_RotateAxisF(BINANG_TO_RAD(angle), &this->axisUp, MTXMODE_NEW);
    Matrix_MultVec3f(&this->axisForwards, &newAxisForwards);
    Math_Vec3f_Copy(&this->axisForwards, &newAxisForwards);
    Math3D_Vec3f_Cross(&this->axisUp, &this->axisForwards, &this->axisLeft);
    this->shouldRotateOntoSurfaces = true;
}

s32 EnRat_IsOnCollisionPoly(PlayState* play, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                            s32* bgId) {
    WaterBox* waterBox;
    s32 isOnWater;
    f32 waterSurface;

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, posB->x, posB->z, &waterSurface, &waterBox) &&
        (waterSurface <= posA->y) && (posB->y <= waterSurface)) {
        isOnWater = true;
    } else {
        isOnWater = false;
    }

    if (BgCheck_EntityLineTest1(&play->colCtx, posA, posB, posResult, poly, 1, 1, 1, 1, bgId)) {
        if (!(SurfaceType_GetWallFlags(&play->colCtx, *poly, *bgId) & (WALL_FLAG_4 | WALL_FLAG_5)) &&
            (!isOnWater || (waterSurface <= posResult->y))) {
            return true;
        }
    }

    if (isOnWater) {
        posResult->x = posB->x;
        posResult->y = waterSurface;
        posResult->z = posB->z;
        *poly = NULL;
        *bgId = BGCHECK_SCENE;
        return true;
    }

    return false;
}

s32 EnRat_IsTouchingSurface(EnRat* this, PlayState* play) {
    CollisionPoly* polySide = NULL;
    CollisionPoly* polyUpDown = NULL;
    s32 bgIdSide;
    s32 bgIdUpDown;
    s32 i;
    f32 lineLength;
    Vec3f posA;
    Vec3f posB;
    Vec3f posSide;
    Vec3f posUpDown;

    bgIdUpDown = bgIdSide = BGCHECK_SCENE;

    lineLength = 2.0f * this->actor.speed;

    posA.x = this->actor.world.pos.x + (this->axisUp.x * 5.0f);
    posA.y = this->actor.world.pos.y + (this->axisUp.y * 5.0f);
    posA.z = this->actor.world.pos.z + (this->axisUp.z * 5.0f);

    posB.x = this->actor.world.pos.x - (this->axisUp.x * 4.0f);
    posB.y = this->actor.world.pos.y - (this->axisUp.y * 4.0f);
    posB.z = this->actor.world.pos.z - (this->axisUp.z * 4.0f);

    if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posUpDown, &polyUpDown, &bgIdUpDown)) {
        posB.x = (this->axisForwards.x * lineLength) + posA.x;
        posB.y = (this->axisForwards.y * lineLength) + posA.y;
        posB.z = (this->axisForwards.z * lineLength) + posA.z;

        if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posSide, &polySide, &bgIdSide)) {
            if ((polySide != NULL) && this->hasLostTrackOfPlayer) {
                return false;
            }

            this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polySide, play);
            Math_Vec3f_Copy(&this->actor.world.pos, &posSide);
            this->actor.floorBgId = bgIdSide;
            this->actor.speed = 0.0f;
        } else {
            if (polyUpDown != this->actor.floorPoly) {
                this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polyUpDown, play);
            }

            Math_Vec3f_Copy(&this->actor.world.pos, &posUpDown);
            this->actor.floorBgId = bgIdUpDown;
        }
    } else {
        this->actor.speed = 0.0f;
        lineLength *= 3.0f;
        Math_Vec3f_Copy(&posA, &posB);

        for (i = 0; i < 3; i++) {
            if (i == 0) {
                posB.x = posA.x - (this->axisForwards.x * lineLength);
                posB.y = posA.y - (this->axisForwards.y * lineLength);
                posB.z = posA.z - (this->axisForwards.z * lineLength);
            } else if (i == 1) {
                posB.x = posA.x + (this->axisLeft.x * lineLength);
                posB.y = posA.y + (this->axisLeft.y * lineLength);
                posB.z = posA.z + (this->axisLeft.z * lineLength);
            } else {
                posB.x = posA.x - (this->axisLeft.x * lineLength);
                posB.y = posA.y - (this->axisLeft.y * lineLength);
                posB.z = posA.z - (this->axisLeft.z * lineLength);
            }

            if (EnRat_IsOnCollisionPoly(play, &posA, &posB, &posSide, &polySide, &bgIdSide)) {
                this->shouldRotateOntoSurfaces |= EnRat_UpdateFloorPoly(this, polySide, play);
                Math_Vec3f_Copy(&this->actor.world.pos, &posSide);
                this->actor.floorBgId = bgIdSide;
                break;
            }
        }

        if (i == 3) {
            return false;
        }
    }

    return true;
}

void EnRat_ActorCoordsToWorld(EnRat* this, Vec3f* offset, Vec3f* pos) {
    f32 x = offset->x + this->visualJitter;

    pos->x = this->actor.world.pos.x + (this->axisLeft.x * x) + (this->axisUp.x * offset->y) +
             (this->axisForwards.x * offset->z);
    pos->y = this->actor.world.pos.y + (this->axisLeft.y * x) + (this->axisUp.y * offset->y) +
             (this->axisForwards.y * offset->z);
    pos->z = this->actor.world.pos.z + (this->axisLeft.z * x) + (this->axisUp.z * offset->y) +
             (this->axisForwards.z * offset->z);
}

void EnRat_SpawnWaterEffects(EnRat* this, PlayState* play) {
    Vec3f splashPos;

    EffectSsGRipple_Spawn(play, &this->actor.world.pos, 70, 500, 0);
    splashPos.x = this->actor.world.pos.x - (this->axisForwards.x * 10.0f);
    splashPos.z = this->actor.world.pos.z - (this->axisForwards.z * 10.0f);
    splashPos.y = this->actor.world.pos.y + 5.0f;
    EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 1, 450);
}

void EnRat_HandleNonSceneCollision(EnRat* this, PlayState* play) {
    s16 yaw = this->actor.shape.rot.y;
    f32 sin;
    f32 cos;
    f32 tempX;

    DynaPolyActor_TransformCarriedActor(&play->colCtx, this->actor.floorBgId, &this->actor);

    if (yaw != this->actor.shape.rot.y) {
        yaw = this->actor.shape.rot.y - yaw;

        sin = Math_SinS(yaw);
        cos = Math_CosS(yaw);

        tempX = this->axisForwards.x;
        this->axisForwards.x = (sin * this->axisForwards.z) + (cos * tempX);
        this->axisForwards.z = (cos * this->axisForwards.z) - (sin * tempX);

        tempX = this->axisUp.x;
        this->axisUp.x = (sin * this->axisUp.z) + (cos * tempX);
        this->axisUp.z = (cos * this->axisUp.z) - (sin * tempX);

        tempX = this->axisLeft.x;
        this->axisLeft.x = (sin * this->axisLeft.z) + (cos * tempX);
        this->axisLeft.z = (cos * this->axisLeft.z) - (sin * tempX);
    }
}

static Vec3f sSmokeAccel = { 0.0f, 0.6f, 0.0f };
static Color_RGBA8 sSmokeColor = { 255, 255, 255, 255 };

void EnRat_SpawnSmoke(EnRat* this, PlayState* play) {
    func_800B0EB0(play, &this->smokePos, &gZeroVec3f, &sSmokeAccel, &sSmokeColor, &sSmokeColor, 75, 7, 8);
}

void EnRat_SetupRevive(EnRat* this) {
    this->hasLostTrackOfPlayer = false;
    this->timer = 200;
    this->damageReaction.stunTimer = 0;
    Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
    this->actor.shape.yOffset = 0.0f;
    this->actor.shape.rot.x = this->actor.home.rot.x;
    this->actor.shape.rot.y = this->actor.home.rot.y;
    this->actor.shape.rot.z = this->actor.home.rot.z;
    EnRat_InitializeAxes(this);
    EnRat_UpdateRotation(this);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.speed = 0.0f;
    Animation_PlayLoopSetSpeed(&this->skelAnime, &gRealBombchuSpotAnim, 0.0f);
    this->revivePosY = 2666.6667f;
    this->actor.draw = NULL;
    this->actor.shape.shadowDraw = NULL;
    this->actionFunc = EnRat_Revive;
}

void EnRat_Revive(EnRat* this, PlayState* play) {
    if (this->timer > 0) {
        this->timer--;
        if (this->timer == 0) {
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            this->actor.draw = EnRat_Draw;
            this->skelAnime.playSpeed = 1.0f;
        }
    } else {
        Math_StepToF(&this->revivePosY, 0.0f, 666.6667f);
        if (Animation_OnFrame(&this->skelAnime, 2.0f)) {
            func_800B1210(play, &this->actor.world.pos, &this->axisUp, &gZeroVec3f, 600, 100);
            this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        }

        if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
            this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
            this->timer = 150;
            EnRat_SetupIdle(this);
        }
    }
}

void EnRat_SetupIdle(EnRat* this) {
    Animation_PlayLoop(&this->skelAnime, &gRealBombchuRunAnim);
    this->animLoopCounter = 5;
    this->actor.speed = 2.0f;
    this->actionFunc = EnRat_Idle;
}

void EnRat_Idle(EnRat* this, PlayState* play) {

    this->actor.speed = 2.0f;
    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BOMCHU_WALK);
        if (this->animLoopCounter != 0) {
            this->animLoopCounter--;
        }
    }

    if ((this->animLoopCounter == 0) && (Rand_ZeroOne() < 0.05f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BOMCHU_VOICE);
        this->animLoopCounter = 5;
    }

    if (!(0) && (this->actor.xzDistToPlayer < this->attackRange) && (Player_GetMask(play) != PLAYER_MASK_STONE) &&
        Actor_IsFacingPlayer(&this->actor, 0x3800)) {
        EnRat_SetupSpottedPlayer(this);
    }
}

void EnRat_SetupSpottedPlayer(EnRat* this) {
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    Animation_MorphToLoop(&this->skelAnime, &gRealBombchuSpotAnim, -5.0f);
    this->animLoopCounter = 3;
    this->actor.speed = 0.0f;
    this->actionFunc = EnRat_SpottedPlayer;
}

void EnRat_SpottedPlayer(EnRat* this, PlayState* play) {
    if ((this->animLoopCounter == 3) && Animation_OnFrame(&this->skelAnime, 5.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BOMCHU_AIM);
    }

    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        this->animLoopCounter--;
        if (this->animLoopCounter == 0) {
            EnRat_SetupChasePlayer(this);
        }
    }
}

void EnRat_UpdateSparkOffsets(EnRat* this) {
    s32 i;
    Vec3f* ptr;

    for (i = 0; i < ARRAY_COUNT(this->sparkOffsets); i++) {
        ptr = &this->sparkOffsets[i];
        ptr->x = Rand_CenteredFloat(6.0f);
        ptr->y = Rand_CenteredFloat(6.0f);
        ptr->z = Rand_CenteredFloat(6.0f);
    }
}

void EnRat_SetupChasePlayer(EnRat* this) {
    Animation_MorphToLoop(&this->skelAnime, &gRealBombchuRunAnim, -5.0f);
    this->actor.speed = 6.1f;
    EnRat_UpdateSparkOffsets(this);
    this->actionFunc = EnRat_ChasePlayer;
}

static Vec3f sBlureP1Offset = { 0.0f, 10.5f, -9.0f };
static Vec3f sBlureP2LeftOffset = { 18.0f, 0.0f, -7.5f };
static Vec3f sBlureP2RightOffset = { -18.0f, 0.0f, -7.5f };
static Vec3f sDustVelocity = { 0.0f, 3.0f, 0.0f };

void EnRat_ChasePlayer(EnRat* this, PlayState* play) {
    Vec3f blureP1;
    Vec3f blureP2;

    this->actor.speed = 6.1f;
    if (this->hasLostTrackOfPlayer) {
        if (!(0) && (Player_GetMask(play) != PLAYER_MASK_STONE) && Actor_IsFacingPlayer(&this->actor, 0x3000)) {
            this->hasLostTrackOfPlayer = false;
        }
    } else if ((0) || (Player_GetMask(play) == PLAYER_MASK_STONE)) {
        this->hasLostTrackOfPlayer = true;
    }

    if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
        if (this->animLoopCounter != 0) {
            this->animLoopCounter--;
        }

        Actor_PlaySfx(&this->actor, NA_SE_EN_BOMCHU_WALK);
    }

    DECR(this->timer);

    if ((this->timer == 0) && (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON)) {
        this->timer = 30;
    }

    EnRat_SpawnSmoke(this, play);
    this->visualJitter =
        (5.0f + (Rand_ZeroOne() * 3.0f)) * Math_SinS(((Rand_ZeroOne() * 0x200) + 0x3000) * this->timer);

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
        EnRat_ActorCoordsToWorld(this, &sBlureP1Offset, &blureP1);

        EnRat_ActorCoordsToWorld(this, &sBlureP2LeftOffset, &blureP2);
        EffectBlure_AddVertex(Effect_GetByIndex(this->blure1Index), &blureP1, &blureP2);

        EnRat_ActorCoordsToWorld(this, &sBlureP2RightOffset, &blureP2);
        EffectBlure_AddVertex(Effect_GetByIndex(this->blure2Index), &blureP1, &blureP2);
    } else if ((this->actor.floorPoly != NULL) && ((play->gameplayFrames % 4) == 0)) {
        func_800B1210(play, &this->actor.world.pos, &sDustVelocity, &gZeroVec3f, 550, 50);
    }

    if ((this->actor.floorPoly == NULL) && Animation_OnFrame(&this->skelAnime, 0.0f)) {
        EnRat_SpawnWaterEffects(this, play);
    }

    if ((this->animLoopCounter == 0) && (Rand_ZeroOne() < 0.05f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BOMCHU_AIM);
        this->animLoopCounter = 5;
    }

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_BOMCHU_RUN - SFX_FLAG);
    EnRat_UpdateSparkOffsets(this);
}

void EnRat_SetupBounced(EnRat* this) {
    this->actor.speed = 5.0f;
    this->actor.velocity.y = 8.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer + 0x8000;
    this->actor.gravity = -1.0f;
    EnRat_UpdateSparkOffsets(this);
    this->actor.bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
    this->actionFunc = EnRat_Bounced;
}

void EnRat_Bounced(EnRat* this, PlayState* play) {
    this->actor.shape.rot.x -= 0x700;
    Math_StepToF(&this->actor.shape.yOffset, 1700.0f, 170.0f);
    this->timer--;
    if (this->timer == 0) {
        this->timer = 30;
    }

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING)) {
        EnRat_Explode(this, play);
    }
}

void EnRat_Explode(EnRat* this, PlayState* play) {
    EnBom* bomb =
        (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, BOMB_EXPLOSIVE_TYPE_BOMB, 0, 0, BOMB_TYPE_BODY);

    if (bomb != NULL) {
        bomb->timer = 0;
    }

    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_OVERWORLD) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x100);
    }

    this->actor.speed = 0.0f;
    this->actionFunc = EnRat_PostDetonation;
}

void EnRat_PostDetonation(EnRat* this, PlayState* play) {
    if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_OVERWORLD) {
        EnRat_SetupRevive(this);
    } else {
        Actor_Kill(&this->actor);
    }
}

void EnRat_Update(Actor* thisx, PlayState* play) {
    EnRat* this = (EnRat*)thisx;

    this->shouldRotateOntoSurfaces = false;
    if (this->damageReaction.stunTimer == 0) {
        SkelAnime_Update(&this->skelAnime);
    }

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.ocFlags1 &= ~OC1_HIT;
        if (this->collider.base.atFlags & AT_BOUNCED) {
            EnRat_SetupBounced(this);
        } else {
            EnRat_Explode(this, play);
            return;
        }
    } else if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        if (this->actor.colChkInfo.damageEffect == EN_RAT_DMGEFF_HOOKSHOT) {
            this->damageReaction.hookedState = EN_RAT_HOOK_STARTED;
        } else if (this->actor.colChkInfo.damageEffect == EN_RAT_DMGEFF_STUN) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 40);
            if (this->actionFunc == EnRat_Bounced) {
                this->actor.speed = 0.0f;
                if (this->actor.velocity.y > 0.0f) {
                    this->actor.velocity.y = 0.0f;
                }
            } else {
                this->damageReaction.stunTimer = 40;
            }
        } else {
            EnRat_Explode(this, play);
            return;
        }
    } else if (((this->collider.base.ocFlags1 & OC1_HIT) && (((this->collider.base.oc->category == ACTORCAT_ENEMY)) ||
                                                             (this->collider.base.oc->category == ACTORCAT_BOSS) ||
                                                             (this->collider.base.oc->category == ACTORCAT_PLAYER))) ||
               ((this->actionFunc == EnRat_ChasePlayer) && (this->timer == 0))) {
        this->collider.base.ocFlags1 &= ~OC1_HIT;
        EnRat_Explode(this, play);
        return;
    }

    this->actionFunc(this, play);

    if ((this->actionFunc != EnRat_PostDetonation) && (this->actionFunc != EnRat_Revive)) {
        if (this->damageReaction.stunTimer > 0) {
            this->damageReaction.stunTimer--;
        } else if (this->damageReaction.hookedState < 0) {
            if (this->damageReaction.hookedState == EN_RAT_HOOK_STARTED) {
                this->damageReaction.hookedState = EN_RAT_HOOKED;
            } else if (!CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_HOOKSHOT_ATTACHED)) {
                EnRat_Explode(this, play);
                return;
            }
        } else if (this->actionFunc != EnRat_Bounced) {
            if (this->actor.floorBgId != BGCHECK_SCENE) {
                EnRat_HandleNonSceneCollision(this, play);
            }

            if (!this->hasLostTrackOfPlayer) {
                EnRat_ChooseDirection(this);
            }

            if ((this->actionFunc != EnRat_SpottedPlayer) && !EnRat_IsTouchingSurface(this, play)) {
                EnRat_Explode(this, play);
                return;
            }

            if (this->shouldRotateOntoSurfaces) {
                EnRat_UpdateRotation(this);
                this->actor.shape.rot.x = -this->actor.world.rot.x;
                this->actor.shape.rot.y = this->actor.world.rot.y;
                this->actor.shape.rot.z = this->actor.world.rot.z;
            }

            Actor_MoveWithoutGravity(&this->actor);
            this->actor.floorHeight = this->actor.world.pos.y;
        } else {
            Actor_MoveWithGravity(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 30.0f, 60.0f,
                                    UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4);
        }

        if (SurfaceType_IsWallDamage(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId)) {
            EnRat_Explode(this, play);
            return;
        }

        this->collider.dim.worldSphere.center.x = this->actor.world.pos.x + (this->axisUp.x * 10.0f);
        this->collider.dim.worldSphere.center.y = this->actor.world.pos.y + (this->axisUp.y * 10.0f);
        this->collider.dim.worldSphere.center.z = this->actor.world.pos.z + (this->axisUp.z * 10.0f);

        if (this->actionFunc != EnRat_Revive) {
            if (this->damageReaction.stunTimer == 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
            }

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }

        Actor_SetFocus(&this->actor, this->actor.shape.yOffset * 0.015f);
    }
}

s32 EnRat_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnRat* this = (EnRat*)thisx;

    if (limbIndex == REAL_BOMBCHU_LIMB_BODY) {
        pos->y -= this->revivePosY;
    }

    if (limbIndex == REAL_BOMBCHU_LIMB_TAIL_END) {
        *dList = NULL;
    }

    return false;
}

void EnRat_PostLimbDraw(PlayState* play2, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    PlayState* play = play2;
    EnRat* this = (EnRat*)thisx;
    MtxF* currentMatrixState;
    Vec3f* ptr;
    f32 redModifier;

    if (limbIndex == REAL_BOMBCHU_LIMB_TAIL_END) {
        OPEN_DISPS(play->state.gfxCtx);

        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_MultZero(&this->smokePos);
        this->smokePos.y += 15.0f;
        currentMatrixState = Matrix_GetCurrent();

        if (this->actionFunc == EnRat_ChasePlayer) {
            s32 i;

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 150, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
            Matrix_Scale(45.0f, 45.0f, 45.0f, MTXMODE_APPLY);

            for (i = 0; i < ARRAY_COUNT(this->sparkOffsets); i++) {
                ptr = &this->sparkOffsets[i];
                currentMatrixState->mf[3][0] = this->smokePos.x + ptr->x;
                currentMatrixState->mf[3][1] = this->smokePos.y + ptr->y;
                currentMatrixState->mf[3][2] = this->smokePos.z + ptr->z;
                MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
                gSPSegment(POLY_XLU_DISP++, 0x08,
                           (void*)(gSegments[4] + sSparkTextures[(play->gameplayFrames + i) & 3]));
                gSPDisplayList(POLY_XLU_DISP++, gEffSparkDL);
            }

            Matrix_Scale(1.0f / 45.0f, 1.0f / 45.0f, 1.0f / 45.0f, MTXMODE_APPLY);
            currentMatrixState->mf[3][0] = this->smokePos.x;
            currentMatrixState->mf[3][1] = this->smokePos.y - 15.0f;
            currentMatrixState->mf[3][2] = this->smokePos.z;
        }

        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gBombCapDL);
        if (EN_RAT_GET_TYPE(&this->actor) == EN_RAT_TYPE_DUNGEON) {
            redModifier = fabsf(Math_CosF(this->timer * (M_PIf / 30)));
        } else {
            if (this->timer >= 120) {
                redModifier = fabsf(Math_CosF((this->timer % 30) * (M_PIf / 30)));
            } else if (this->timer >= 30) {
                redModifier = fabsf(Math_CosF((this->timer % 6) * (M_PIf / 6)));
            } else {
                redModifier = fabsf(Math_CosF((this->timer % 3) * (M_PIf / 3)));
            }
        }

        gDPSetEnvColor(POLY_OPA_DISP++, (s32)((1.0f - redModifier) * 255.0f), 0, 40, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, (s32)((1.0f - redModifier) * 255.0f), 0, 40, 255);
        Matrix_RotateZYX(0x4000, 0, 0, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gBombBodyDL);

        CLOSE_DISPS();
    }
}

void EnRat_Draw(Actor* thisx, PlayState* play) {
    EnRat* this = (EnRat*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL60_XluNoCD(play->state.gfxCtx);
    func_800B8050(&this->actor, play, 0);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnRat_OverrideLimbDraw, EnRat_PostLimbDraw, &this->actor);
}

OVL_INFO_NATIVE_ENEMY(0x16f, En_Rat_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_RAT, En_Rat_Profile, EnRat_Init, EnRat_Destroy, EnRat_Update, EnRat_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x16fu, 0x181u, 0x644fa34eu };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_RAT_ORIGIN, generatedNativeOrigin);
static const u32 generatedNativeDefeat[] = { __builtin_offsetof(EnRat, actionFunc), (u32)EnRat_PostDetonation,
                                             (u32)EnRat_Revive };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_RAT_DEFEAT, generatedNativeDefeat);
