#include "mm_native_compat.h"
#include <combo/enemy_native.h>
#define OBJECT_DODONGO_LIMB_01 1
#define OBJECT_DODONGO_LIMB_07 7
#define OBJECT_DODONGO_LIMB_0D 13
#define OBJECT_DODONGO_LIMB_0F 15
#define OBJECT_DODONGO_LIMB_10 16
#define OBJECT_DODONGO_LIMB_1E 30
#define OBJECT_DODONGO_LIMB_MAX 31
#include "types/en_dodongo.h"
#define TATL_HINT_ID_DODONGO 0x0d
#define EFFECT_BLURE_ELEMENT_FLAG_8 8
#define EFFECT_HITMARK_WHITE 0
#define EFFECT_HITMARK_METAL 3
#define CollisionCheck_SpawnShieldParticlesMetalSound ((void (*)(PlayState*,Vec3f*,Vec3f*))0x80051204)
static Vec3f dodongoOneVec = { 1, 1, 1 };
#define gOneVec3f dodongoOneVec
extern void FOREIGN_ENEMY_API_DODONGO_FIRE(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16, s16, s16, s32);
#define EffectSsDFire_Spawn FOREIGN_ENEMY_API_DODONGO_FIRE

#include <combo/enemy_actor_layout.h>
typedef struct {
    Actor actor;
    ColliderCylinder bombCollider;
    ColliderJntSph explosionCollider;
    ColliderJntSphElement explosionColliderElements[1];
    s16 timer;
} EnBombf;
_Static_assert(__builtin_offsetof(EnBombf, timer) == 0x1e8, "receiving Bomb Flower timer");

#undef ACTOR_EN_DODONGO
#define ACTOR_EN_DODONGO 0xc0b
#undef OBJECT_DODONGO
#define OBJECT_DODONGO 0x100a
#undef ActorShadow_DrawCircle
#define ActorShadow_DrawCircle ((void (*)(Actor* actor, struct Lights* lights, struct PlayState* play))0x8001eff4u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d21cu)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef CollisionCheck_BlueBlood
#define CollisionCheck_BlueBlood ((void (*)(struct PlayState* play, Collider* collider, Vec3f* v))0x8004c430u)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef EffectBlure_AddVertex
static inline void dodongoBlureVertex(EffectBlure* effect, Vec3f* a, Vec3f* b) {
    if (effect)
        ((void (*)(EffectBlure*, Vec3f*, Vec3f*))0x80013f30)(effect, a, b);
}
#define EffectBlure_AddVertex dodongoBlureVertex
#undef EffectSsHitmark_SpawnFixedScale
#define EffectSsHitmark_SpawnFixedScale ((void (*)(struct PlayState* play, s32 type, Vec3f* pos))0x8001d7a8u)
#undef Effect_Add
#define Effect_Add ((void (*)(struct PlayState* play, s32* pIndex, EffectType type, u8 arg3, u8 arg4, void* initParams))0x8001aa5cu)
#undef Effect_Destroy
#define Effect_Destroy ((void (*)(struct PlayState* play, s32 index))(void*)FOREIGN_ENEMY_API_HIPLOOP_EFFECT_DESTROY)
#undef Effect_GetByIndex
#define Effect_GetByIndex ((void* (*)(s32 index))0x8001a890u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Item_DropCollectible
#define Item_DropCollectible ((Actor* (*)(struct PlayState* play, Vec3f* spawnPos, u32 params))(void*)FOREIGN_ENEMY_API_DROP)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_Vec3f_Diff
#define Math_Vec3f_Diff ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d48u)
#undef Math_Vec3s_ToVec3f
#define Math_Vec3s_ToVec3f ((void (*)(Vec3f* dest, Vec3s* src))0x80063cccu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Matrix_MultVecX
#define Matrix_MultVecX ((void (*)(f32 x, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_X)
#undef Matrix_MultVecY
#define Matrix_MultVecY ((void (*)(f32 y, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Y)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x8008993cu)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(struct PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c684u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef SurfaceType_GetMaterial
#define SurfaceType_GetMaterial ((SurfaceMaterial (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x800345b8u)
#undef func_800B0DE0
#define func_800B0DE0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep))0x8001bd94u)
#undef func_800B0EB0
#define func_800B0EB0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life))0x8001be64u)
#undef NA_SE_EN_DODO_J_WALK
#define NA_SE_EN_DODO_J_WALK 0x3800
#undef NA_SE_EN_DODO_J_CRY
#define NA_SE_EN_DODO_J_CRY 0x3801
#undef NA_SE_EN_DODO_J_FIRE
#define NA_SE_EN_DODO_J_FIRE 0x3802
#undef NA_SE_EN_DODO_J_DAMAGE
#define NA_SE_EN_DODO_J_DAMAGE 0x3803
#undef NA_SE_EN_DODO_J_DEAD
#define NA_SE_EN_DODO_J_DEAD 0x3804
#undef NA_SE_EN_DODO_J_BREATH
#define NA_SE_EN_DODO_J_BREATH 0x3858
#undef NA_SE_EN_DODO_J_TAIL
#define NA_SE_EN_DODO_J_TAIL 0x3859
#undef NA_SE_EN_DODO_J_EAT
#define NA_SE_EN_DODO_J_EAT 0x385B
#undef NA_SE_EN_GERUDOFT_DOWN
#define NA_SE_EN_GERUDOFT_DOWN 0x387A
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389E
#undef NA_SE_EV_ICE_FREEZE
#define NA_SE_EV_ICE_FREEZE 0x28B2
#undef NA_SE_IT_BOMB_EXPLOSION
#define NA_SE_IT_BOMB_EXPLOSION 0x180E
#define object_dodongo_Anim_0013C4 (*(AnimationHeader*)0x060013c4)
#define object_dodongo_Anim_001A44 (*(AnimationHeader*)0x06001a44)
#define object_dodongo_Anim_0028F0 (*(AnimationHeader*)0x060028f0)
#define object_dodongo_Anim_003088 (*(AnimationHeader*)0x06003088)
#define object_dodongo_Anim_003B14 (*(AnimationHeader*)0x06003b14)
#define object_dodongo_Anim_0042C4 (*(AnimationHeader*)0x060042c4)
#define object_dodongo_Anim_004C20 (*(AnimationHeader*)0x06004c20)
#define object_dodongo_Skel_008318 (*(SkeletonHeader*)0x06008318)
#define object_dodongo_Anim_008B1C (*(AnimationHeader*)0x06008b1c)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER)

void LargeDodongo_Init(Actor* thisx, PlayState* play);
static void dodongoPortInit(Actor* actor, PlayState* play);
void LargeDodongo_Destroy(Actor* thisx, PlayState* play);
static void dodongoPortDestroy(Actor* actor, PlayState* play);
void LargeDodongo_Update(Actor* thisx, PlayState* play2);
static void dodongoPortUpdate(Actor* actor, PlayState* play);
void LargeDodongo_Draw(Actor* thisx, PlayState* play);

void func_808773C4(EnDodongo* this);
void func_80877424(EnDodongo* this, PlayState* play);
void func_80877494(EnDodongo* this);
void func_80877500(EnDodongo* this, PlayState* play);
void func_808777A8(EnDodongo* this);
void func_8087784C(EnDodongo* this, PlayState* play);
void func_80877D50(EnDodongo* this);
void func_80877D90(EnDodongo* this, PlayState* play);
void func_80877E60(EnDodongo* this, PlayState* play);
void func_80878354(EnDodongo* this);
void func_80878424(EnDodongo* this, PlayState* play);
void func_808785B0(EnDodongo* this, PlayState* play);
void func_8087864C(EnDodongo* this);
void func_808786C8(EnDodongo* this, PlayState* play);
void func_80878724(EnDodongo* this);
void func_808787B0(EnDodongo* this, PlayState* play);

ActorProfile MM_Large_Dodongo_Profile = {
    ACTOR_EN_DODONGO,  ACTORCAT_ENEMY,    FLAGS, OBJECT_DODONGO, sizeof(EnDodongo), dodongoPortInit, dodongoPortDestroy,
    dodongoPortUpdate, LargeDodongo_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit1[10] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x800CB019, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 7, { { 500, 0, 0 }, 27 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x800CB019, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 21, { { -200, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x800CB019, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 28, { { -200, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x800CB019, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 22, { { 400, -400, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x800CB019, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 29, { { 400, -400, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 16, { { 1000, 0, 0 }, 28 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 14, { { -300, 300, 0 }, 17 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 14, { { 800, 100, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 13, { { 600, 0, 0 }, 12 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x08 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 13, { { 1500, 0, 0 }, 12 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit1 = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit1),
    sJntSphElementsInit1,
};

static ColliderJntSphInit sJntSphInit2 = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    3,
    sJntSphElementsInit1,
};

static ColliderJntSphElementInit sJntSphElementsInit2[3] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x20000000, 0x01, 0x10 },
            { 0x00000000, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 2, { { 0, -10, 10 }, 25 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x20000000, 0x01, 0x10 },
            { 0x00000000, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 2, { { 0, -5, 50 }, 37 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x20000000, 0x01, 0x10 },
            { 0x00000000, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 2, { { 0, 0, 90 }, 50 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit3 = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit2),
    sJntSphElementsInit2,
};

static DamageTable sDamageTable = {
    DMG_ENTRY(0, 0x1), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x1), DMG_ENTRY(1, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0xF), DMG_ENTRY(2, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(3, 0x0), DMG_ENTRY(1, 0x2),
    DMG_ENTRY(1, 0x3), DMG_ENTRY(2, 0x4), DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x1), DMG_ENTRY(1, 0xE), DMG_ENTRY(2, 0x0),
    DMG_ENTRY(0, 0x1), DMG_ENTRY(0, 0x5), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0),
    DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0),
};

static CollisionCheckInfoInit sColChkInfoInit = { 3, 100, 100, 80 };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_DODONGO, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 1400, ICHAIN_STOP),
};

void LargeDodongo_Init(Actor* thisx, PlayState* play) {
    static EffectBlureInit2 D_80879308 = {
        2,
        EFFECT_BLURE_ELEMENT_FLAG_8,
        0,
        { 255, 255, 255, 255 },
        { 255, 255, 255, 64 },
        { 255, 255, 255, 0 },
        { 255, 255, 255, 0 },
        8,
        0,
        EFF_BLURE_DRAW_MODE_SIMPLE,
        0,
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    };
    EnDodongo* this = (EnDodongo*)thisx;
    s32 i;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->unk_32C.r = 255;
    this->unk_32C.g = 255;
    this->unk_32C.a = 200;
    this->unk_330.r = 255;
    this->unk_330.g = 10;
    this->unk_330.a = 200;
    Math_Vec3f_Copy(&this->unk_314, &gOneVec3f);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    SkelAnime_Init(play, &this->skelAnime, &object_dodongo_Skel_008318, &object_dodongo_Anim_004C20, this->jointTable,
                   this->morphTable, OBJECT_DODONGO_LIMB_MAX);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    Collider_InitAndSetJntSph(play, &this->collider2, &this->actor, &sJntSphInit2, this->collider2Elements);
    Collider_InitAndSetJntSph(play, &this->collider1, &this->actor, &sJntSphInit1, this->collider1Elements);
    Collider_InitAndSetJntSph(play, &this->collider3, &this->actor, &sJntSphInit3, this->collider3Elements);

    for (i = 0; i < ARRAY_COUNT(this->collider2Elements); i++) {
        this->collider2.elements[i].base.elemMaterial = ELEM_MATERIAL_UNK2;
        this->collider2.elements[i].base.acDmgInfo.dmgFlags = 0x77C34FE6;
    }

    Effect_Add(play, &this->unk_338, EFFECT_BLURE2, 0, 0, &D_80879308);
    if (this->actor.params == 0) {
        Actor_SetScale(&this->actor, 3.0f / 160.0f);
        this->unk_334 = 1.0f;
    } else {
        Actor_SetScale(&this->actor, 3.0f / 80.0f);
        this->actor.colChkInfo.health = 12;
        this->unk_334 = 2.0f;

        for (i = 0; i < ARRAY_COUNT(this->collider1Elements); i++) {
            this->collider1.elements[i].dim.modelSphere.radius *= 2;
            this->collider1.elements[i].base.atDmgInfo.damage *= 2;
        }

        for (i = 0; i < ARRAY_COUNT(this->collider2Elements); i++) {
            this->collider2.elements[i].dim.modelSphere.radius *= 2;
        }

        for (i = 0; i < ARRAY_COUNT(this->collider3Elements); i++) {
            this->collider3.elements[i].dim.modelSphere.radius *= 2;
        }
    }

    func_808773C4(this);
}

void LargeDodongo_Destroy(Actor* thisx, PlayState* play) {
    EnDodongo* this = (EnDodongo*)thisx;

    Effect_Destroy(play, this->unk_338);
    Collider_DestroyJntSph(play, &this->collider2);
    Collider_DestroyJntSph(play, &this->collider3);
    Collider_DestroyJntSph(play, &this->collider1);
}

void func_80876930(EnDodongo* this, PlayState* play, Vec3f* arg2) {
    static Color_RGBA8 D_8087932C = { 250, 250, 250, 255 };
    static Color_RGBA8 D_80879330 = { 180, 180, 180, 255 };
    static Color_RGBA8 D_80879334 = { 170, 130, 90, 255 };
    static Color_RGBA8 D_80879338 = { 100, 60, 20, 255 };
    static Vec3f D_8087933C = { 0.0f, 0.3f, 0.0f };
    s32 i;
    Vec3f sp88;
    s32 pad2;
    Color_RGBA8* sp80;
    Color_RGBA8* sp7C;
    s32 pad;
    s16 temp1;
    s16 temp2;
    f32 temp3;

    if (SurfaceType_GetMaterial(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) == SURFACE_MATERIAL_SNOW) {
        sp80 = &D_8087932C;
        sp7C = &D_80879330;
    } else {
        sp80 = &D_80879334;
        sp7C = &D_80879338;
    }

    temp1 = this->unk_334 * 300.0f;
    temp2 = this->unk_334 * 15.0f;
    temp3 = this->unk_334 * 30.0f;

    for (i = 0; i < 3; i++) {
        sp88.x = Rand_CenteredFloat(temp3) + arg2->x;
        sp88.y = Rand_ZeroFloat(5.0f) + this->actor.floorHeight;
        sp88.z = Rand_CenteredFloat(temp3) + arg2->z;
        D_8087933C.x = Rand_CenteredFloat(2.0f);
        D_8087933C.z = Rand_CenteredFloat(2.0f);
        func_800B0DE0(play, &sp88, &gZeroVec3f, &D_8087933C, sp80, sp7C, temp1, temp2);
    }
}

void func_80876B08(EnDodongo* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if (ABS_ALT(yDiff) < 0x4000) {
        if (!(player->stateFlags1 & 0x00800000) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
            func_808777A8(this);
        } else {
            func_80877494(this);
        }
    } else if (Player_GetMask(play) != PLAYER_MASK_STONE) {
        func_80878354(this);
    } else {
        func_80877494(this);
    }
}

void func_80876BD0(EnDodongo* this, PlayState* play, s32 arg2) {
    if (this->actor.colChkInfo.damageEffect == 2) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->drawDmgEffScale = 0.75f;
        this->drawDmgEffAlpha = 4.0f;
    } else if (this->actor.colChkInfo.damageEffect == 4) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
        this->drawDmgEffScale = 0.75f;
        this->drawDmgEffAlpha = 4.0f;
        Actor_Spawn(&play->actorCtx, play, 0xa2, this->collider1.elements[arg2].base.acDmgInfo.hitPos.x,
                    this->collider1.elements[arg2].base.acDmgInfo.hitPos.y,
                    this->collider1.elements[arg2].base.acDmgInfo.hitPos.z, 0, 0, 0,
                    CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
    }
}

void func_80876CAC(EnDodongo* this) {
    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
    this->collider1.base.colMaterial = COL_MATERIAL_HIT3;
    this->drawDmgEffScale = 0.75f;
    this->drawDmgEffFrozenSteamScale = 1.125f;
    this->drawDmgEffAlpha = 1.0f;
    this->timer = 80;
    this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 80);
}

void func_80876D28(EnDodongo* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        this->timer = 0;
        this->actor.colorFilterTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->collider1.base.colMaterial = COL_MATERIAL_HIT0;
        this->drawDmgEffAlpha = 0.0f;
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, DODONGO_BODYPART_MAX, 2, this->unk_334 * 0.3f,
                              this->unk_334 * 0.2f);
        this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    }
}

void func_80876DC4(EnDodongo* this, PlayState* play) {
    s32 pad;
    Vec3f sp80;
    Vec3f sp74;
    Vec3f sp68;
    s16 sp66;
    s16 sp64;
    s16 sp62;
    f32 temp_f22;
    f32 temp_f20;

    Math_Vec3f_Copy(&sp68, &this->bodyPartsPos[DODONGO_BODYPART_0]);
    sp66 = ((s32)Rand_Next() >> 0x12) + this->actor.shape.rot.y;
    temp_f20 = Math_CosS(sp66);
    temp_f22 = Math_SinS(sp66);

    if (this->unk_32C.r > 30) {
        this->unk_32C.r -= 16;
        this->unk_32C.g += -16;
    }

    if (this->unk_32C.b < 30) {
        this->unk_32C.b += 5;
        this->unk_32C.a += 8;
        this->unk_330.a += 8;
    }

    if (this->unk_330.r != 0) {
        this->unk_330.r -= 15;
    }

    if (this->unk_330.g != 0) {
        this->unk_330.g--;
    }

    sp80.x = temp_f22 * 3.5f * this->unk_334;
    sp80.y = this->unk_330.r * 0.02f * this->unk_334;
    sp80.z = temp_f20 * 3.5f * this->unk_334;
    sp74.x = (Rand_ZeroFloat(0.1f) + 0.15f) * -temp_f22 * this->unk_334;
    sp74.y = this->unk_334 * 0.3f;
    sp74.z = (Rand_ZeroFloat(0.1f) + 0.15f) * -temp_f20 * this->unk_334;
    sp64 = this->unk_334 * 100.0f;
    sp62 = this->unk_334 * 25.0f;
    func_800B0EB0(play, &sp68, &sp80, &sp74, &this->unk_32C, &this->unk_330, sp64, sp62, 0x14);

    sp66 = (Rand_Next() >> 0x13) + this->actor.shape.rot.y;
    temp_f20 = Math_CosS(sp66);
    temp_f22 = Math_SinS(sp66);

    sp68.x -= (temp_f20 * 6.0f) * this->unk_334;
    sp68.z += (temp_f22 * 6.0f) * this->unk_334;

    sp80.x = -temp_f20 * 3.5f * this->unk_334;
    sp80.z = temp_f22 * 3.5f * this->unk_334;

    sp74.x = (Rand_ZeroFloat(0.1f) + 0.15f) * temp_f20 * this->unk_334;
    sp74.z = (Rand_ZeroFloat(0.1f) + 0.15f) * -temp_f22 * this->unk_334;
    func_800B0EB0(play, &sp68, &sp80, &sp74, &this->unk_32C, &this->unk_330, sp64, sp62, 0x14);

    sp68.x = this->bodyPartsPos[DODONGO_BODYPART_0].x + (temp_f20 * 6.0f * this->unk_334);
    sp68.z = this->bodyPartsPos[DODONGO_BODYPART_0].z - (temp_f22 * 6.0f * this->unk_334);
    sp80.x *= -1.0f;
    sp80.z *= -1.0f;
    sp74.x = (Rand_ZeroFloat(0.1f) + 0.15f) * -temp_f20 * this->unk_334;
    sp74.z = (Rand_ZeroFloat(0.1f) + 0.15f) * temp_f22 * this->unk_334;
    func_800B0EB0(play, &sp68, &sp80, &sp74, &this->unk_32C, &this->unk_330, sp64, sp62, 0x14);
}

s32 func_8087721C(EnDodongo* this) {
    if ((this->actionFunc == func_8087784C) && (this->skelAnime.curFrame >= 29.0f) &&
        (this->skelAnime.curFrame <= 43.0f)) {
        return true;
    }
    return false;
}

s32 func_80877278(EnDodongo* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
    Vec3f sp44;

    while (explosive != NULL) {
        if (!explosive->params && (explosive->parent == NULL) && (explosive->update != NULL) &&
            (explosive != player->interactRangeActor) &&
            (((explosive->id == ACTOR_EN_BOM) && (((EnBom*)explosive)->timer > 0)) ||
             ((explosive->id == ACTOR_EN_BOMBF) && (((EnBombf*)explosive)->timer > 0)))) {
            Math_Vec3f_Diff(&explosive->world.pos, &this->unk_308, &sp44);
            if ((fabsf(sp44.x) < 20.0f) && (fabsf(sp44.y) < 10.0f) && (fabsf(sp44.z) < 20.0f)) {
                this->actor.child = explosive;
                explosive->freezeTimer = 30000;
                return true;
            }
        }
        explosive = explosive->next;
    }
    return false;
}

void func_808773C4(EnDodongo* this) {
    Animation_MorphToLoop(&this->skelAnime, &object_dodongo_Anim_004C20, -4.0f);
    this->actor.speed = 0.0f;
    this->timer = Rand_S16Offset(30, 50);
    this->actionFunc = func_80877424;
}

void func_80877424(EnDodongo* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (this->timer != 0) {
        this->timer--;
    }

    if (this->timer == 0) {
        if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
            func_80877494(this);
        }
    }
}

void func_80877494(EnDodongo* this) {
    Animation_MorphToLoop(&this->skelAnime, &object_dodongo_Anim_008B1C, -4.0f);
    this->actor.speed = this->unk_334 * 1.5f;
    this->timer = Rand_S16Offset(50, 70);
    this->actionFunc = func_80877500;
}

void func_80877500(EnDodongo* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f sp30;
    s16 temp_v1;

    SkelAnime_Update(&this->skelAnime);
    if (Animation_OnFrame(&this->skelAnime, 19.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_WALK);
        sp30.x = this->collider1Elements[2].dim.worldSphere.center.x;
        sp30.y = this->collider1Elements[2].dim.worldSphere.center.y;
        sp30.z = this->collider1Elements[2].dim.worldSphere.center.z;
        func_80876930(this, play, &sp30);
    } else if (Animation_OnFrame(&this->skelAnime, 39.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_WALK);
        sp30.x = this->collider1Elements[1].dim.worldSphere.center.x;
        sp30.y = this->collider1Elements[1].dim.worldSphere.center.y;
        sp30.z = this->collider1Elements[1].dim.worldSphere.center.z;
        func_80876930(this, play, &sp30);
    }

    if ((Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < 400.0f) &&
        (Player_GetMask(play) != PLAYER_MASK_STONE)) {
        temp_v1 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        Math_ScaledStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0x1F4);
        if (this->actor.xzDistToPlayer < (100.0f * this->unk_334)) {
            if ((ABS_ALT(temp_v1) < 0x1388) && (this->actor.yDistanceFromLink < 60.0f) &&
                !(player->stateFlags1 & PLAYER_STATE1_800000)) {
                func_808777A8(this);
            }
        }
    } else {
        if ((Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 150.0f) ||
            (Player_GetMask(play) == PLAYER_MASK_STONE)) {
            Math_ScaledStepToS(&this->actor.world.rot.y, Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos),
                               0x1F4);
        }

        this->timer--;
        if (this->timer == 0) {
            if (Rand_ZeroOne() > 0.7f) {
                this->timer = Rand_S16Offset(50, 70);
            } else {
                func_808773C4(this);
            }
        }
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void func_808777A8(EnDodongo* this) {
    s32 i;
    Sphere16* sph;

    Animation_MorphToPlayOnce(&this->skelAnime, &object_dodongo_Anim_0028F0, -4.0f);
    this->actor.speed = 0.0f;

    for (i = 0; i < ARRAY_COUNT(this->collider3Elements); i++) {
        sph = &this->collider3.elements[i].dim.worldSphere;

        sph->center.x = this->bodyPartsPos[DODONGO_BODYPART_0].x;
        sph->center.y = this->bodyPartsPos[DODONGO_BODYPART_0].y;
        sph->center.z = this->bodyPartsPos[DODONGO_BODYPART_0].z;
        sph->radius = 0;
    }

    this->actionFunc = func_8087784C;
}

void func_8087784C(EnDodongo* this, PlayState* play) {
    static Vec3f D_80879348 = { 0.0f, 0.9f, 0.0f };
    static Vec3f D_80879354 = { 0.0f, 0.0f, 0.0f };
    s16 frame;
    ColliderJntSphElement* jntSphElem;
    s32 i;
    s32 end;
    f32 temp_f2;
    f32 temp_f12;

    if (Animation_OnFrame(&this->skelAnime, 24.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_CRY);
    }

    if (func_8087721C(this)) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_DODO_J_FIRE - SFX_FLAG);
        frame = this->skelAnime.curFrame - 29.0f;
        end = frame >> 1;
        if (end > 3) {
            end = 3;
        }

        jntSphElem = &this->collider3.elements[0];
        temp_f2 = Math_SinS(this->actor.shape.rot.y) * this->unk_334;
        temp_f12 = Math_CosS(this->actor.shape.rot.y) * this->unk_334;

        for (i = 0; i < end; i++, jntSphElem++) {
            jntSphElem->dim.worldSphere.center.x =
                this->bodyPartsPos[DODONGO_BODYPART_0].x + (jntSphElem->dim.modelSphere.center.z * temp_f2);
            jntSphElem->dim.worldSphere.center.y =
                this->bodyPartsPos[DODONGO_BODYPART_0].y + (jntSphElem->dim.modelSphere.center.y * this->unk_334);
            jntSphElem->dim.worldSphere.center.z =
                this->bodyPartsPos[DODONGO_BODYPART_0].z + (jntSphElem->dim.modelSphere.center.z * temp_f12);
            jntSphElem->dim.worldSphere.radius = jntSphElem->dim.modelSphere.radius;
        }

        D_80879354.y = this->unk_334 * -4.5f;
        D_80879348.x = 2.5f * temp_f2;
        D_80879348.y = this->unk_334 * 1.4f;
        D_80879348.z = 2.5f * temp_f12;
        EffectSsDFire_Spawn(play, &this->bodyPartsPos[DODONGO_BODYPART_0], &D_80879354, &D_80879348,
                            this->unk_334 * 100.0f, this->unk_334 * 35.0f, 0xFF - (frame * 10), 5, 0, 8);
    } else if ((this->skelAnime.curFrame >= 2.0f) && (this->skelAnime.curFrame <= 20.0f)) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_DODO_J_BREATH - SFX_FLAG);
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        func_80877D50(this);
    }
}

void func_80877D50(EnDodongo* this) {
    Animation_PlayOnce(&this->skelAnime, &object_dodongo_Anim_003088);
    this->actionFunc = func_80877D90;
}

void func_80877D90(EnDodongo* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        func_808773C4(this);
        this->timer = Rand_S16Offset(10, 20);
    }
}

void func_80877DE0(EnDodongo* this) {
    Animation_Change(&this->skelAnime, &object_dodongo_Anim_0028F0, -1.0f, 35.0f, 0.0f, ANIMMODE_ONCE, -4.0f);
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->timer = 25;
    this->actionFunc = func_80877E60;
    this->actor.speed = 0.0f;
}

void func_80877E60(EnDodongo* this, PlayState* play) {
    static Vec3f D_80879360 = { 0.0f, 0.6f, 0.0f };
    static Color_RGBA8 D_8087936C = { 255, 255, 255, 255 };
    Vec3f sp84;
    Vec3f sp78;
    s16 i;
    Actor* explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
    Vec3f sp64;
    s32 pad;
    s16 sp5E;
    s16 sp5C;

    while (explosive != NULL) {
        if (explosive == this->actor.child) {
            break;
        }
        explosive = explosive->next;
    }

    if (explosive == NULL) {
        this->actor.child = NULL;
    }

    if (this->actor.child != NULL) {
        Math_Vec3f_Copy(&this->actor.child->world.pos, &this->unk_308);
        this->actor.child->freezeTimer = 30000;
    }

    if (Animation_OnFrame(&this->skelAnime, 28.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_EAT);
        if (this->actor.child != NULL) {
            Actor_Kill(this->actor.child);
            this->actor.child = NULL;
        }
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    } else if (this->skelAnime.playSpeed > -0.5f) {
        this->timer--;
        if (this->timer == 10) {
            for (i = 10; i >= 0; i--) {
                sp84.x = Rand_CenteredFloat(10.0f);
                sp84.y = Rand_CenteredFloat(10.0f);
                sp84.z = Rand_CenteredFloat(10.0f);
                sp78.x = sp84.x * -0.1f;
                sp78.y = sp84.y * -0.1f;
                sp78.z = sp84.z * -0.1f;
                sp64.x = this->collider1Elements[0].dim.worldSphere.center.x + sp84.x;
                sp64.y = this->collider1Elements[0].dim.worldSphere.center.y + sp84.y;
                sp64.z = this->collider1Elements[0].dim.worldSphere.center.z + sp84.z;
                func_800B0EB0(play, &sp64, &sp84, &sp78, &this->unk_32C, &this->unk_330, this->unk_334 * 400.0f,
                              this->unk_334 * 10.0f, 10);
            }

            Actor_PlaySfx(&this->actor, NA_SE_IT_BOMB_EXPLOSION);
            if (this->actor.colChkInfo.health <= 4) {
                this->actor.colChkInfo.health = 0;
                Enemy_StartFinishingBlow(play, &this->actor);
            } else {
                this->actor.colChkInfo.health -= 4;
            }
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 120, COLORFILTER_BUFFLAG_OPA, 8);
        }
    } else if (Animation_OnFrame(&this->skelAnime, 24.0f)) {
        this->timer--;
        this->skelAnime.playSpeed = 0.0f;
    }

    if (this->skelAnime.curFrame <= 27.0f) {
        if ((this->skelAnime.curFrame <= 25.0f) && (this->timer < 11)) {
            func_80876DC4(this, play);
        } else {
            sp5E = this->unk_334 * 50.0f;
            sp5C = this->unk_334 * 5.0f;
            Math_Vec3f_Copy(&sp64, &this->bodyPartsPos[DODONGO_BODYPART_0]);
            func_800B0DE0(play, &sp64, &gZeroVec3f, &D_80879360, &D_8087936C, &D_8087936C, sp5E, sp5C);
            sp64.x -= Math_CosS(this->actor.shape.rot.y) * 6.0f * this->unk_334;
            sp64.z += Math_SinS(this->actor.shape.rot.y) * 6.0f * this->unk_334;
            func_800B0DE0(play, &sp64, &gZeroVec3f, &D_80879360, &D_8087936C, &D_8087936C, sp5E, sp5C);
            sp64.x = (2.0f * this->bodyPartsPos[DODONGO_BODYPART_0].x) - sp64.x;
            sp64.z = (2.0f * this->bodyPartsPos[DODONGO_BODYPART_0].z) - sp64.z;
            func_800B0DE0(play, &sp64, &gZeroVec3f, &D_80879360, &D_8087936C, &D_8087936C, sp5E, sp5C);
        }
    }

    this->unk_314.z = Math_SinS((this->actor.colorFilterTimer * 0x1000) >> 1) + 1.0f;
    this->unk_314.x = Math_SinS(this->actor.colorFilterTimer * 0x1000) + 1.0f;
    this->unk_314.y = this->unk_314.z;
    SkelAnime_Update(&this->skelAnime);
    if (this->timer == 0) {
        if (this->actor.colChkInfo.health == 0) {
            func_80878724(this);
        } else {
            func_8087864C(this);
        }
        this->unk_304 = 1;
    }
}

void func_80878354(EnDodongo* this) {
    s32 pad;
    AnimationHeader* anim;
    s16 yDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    this->unk_306 = (0xFFFF - ABS_ALT(yDiff)) / 15;

    if (yDiff >= 0) {
        anim = &object_dodongo_Anim_0042C4;
        this->unk_306 = -this->unk_306;
    } else {
        anim = &object_dodongo_Anim_003B14;
    }

    Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_TAIL);
    Animation_PlayOnceSetSpeed(&this->skelAnime, anim, 2.0f);
    this->timer = 0;
    this->collider1.base.atFlags |= AT_ON;
    this->unk_304 = -1;
    this->actionFunc = func_80878424;
}

void func_80878424(EnDodongo* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f sp20;

    this->timer++;
    if (SkelAnime_Update(&this->skelAnime)) {
        if (!(player->stateFlags1 & PLAYER_STATE1_800000) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
            this->collider1.base.atFlags &= ~AT_ON;
            func_808777A8(this);
        } else {
            func_80877494(this);
        }
        return;
    }

    this->actor.world.rot.y += this->unk_306;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    sp20.x = this->collider1Elements[1].dim.worldSphere.center.x;
    sp20.y = this->collider1Elements[1].dim.worldSphere.center.y;
    sp20.z = this->collider1Elements[1].dim.worldSphere.center.z;
    func_80876930(this, play, &sp20);
    sp20.x = this->collider1Elements[2].dim.worldSphere.center.x;
    sp20.y = this->collider1Elements[2].dim.worldSphere.center.y;
    sp20.z = this->collider1Elements[2].dim.worldSphere.center.z;
    func_80876930(this, play, &sp20);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider1.base);
    this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;
}

void func_80878594(EnDodongo* this) {
    this->actionFunc = func_808785B0;
    this->actor.speed = 0.0f;
}

void func_808785B0(EnDodongo* this, PlayState* play) {
    if (this->timer != 0) {
        this->timer--;
    }

    if (this->timer == 0) {
        func_80876D28(this, play);
        if (this->actor.colChkInfo.health == 0) {
            func_80878724(this);
        } else if (this->actor.xzDistToPlayer > (100.0f * this->unk_334)) {
            func_80877494(this);
        } else {
            func_80876B08(this, play);
        }
    }
}

void func_8087864C(EnDodongo* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dodongo_Anim_001A44, -4.0f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_DAMAGE);
    this->timer = 0;
    this->unk_304 = 0;
    this->actor.speed = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->actionFunc = func_808786C8;
}

void func_808786C8(EnDodongo* this, PlayState* play) {
    if (this->unk_304 == 1) {
        func_80876DC4(this, play);
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        func_80876B08(this, play);
    }
}

void func_80878724(EnDodongo* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dodongo_Anim_0013C4, -8.0f);
    this->timer = 0;
    this->unk_304 = 0;
    Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_J_DEAD);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.speed = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->actionFunc = func_808787B0;
}

void func_808787B0(EnDodongo* this, PlayState* play) {
    s32 pad;

    if (this->skelAnime.curFrame < 35.0f) {
        if (this->unk_304 != 0) {
            func_80876DC4(this, play);
        }
    } else if (this->actor.colorFilterTimer == 0) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 120, COLORFILTER_BUFFLAG_OPA, 4);
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer == 0) {
            EnBom* bomb =
                (EnBom*)Actor_Spawn(&play->actorCtx, play, 9, this->actor.world.pos.x, this->actor.world.pos.y,
                                    this->actor.world.pos.z, BOMB_EXPLOSIVE_TYPE_BOMB, 0, 0, BOMB_TYPE_BODY);
            if (bomb != NULL) {
                bomb->timer = 0;
            }
            this->timer = 8;
        }
    } else if (Animation_OnFrame(&this->skelAnime, 52.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_GERUDOFT_DOWN);
    }

    if (this->timer != 0) {
        this->timer--;
        if (this->timer == 0) {
            if (this->actor.params == 0) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x90);
            } else {
                Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_PURPLE);
            }
            Actor_Kill(&this->actor);
        }
    }
}

void LargeDodongo_UpdateDamage(EnDodongo* this, PlayState* play) {
    Vec3f sp3C;
    s32 i;

    if (this->collider2.base.acFlags & AC_HIT) {
        this->collider2.base.acFlags &= ~AC_HIT;
        this->collider1.base.acFlags &= ~AC_HIT;

        for (i = 0; i < ARRAY_COUNT(this->collider2Elements); i++) {
            if (this->collider2.elements[i].base.acElemFlags & ACELEM_HIT) {
                break;
            }
        }

        if ((i != ARRAY_COUNT(this->collider2Elements)) &&
            ((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) ||
             !(this->collider2.elements[i].base.acHitElem->atDmgInfo.dmgFlags & 0xDB0B3))) {
            func_80876D28(this, play);
            Math_Vec3s_ToVec3f(&sp3C, &this->collider2.elements[i].base.acDmgInfo.hitPos);
            if (this->actor.colChkInfo.damageEffect == 0xF) {
                CollisionCheck_BlueBlood(play, NULL, &sp3C);
                EffectSsHitmark_SpawnFixedScale(play, EFFECT_HITMARK_WHITE, &sp3C);
            } else if (this->actor.colChkInfo.damageEffect != 0xE) {
                EffectSsHitmark_SpawnFixedScale(play, EFFECT_HITMARK_METAL, &sp3C);
                CollisionCheck_SpawnShieldParticlesMetalSound(play, &sp3C, &this->actor.projectedPos);
            }
        }
    } else if (this->collider1.base.acFlags & AC_HIT) {
        this->collider2.base.acFlags &= ~AC_HIT;
        this->collider1.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&this->actor, &this->collider1);

        for (i = 0; i < ARRAY_COUNT(this->collider1Elements); i++) {
            if (this->collider1.elements[i].base.acElemFlags & ACELEM_HIT) {
                break;
            }
        }

        if ((i != ARRAY_COUNT(this->collider1Elements)) &&
            ((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) ||
             !(this->collider1.elements[i].base.acHitElem->atDmgInfo.dmgFlags & 0xDB0B3))) {
            func_80876D28(this, play);
            if (this->actor.colChkInfo.damageEffect != 0xF) {
                if (!Actor_ApplyDamage(&this->actor)) {
                    Enemy_StartFinishingBlow(play, &this->actor);
                    if (this->actor.colChkInfo.damageEffect == 3) {
                        func_80876CAC(this);
                        this->timer = 3;
                        this->collider1.base.acFlags &= ~AC_ON;
                        this->collider2.base.acFlags &= ~AC_ON;
                        func_80878594(this);
                    } else {
                        func_80876BD0(this, play, i);
                        func_80878724(this);
                    }
                } else if (this->actor.colChkInfo.damageEffect == 1) {
                    this->timer = 40;
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                    Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                    func_80878594(this);
                } else if (this->actor.colChkInfo.damageEffect == 5) {
                    this->timer = 40;
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                    Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
                    this->drawDmgEffScale = 0.75f;
                    this->drawDmgEffAlpha = 2.0f;
                    func_80878594(this);
                } else if (this->actor.colChkInfo.damageEffect == 3) {
                    func_80876CAC(this);
                    func_80878594(this);
                } else {
                    func_80876BD0(this, play, i);
                    func_8087864C(this);
                }
            }
        }
    }
}

void LargeDodongo_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnDodongo* this = (EnDodongo*)thisx;

    LargeDodongo_UpdateDamage(this, play);
    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 60.0f, 70.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_GERUDOFT_DOWN);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
    if (this->actionFunc != func_808787B0) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider2.base);
        if ((this->actionFunc != func_808786C8) && (this->actionFunc != func_80877E60)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
            if ((this->actionFunc != func_8087784C) && func_80877278(this, play)) {
                func_80877DE0(this);
            }
        }
    }

    if (func_8087721C(this)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider3.base);
    }

    if (this->drawDmgEffAlpha > 0.0f) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
            this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.375f;
            this->drawDmgEffScale = (this->drawDmgEffScale > 0.75f) ? 0.75f : this->drawDmgEffScale;
        } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.75f, 0.01875f)) {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        }
    }
}

s32 LargeDodongo_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnDodongo* this = (EnDodongo*)thisx;

    if (limbIndex == OBJECT_DODONGO_LIMB_01) {
        pos->z += 1000.0f;
    } else if ((limbIndex == OBJECT_DODONGO_LIMB_0F) || (limbIndex == OBJECT_DODONGO_LIMB_10)) {
        Matrix_Scale(this->unk_314.x, this->unk_314.y, this->unk_314.z, MTXMODE_APPLY);
    }

    return false;
}

static Vec3f D_80879370 = { 1800.0f, 1200.0f, 0.0f };
static Vec3f D_8087937C = { 1500.0f, 300.0f, 0.0f };

static s8 sLimbToBodyParts[OBJECT_DODONGO_LIMB_MAX] = {
    BODYPART_NONE, BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,
    BODYPART_NONE, BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,
    BODYPART_NONE, DODONGO_BODYPART_2, DODONGO_BODYPART_3, DODONGO_BODYPART_4, BODYPART_NONE,      BODYPART_NONE,
    BODYPART_NONE, BODYPART_NONE,      BODYPART_NONE,      DODONGO_BODYPART_5, DODONGO_BODYPART_6, BODYPART_NONE,
    BODYPART_NONE, BODYPART_NONE,      BODYPART_NONE,      BODYPART_NONE,      DODONGO_BODYPART_7, DODONGO_BODYPART_8,
    BODYPART_NONE,
};

void LargeDodongo_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnDodongo* this = (EnDodongo*)thisx;

    Collider_UpdateSpheres(limbIndex, &this->collider1);
    Collider_UpdateSpheres(limbIndex, &this->collider2);
    if (sLimbToBodyParts[limbIndex] != BODYPART_NONE) {
        Matrix_MultZero(&this->bodyPartsPos[sLimbToBodyParts[limbIndex]]);
    }

    if (limbIndex == OBJECT_DODONGO_LIMB_07) {
        Matrix_MultVec3f(&D_80879370, &this->unk_308);
        Matrix_MultVec3f(&D_8087937C, &this->bodyPartsPos[DODONGO_BODYPART_0]);
        Matrix_MultZero(&this->actor.focus.pos);
        Matrix_MultVecY(-200.0f, &this->bodyPartsPos[DODONGO_BODYPART_1]);
    } else if (limbIndex == OBJECT_DODONGO_LIMB_0D) {
        Matrix_MultVecX(1600.0f, &this->unk_320);
    }

    if ((limbIndex == OBJECT_DODONGO_LIMB_1E) && (this->actionFunc == func_80878424) &&
        (this->timer != this->unk_304)) {
        EffectBlure_AddVertex(Effect_GetByIndex(this->unk_338), &this->unk_320,
                              &this->bodyPartsPos[DODONGO_BODYPART_4]);
        this->unk_304 = this->timer;
    }
}

void LargeDodongo_Draw(Actor* thisx, PlayState* play) {
    EnDodongo* this = (EnDodongo*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, LargeDodongo_OverrideLimbDraw,
                      LargeDodongo_PostLimbDraw, &this->actor);
    Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, DODONGO_BODYPART_MAX,
                            this->drawDmgEffScale * this->unk_334, this->drawDmgEffFrozenSteamScale * this->unk_334,
                            this->drawDmgEffAlpha, this->drawDmgEffType);
}

OVL_INFO_NATIVE_ENEMY(0xb, MM_Large_Dodongo_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_INIT, dodongoPortInit);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_DESTROY, dodongoPortDestroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_UPDATE, dodongoPortUpdate);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_DRAW, LargeDodongo_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_PROFILE, MM_Large_Dodongo_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 1, 0xb, 0xa, 0x4dfdaf2a };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_DODONGO_ORIGIN, kNativePortOrigin);

static void dodongoPortInit(Actor* actor, PlayState* play) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    LargeDodongo_Init(actor, play);
    comboEnemyNativeLeave(&scope);
}

static void dodongoPortUpdate(Actor* actor, PlayState* play) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    LargeDodongo_Update(actor, play);
    comboEnemyNativeLeave(&scope);
}

static void dodongoPortDestroy(Actor* actor, PlayState* play) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    LargeDodongo_Destroy(actor, play);
    comboEnemyNativeLeave(&scope);
}
