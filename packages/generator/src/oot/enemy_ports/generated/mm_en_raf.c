#include "mm_native_compat.h"
#include "types/en_raf.h"
#define gCarnivorousLilyPadCol (*(CollisionHeader*)0x6000108)
#define gCarnivorousLilyPadConvulseAnim (*(AnimationHeader*)0x60003fc)
#define gCarnivorousLilyPadDeathAnim (*(AnimationHeader*)0x60007a8)
#define gCarnivorousLilyPadSpitAnim (*(AnimationHeader*)0x6000a64)
#define gCarnivorousLilyPadChewAnim (*(AnimationHeader*)0x6000b30)
#define gCarnivorousLilyPadCloseAnim (*(AnimationHeader*)0x6000c7c)
#define gCarnivorousLilyPadParticleDL ((Gfx*)0x60024e0)
#define gCarnivorousLilyPadTrapPetalTex ((u8*)0x6002ef8)
#define gCarnivorousLilyPadTrapTeethTex ((u8*)0x60032f8)
#define gCarnivorousLilyPadSkel (*(FlexSkeletonHeader*)0x6003428)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Actor_ChangeCategory
#define Actor_ChangeCategory ((void (*)(struct PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory))(void*)FOREIGN_ENEMY_API_CATEGORY)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef CollisionHeader_GetVirtual
#define CollisionHeader_GetVirtual ((void (*)(CollisionHeader* colHeader, CollisionHeader** dest))(void*)FOREIGN_ENEMY_API_LILYPAD_COLLISION)
#undef DynaPolyActor_Init
#define DynaPolyActor_Init ((void (*)(DynaPolyActor* dynaActor, s32 transformFlags))0x80035260u)
#undef DynaPolyActor_IsPlayerOnTop
#define DynaPolyActor_IsPlayerOnTop ((s32 (*)(DynaPolyActor* dynaActor))0x80035364u)
#undef DynaPoly_DeleteBgActor
#define DynaPoly_DeleteBgActor ((void (*)(struct PlayState* play, DynaCollisionContext* dyna, s32 bgId))0x80031638u)
#undef DynaPoly_SetBgActor
#define DynaPoly_SetBgActor ((s32 (*)(struct PlayState* play, DynaCollisionContext* dyna, struct Actor* actor, CollisionHeader* colHeader))0x800313a4u)
#undef EffectSsGRipple_Spawn
#define EffectSsGRipple_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life))0x8001cf3cu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Flags_GetSwitch
#define Flags_GetSwitch ((s32 (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef Flags_SetSwitch
#define Flags_SetSwitch ((void (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef SkelAnime_DrawTransformFlexOpa
#define SkelAnime_DrawTransformFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, TransformLimbDrawOpa transformLimbDraw, struct Actor* actor))(void*)FOREIGN_ENEMY_API_TRANSFORM_FLEX_OPA)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef func_800B8D50
#define func_800B8D50 ((void (*)(struct PlayState* play, Actor* actor, f32 arg2, s16 yaw, f32 arg4, u32 arg5))(void*)FOREIGN_ENEMY_API_KNOCKBACK_LARGE)

#undef ACTOR_EN_RAF
#define ACTOR_EN_RAF 0x13a
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#define CLEAR_TAG_SMALL_EXPLOSION 0
#define PLAYER_FORM_DEKU 3
#define PLAYER_FORM_GORON 1
#define CHECK_WEEKEVENTREG(x) 0
#undef BREG
#define BREG(x) 0
#define SWITCH_FLAG_NONE (-1)
static Vec3f rafZeroVec;
#define gZeroVec3f rafZeroVec
#define CollisionCheck_GreenBlood ((void (*)(PlayState*,Collider*,Vec3f*))0x8004c59c)

#undef NA_SE_EN_COMMON_WATER_MID
#define NA_SE_EN_COMMON_WATER_MID 0x38c5

#undef NA_SE_EN_SUISEN_DEAD
#define NA_SE_EN_SUISEN_DEAD 0x3851

#undef NA_SE_EN_SUISEN_DRINK
#define NA_SE_EN_SUISEN_DRINK 0x3992

#undef NA_SE_EN_SUISEN_EAT
#define NA_SE_EN_SUISEN_EAT 0x3850

#undef NA_SE_EN_SUISEN_THROW
#define NA_SE_EN_SUISEN_THROW 0x3994

#undef NA_SE_EV_BOMB_DROP_WATER
#define NA_SE_EV_BOMB_DROP_WATER 0x2817

#undef NA_SE_IT_BOMB_EXPLOSION
#define NA_SE_IT_BOMB_EXPLOSION 0x180e

#undef NA_SE_VO_LI_DAMAGE_S
#define NA_SE_VO_LI_DAMAGE_S 0x6805

#define FLAGS (ACTOR_FLAG_LOCK_ON_DISABLED)

void EnRaf_Init(Actor* thisx, PlayState* play);
void EnRaf_Destroy(Actor* thisx, PlayState* play);
void EnRaf_Update(Actor* thisx, PlayState* play);
void EnRaf_Draw(Actor* thisx, PlayState* play);

void EnRaf_SetupIdle(EnRaf* this);
void EnRaf_Idle(EnRaf* this, PlayState* play);
void EnRaf_SetupGrab(EnRaf* this);
void EnRaf_Grab(EnRaf* this, PlayState* play);
void EnRaf_SetupChew(EnRaf* this);
void EnRaf_Chew(EnRaf* this, PlayState* play);
void EnRaf_SetupThrow(EnRaf* this, PlayState* play);
void EnRaf_Throw(EnRaf* this, PlayState* play);
void EnRaf_Explode(EnRaf* this, PlayState* play);
void EnRaf_PostDetonation(EnRaf* this, PlayState* play);
void EnRaf_Convulse(EnRaf* this, PlayState* play);
void EnRaf_SetupDissolve(EnRaf* this);
void EnRaf_Dissolve(EnRaf* this, PlayState* play);
void EnRaf_SetupDormant(EnRaf* this);
void EnRaf_Dormant(EnRaf* this, PlayState* play);
void EnRaf_InitializeEffect(EnRaf* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 timer);
void EnRaf_UpdateEffects(EnRaf* this, PlayState* play);
void EnRaf_DrawEffects(EnRaf* this, PlayState* play);

typedef enum CarnivorousLilyAction {
    CARNIVOROUS_LILY_ACTION_IDLE,
    CARNIVOROUS_LILY_ACTION_GRAB,
    CARNIVOROUS_LILY_ACTION_CHEW,
    CARNIVOROUS_LILY_ACTION_THROW,
    CARNIVOROUS_LILY_ACTION_EXPLODE,
    CARNIVOROUS_LILY_ACTION_CONVULSE,
    CARNIVOROUS_LILY_ACTION_DISSOLVE,
    CARNIVOROUS_LILY_ACTION_DORMANT
} CarnivorousLilyAction;

typedef enum CarnivorousLilyGrabTarget {
    CARNIVOROUS_LILY_GRAB_TARGET_PLAYER,
    CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE,
    CARNIVOROUS_LILY_GRAB_TARGET_GORON_PLAYER
} CarnivorousLilyGrabTarget;

typedef enum CarnivorousLilyPetalScaleType {
    CARNIVOROUS_LILY_PETAL_SCALE_TYPE_DEAD,
    CARNIVOROUS_LILY_PETAL_SCALE_TYPE_GRAB,
    CARNIVOROUS_LILY_PETAL_SCALE_TYPE_CHEW,
    CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW
} CarnivorousLilyPetalScaleType;

ActorProfile En_Raf_Profile = {
    0xd3a, ACTORCAT_ENEMY, FLAGS, 0x1161, sizeof(EnRaf), EnRaf_Init, EnRaf_Destroy, EnRaf_Update, EnRaf_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x04, 0x10 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_ON,
    },
    { 50, 10, -10, { 0, 0, 0 } },
};

static u8 sTeethClearPixelTableFirstPass[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
    1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,
};

static u8 sTeethClearPixelTableSecondPass[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static u8 sPetalClearPixelTableFirstPass[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1,
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0,
    0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
};

static u8 sPetalClearPixelTableSecondPass[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static DamageTable sDamageTable = {
    DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xE), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF),
    DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF),
    DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0xF),
    DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0xF),
};

void EnRaf_ClearPixelTeeth(u16* texture, u8* clearPixelTable, s32 index) {
    if ((index < (8 * 8)) && (clearPixelTable[index] != 0)) {
        texture[index] = 0;
    }
}

void EnRaf_ClearPixelPetal(u16* texture, u8* clearPixelTable, s32 index) {
    if (clearPixelTable[index] != 0) {
        texture[index] = 0;
    }
}

void EnRaf_Init(Actor* thisx, PlayState* play) {
    EnRaf* this = (EnRaf*)thisx;
    Vec3f limbScale = { 1.0f, 1.0f, 1.0f };
    s32 pad;
    s32 i;
    CollisionHeader* colHeader = NULL;

    Vec3f probe = thisx->world.pos;
    CollisionPoly* floor = NULL;
    WaterBox* water;
    s32 bgId;
    f32 surface;
    probe.y += 80.f;
    f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, thisx, &probe);
    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, probe.x, probe.z, &surface, &water) && surface > y)
        y = surface;
    if (y > -30000.f)
        thisx->home.pos.y = thisx->world.pos.y = y;
    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gCarnivorousLilyPadCol, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    Collider_InitAndSetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    if (this->dyna.bgId == BGCHECK_SCENE) {
        Actor_Kill(&this->dyna.actor);
        return;
    }
    this->dyna.actor.attentionRangeType = ATTENTION_RANGE_3;
    this->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
    SkelAnime_InitFlex(play, &this->skelAnime, &gCarnivorousLilyPadSkel, &gCarnivorousLilyPadSpitAnim, this->jointTable,
                       this->morphTable, CARNIVOROUS_LILY_PAD_LIMB_MAX);

    for (i = 0; i < ARRAY_COUNT(this->limbScale); i++) {
        Math_Vec3f_Copy(&this->targetLimbScale[i], &limbScale);
        Math_Vec3f_Copy(&this->limbScale[i], &limbScale);
    }

    this->dyna.actor.colChkInfo.damageTable = &sDamageTable;
    this->dyna.actor.colChkInfo.health = BREG(1) + 2;
    this->mainType = CARNIVOROUS_LILY_GET_TYPE(&this->dyna.actor);
    this->reviveTimer = CARNIVOROUS_LILY_GET_REVIVE_TIMER(&this->dyna.actor);
    this->switchFlag = CARNIVOROUS_LILY_GET_SWITCH_FLAG(&this->dyna.actor);
    if (this->switchFlag == CARNIVOROUS_LILY_SWITCH_FLAG_NONE) {
        this->switchFlag = SWITCH_FLAG_NONE;
    }

    if (this->reviveTimer == 31) {
        this->reviveTimer = -1;
    } else {
        this->reviveTimer = 30;
    }

    if (((this->switchFlag > SWITCH_FLAG_NONE) || (this->mainType == CARNIVOROUS_LILY_TYPE_DORMANT) ||
         CHECK_WEEKEVENTREG(WEEKEVENTREG_12_01)) &&
        (Flags_GetSwitch(play, this->switchFlag) || (this->mainType == CARNIVOROUS_LILY_TYPE_DORMANT))) {
        s32 i;

        for (i = CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT; i <= CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT;
             i++) {
            Math_Vec3f_Copy(&this->limbScale[i], &gZeroVec3f);
            Math_Vec3f_Copy(&this->targetLimbScale[i], &gZeroVec3f);
        }

        EnRaf_SetupDormant(this);
    } else {
        this->bobPhase = Rand_ZeroFloat(1.0f) * 20000.0f;
        Actor_SetScale(&this->dyna.actor, 0.01f);
        EnRaf_SetupIdle(this);
    }
}

void EnRaf_Destroy(Actor* thisx, PlayState* play) {
    EnRaf* this = (EnRaf*)thisx;
    Player* player = GET_PLAYER(play);
    if (player != NULL && player->actor.parent == thisx) {
        player->actor.parent = NULL;
        player->actor.freezeTimer = 0;
        player->av2.actionVar2 = 1000;
    }

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    Collider_DestroyCylinder(play, &this->collider);
}

typedef enum CarnivorousLilyAnimation {
    CARNIVOROUS_LILY_ANIM_IDLE,
    CARNIVOROUS_LILY_ANIM_CLOSE,
    CARNIVOROUS_LILY_ANIM_CHEW,
    CARNIVOROUS_LILY_ANIM_SPIT,
    CARNIVOROUS_LILY_ANIM_CONVULSE,
    CARNIVOROUS_LILY_ANIM_DEATH,
    CARNIVOROUS_LILY_ANIM_MAX
} CarnivorousLilyAnimation;

static AnimationHeader* sAnimations[CARNIVOROUS_LILY_ANIM_MAX] = {
    &gCarnivorousLilyPadSpitAnim, &gCarnivorousLilyPadCloseAnim,    &gCarnivorousLilyPadChewAnim,
    &gCarnivorousLilyPadSpitAnim, &gCarnivorousLilyPadConvulseAnim, &gCarnivorousLilyPadDeathAnim,
};

static u8 sAnimationModes[CARNIVOROUS_LILY_ANIM_MAX] = {
    ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE,
};

void EnRaf_ChangeAnim(EnRaf* this, s32 animIndex) {
    f32 startFrame = 0.0f;
    f32 playSpeed = 1.0f;

    this->animEndFrame = Animation_GetLastFrame(sAnimations[animIndex]);
    if (animIndex == CARNIVOROUS_LILY_ANIM_IDLE) {
        startFrame = this->animEndFrame;
    } else if (animIndex == CARNIVOROUS_LILY_ANIM_CLOSE) {
        playSpeed = 2.0f;
    }

    Animation_Change(&this->skelAnime, sAnimations[animIndex], playSpeed, startFrame, this->animEndFrame,
                     sAnimationModes[animIndex], -4.0f);
}

void EnRaf_SetupIdle(EnRaf* this) {
    Vec3f targetLimbScale = { 1.0f, 1.0f, 1.0f };
    s32 i;

    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_IDLE);

    for (i = CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT; i <= CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT; i++) {
        Math_Vec3f_Copy(&this->targetLimbScale[i], &targetLimbScale);
    }

    this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW;
    this->action = CARNIVOROUS_LILY_ACTION_IDLE;
    this->actionFunc = EnRaf_Idle;
}

void EnRaf_Idle(EnRaf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* explosive;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    if (this->timer == 0) {
        if ((PLAYER_FORM_HUMAN != PLAYER_FORM_DEKU) &&
            (this->dyna.actor.xzDistToPlayer < (BREG(48) + 80.0f) && (player->invincibilityTimer == 0) &&
             DynaPolyActor_IsPlayerOnTop(&this->dyna) && !(player->stateFlags1 & PLAYER_STATE1_8000000) &&
             play->grabPlayer(play, player))) {
            player->actor.parent = &this->dyna.actor;
            this->grabTarget = CARNIVOROUS_LILY_GRAB_TARGET_PLAYER;

            if (PLAYER_FORM_HUMAN == PLAYER_FORM_GORON) {
                this->grabTarget = CARNIVOROUS_LILY_GRAB_TARGET_GORON_PLAYER;
            } else {
                player->av2.actionVar2 = 50;
            }

            this->playerRotYWhenGrabbed = player->actor.world.rot.y;
            EnRaf_SetupGrab(this);
            return;
        }

        if ((play->gameplayFrames % 2) == 0) {
            return;
        }

        explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
        while (explosive != NULL) {

            if ((EnRaf*)explosive == this) {
                explosive = explosive->next;
                continue;
            }

            xDiff = explosive->world.pos.x - this->dyna.actor.world.pos.x;
            yDiff = explosive->world.pos.y - this->dyna.actor.world.pos.y;
            zDiff = explosive->world.pos.z - this->dyna.actor.world.pos.z;
            if ((fabsf(xDiff) < 80.0f) && (fabsf(yDiff) < 30.0f) && (fabsf(zDiff) < 80.0f) &&
                (explosive->update != NULL) && (explosive->velocity.y != 0.0f)) {
                Actor_Kill(explosive);
                this->grabTarget = CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE;
                this->collider.dim.radius = 30;
                this->collider.dim.height = 90;
                this->collider.dim.yShift = -10;
                EnRaf_SetupGrab(this);
                return;
            }

            explosive = explosive->next;
        }
    }
}

void EnRaf_SetupGrab(EnRaf* this) {
    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_CLOSE);
    this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_GRAB;
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_SUISEN_DRINK);
    this->action = CARNIVOROUS_LILY_ACTION_GRAB;
    this->actionFunc = EnRaf_Grab;
}

void EnRaf_Grab(EnRaf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;

    if ((this->grabTarget != CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE) && (player->stateFlags2 & PLAYER_STATE2_80) &&
        (&this->dyna.actor == player->actor.parent)) {
        Math_ApproachF(&player->actor.world.pos.x, this->dyna.actor.world.pos.x, 0.3f, 10.0f);
        Math_ApproachF(&player->actor.world.pos.y, this->dyna.actor.world.pos.y, 0.3f, 10.0f);
        Math_ApproachF(&player->actor.world.pos.z, this->dyna.actor.world.pos.z, 0.3f, 10.0f);
    }

    if (curFrame >= this->animEndFrame) {
        EnRaf_SetupChew(this);
    }
}

void EnRaf_SetupChew(EnRaf* this) {
    s32 i;

    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_CHEW);
    this->chewCount = 0;
    for (i = 0; i < ARRAY_COUNT(this->chewLimbRot); i++) {
        this->chewLimbRot[i].x = Rand_S16Offset(8, 8) << 8;
        this->chewLimbRot[i].y = Rand_S16Offset(8, 8) << 8;
        this->chewLimbRot[i].z = Rand_S16Offset(8, 8) << 8;
    }

    this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_CHEW;
    this->action = CARNIVOROUS_LILY_ACTION_CHEW;
    this->actionFunc = EnRaf_Chew;
}

void EnRaf_Chew(EnRaf* this, PlayState* play) {
    f32 targetChewScale;
    f32 curFrame;
    Player* player = GET_PLAYER(play);

    curFrame = this->skelAnime.curFrame;
    targetChewScale = (BREG(51) / 100.0f) + 0.2f;
    Math_ApproachF(&this->chewScale, targetChewScale, 0.2f, 0.03f);

    if ((player->stateFlags2 & PLAYER_STATE2_80) && (this->grabTarget != CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE) &&
        (&this->dyna.actor == player->actor.parent)) {
        Math_ApproachF(&player->actor.world.pos.x, this->dyna.actor.world.pos.x, 0.3f, 10.0f);
        Math_ApproachF(&player->actor.world.pos.y, this->dyna.actor.world.pos.y, 0.3f, 10.0f);
        Math_ApproachF(&player->actor.world.pos.z, this->dyna.actor.world.pos.z, 0.3f, 10.0f);
    }

    if (curFrame >= this->animEndFrame) {
        if (BREG(52) == 0) {
            this->chewCount++;
        }

        Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_SUISEN_EAT);
        switch (this->grabTarget) {
            case CARNIVOROUS_LILY_GRAB_TARGET_PLAYER:
                play->damagePlayer(play, -2);

                Actor_PlaySfx(&player->actor, NA_SE_VO_LI_DAMAGE_S + (gSaveContext.save.age == 1 ? 0x20 : 0));
                CollisionCheck_GreenBlood(play, NULL, &player->actor.world.pos);
                if ((this->chewCount > (BREG(53) + 5)) || !(player->stateFlags2 & PLAYER_STATE2_80)) {
                    player->actor.freezeTimer = 10;
                    EnRaf_SetupThrow(this, play);
                    return;
                }
                break;

            case CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE:
                Actor_ApplyDamage(&this->dyna.actor);
                if (this->chewCount > (BREG(54) + 4)) {
                    EnRaf_Explode(this, play);
                    return;
                }
                break;

            case CARNIVOROUS_LILY_GRAB_TARGET_GORON_PLAYER:
                if (this->chewCount > (BREG(54) + 4)) {
                    player->actor.parent = NULL;
                    player->av2.actionVar2 = 1000;
                    EnRaf_Explode(this, play);
                }
                break;

            default:
                break;
        }
    }
}

void EnRaf_SetupThrow(EnRaf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_SPIT);
    player->actor.freezeTimer = 10;
    this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW;
    this->action = CARNIVOROUS_LILY_ACTION_THROW;
    this->actionFunc = EnRaf_Throw;
}

void EnRaf_Throw(EnRaf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;

    if (Animation_OnFrame(&this->skelAnime, 10.0f)) {
        player->actor.freezeTimer = 0;
        player->actor.parent = NULL;
        Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_SUISEN_THROW);
        func_800B8D50(play, &this->dyna.actor, BREG(55) + 3.0f, this->playerRotYWhenGrabbed + 0x8000, BREG(56) + 10.0f,
                      0);
    } else if (curFrame < 10.0f) {
        player->actor.freezeTimer = 10;
    }

    if (curFrame >= this->animEndFrame) {
        this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW;
        this->action = CARNIVOROUS_LILY_ACTION_IDLE;
        this->timer = 20;
        this->actionFunc = EnRaf_Idle;
    }
}

void EnRaf_Explode(EnRaf* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f explosionPos;
    s32 i;
    s32 pad;

    this->action = CARNIVOROUS_LILY_ACTION_EXPLODE;
    Math_Vec3f_Copy(&explosionPos, &this->dyna.actor.world.pos);
    explosionPos.y += 10.0f;
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, explosionPos.x, explosionPos.y, explosionPos.z, 0, 0, 0,
                CLEAR_TAG_PARAMS(CLEAR_TAG_SMALL_EXPLOSION));
    Actor_PlaySfx(&this->dyna.actor, NA_SE_IT_BOMB_EXPLOSION);
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_SUISEN_DEAD);
    if (this->switchFlag > SWITCH_FLAG_NONE) {
        Flags_SetSwitch(play, this->switchFlag);
    }

    this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_DEAD;
    for (i = 0; i < BREG(57) + 30; i++) {
        accel.x = (Rand_ZeroOne() - 0.5f) * 0.5f;
        accel.y = -0.3f;
        accel.z = (Rand_ZeroOne() - 0.5f) * 0.5f;
        velocity.x = Rand_ZeroOne() - 0.5f;
        velocity.y = Rand_ZeroOne() * 10.0f;
        velocity.z = Rand_ZeroOne() - 0.5f;
        EnRaf_InitializeEffect(this, &this->dyna.actor.world.pos, &velocity, &accel,
                               (Rand_ZeroFloat(1.0f) / 500.0f) + 0.002f, 90);
    }

    for (i = CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT; i <= CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT; i++) {
        Math_Vec3f_Copy(&this->targetLimbScale[i], &gZeroVec3f);
    }

    this->timer = 5;
    if (this->grabTarget == CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->dyna.actor, ACTORCAT_ENEMY);
        this->dyna.actor.flags |= (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    }

    this->actionFunc = EnRaf_PostDetonation;
}

void EnRaf_PostDetonation(EnRaf* this, PlayState* play) {
    if (this->timer == 0) {
        this->collider.dim.radius = 50;
        this->collider.dim.height = 10;

        this->dyna.actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
        EnRaf_SetupDormant(this);
    } else if (this->grabTarget == CARNIVOROUS_LILY_GRAB_TARGET_EXPLOSIVE) {
        this->collider.dim.radius = 80;
        this->collider.dim.height = 50;
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnRaf_SetupConvulse(EnRaf* this) {
    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_CONVULSE);
    this->chewCount = 0;
    this->action = CARNIVOROUS_LILY_ACTION_CONVULSE;
    this->actionFunc = EnRaf_Convulse;
}

void EnRaf_Convulse(EnRaf* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        this->chewCount++;
        if (this->chewCount > (BREG(2) + 2)) {
            if (this->switchFlag > SWITCH_FLAG_NONE) {
                Flags_SetSwitch(play, this->switchFlag);
            }

            EnRaf_SetupDissolve(this);
        }
    }
}

void EnRaf_SetupDissolve(EnRaf* this) {
    EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_DEATH);
    this->action = CARNIVOROUS_LILY_ACTION_DISSOLVE;
    this->dissolveTimer = 0;
    this->actionFunc = EnRaf_Dissolve;
}

void EnRaf_Dissolve(EnRaf* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s32 i;

    if (curFrame >= this->animEndFrame) {
        this->dissolveTimer++;
        if (this->dissolveTimer < (BREG(3) + 105)) {
            for (i = 0; i < (BREG(4) + 5); i++) {
                EnRaf_ClearPixelPetal(Lib_SegmentedToVirtual(gCarnivorousLilyPadTrapPetalTex),
                                      sPetalClearPixelTableFirstPass, this->petalClearPixelFirstPassIndex);
                EnRaf_ClearPixelTeeth(Lib_SegmentedToVirtual(gCarnivorousLilyPadTrapTeethTex),
                                      sTeethClearPixelTableFirstPass, this->teethClearPixelFirstPassIndex);
                if (this->petalClearPixelFirstPassIndex < (16 * 32)) {
                    this->petalClearPixelFirstPassIndex++;
                }

                if (this->teethClearPixelFirstPassIndex < (8 * 8)) {
                    this->teethClearPixelFirstPassIndex++;
                }
            }
        }
    }

    if (this->dissolveTimer > (BREG(5) + 50)) {
        for (i = 0; i < (BREG(6) + 5); i++) {
            EnRaf_ClearPixelPetal(Lib_SegmentedToVirtual(gCarnivorousLilyPadTrapPetalTex),
                                  sPetalClearPixelTableSecondPass, this->petalClearPixelSecondPassIndex);
            EnRaf_ClearPixelTeeth(Lib_SegmentedToVirtual(gCarnivorousLilyPadTrapTeethTex),
                                  sTeethClearPixelTableSecondPass, this->teethClearPixelSecondPassIndex);
            if (this->petalClearPixelSecondPassIndex < (16 * 32)) {
                this->petalClearPixelSecondPassIndex++;
            }

            if (this->teethClearPixelSecondPassIndex < (8 * 8)) {
                this->teethClearPixelSecondPassIndex++;
            }
        }
    }

    if (this->dissolveTimer > (BREG(7) + 160)) {
        for (i = CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT; i <= CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT;
             i++) {
            Math_Vec3f_Copy(&this->limbScale[i], &gZeroVec3f);
            Math_Vec3f_Copy(&this->targetLimbScale[i], &gZeroVec3f);
        }

        EnRaf_SetupDormant(this);
    }
}

void EnRaf_SetupDormant(EnRaf* this) {
    if (this->action == CARNIVOROUS_LILY_ACTION_EXPLODE) {
        this->timer = 90;
    } else {
        this->action = CARNIVOROUS_LILY_ACTION_DORMANT;
    }

    this->actionFunc = EnRaf_Dormant;
}

void EnRaf_Dormant(EnRaf* this, PlayState* play) {
    Vec3f targetLimbScale = { 1.0f, 1.0f, 1.0f };
    s32 i;

    if (this->timer == 0) {
        this->action = CARNIVOROUS_LILY_ACTION_DORMANT;
    }

    if (this->reviveTimer >= 0) {
        DECR(this->reviveTimer);

        if (this->reviveTimer == 0) {
            EnRaf_ChangeAnim(this, CARNIVOROUS_LILY_ANIM_SPIT);

            for (i = CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_LOWER_SEGMENT;
                 i <= CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT; i++) {
                Math_Vec3f_Copy(&this->targetLimbScale[i], &targetLimbScale);
            }

            this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW;
            this->action = CARNIVOROUS_LILY_ACTION_IDLE;
            this->reviveTimer = CARNIVOROUS_LILY_GET_REVIVE_TIMER(&this->dyna.actor);
            this->reviveTimer += 30;
            this->actionFunc = EnRaf_Idle;
        }
    }
}

void EnRaf_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRaf* this = (EnRaf*)thisx;
    WaterBox* waterBox;
    f32 ySurface;
    Vec3f ripplePos;
    s32 i;

    SkelAnime_Update(&this->skelAnime);
    DECR(this->rippleTimer);
    DECR(this->timer);
    int terminal = this->action == CARNIVOROUS_LILY_ACTION_EXPLODE && this->actionFunc == EnRaf_Dormant;
    this->actionFunc(this, play);
    if (terminal && this->action == CARNIVOROUS_LILY_ACTION_DORMANT) {
        Actor_Kill(&this->dyna.actor);
        return;
    }

    if ((this->action == CARNIVOROUS_LILY_ACTION_IDLE) && CHECK_WEEKEVENTREG(WEEKEVENTREG_12_01)) {
        this->petalScaleType = CARNIVOROUS_LILY_PETAL_SCALE_TYPE_DEAD;
        EnRaf_SetupConvulse(this);
        return;
    }

    if (DynaPolyActor_IsPlayerOnTop(&this->dyna)) {
        if ((this->heightDiffFromPlayer > -0.1f) && !this->isCurrentlyInRidingMovingState) {
            this->heightDiffFromPlayer = -20.0f;
            this->isCurrentlyInRidingMovingState = true;
            Actor_PlaySfx(&this->dyna.actor, NA_SE_EN_COMMON_WATER_MID);
        }
    } else {
        this->isCurrentlyInRidingMovingState = false;
    }

    this->bobPhase += 3000.0f;
    this->bobOffset = 2.0f * Math_SinS(this->bobPhase);

    if (this->mainType != CARNIVOROUS_LILY_TYPE_NO_WATER_INTERACTIONS) {
        ySurface = BREG(60) + (this->dyna.actor.world.pos.y - 60.0f);
        if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->dyna.actor.world.pos.x,
                                             this->dyna.actor.world.pos.z, &ySurface, &waterBox)) {
            ySurface -= this->bobOffset + BREG(59);
            Math_ApproachF(&this->dyna.actor.world.pos.y, this->heightDiffFromPlayer + ySurface, 0.5f, 40.0f);
            if (this->rippleTimer == 0) {
                this->rippleTimer = 30;
                if (this->petalScaleType == CARNIVOROUS_LILY_PETAL_SCALE_TYPE_CHEW) {
                    this->rippleTimer = 10;
                }

                Math_Vec3f_Copy(&ripplePos, &this->dyna.actor.world.pos);
                ripplePos.y = ySurface;
                EffectSsGRipple_Spawn(play, &ripplePos, 650, 3150, 0);
            }
        }
    } else {
        Math_ApproachF(&this->dyna.actor.world.pos.y,
                       (this->dyna.actor.home.pos.y + this->heightDiffFromPlayer) - this->bobOffset, 0.5f, 40.0f);
    }

    Math_ApproachZeroF(&this->heightDiffFromPlayer, 0.3f, 2.0f);

    if (this->action == CARNIVOROUS_LILY_ACTION_EXPLODE) {
        EnRaf_UpdateEffects(this, play);
    }

    for (i = 0; i < ARRAY_COUNT(this->limbScale); i++) {
        if (this->action < CARNIVOROUS_LILY_ACTION_EXPLODE) {
            Math_ApproachF(&this->limbScale[i].x, this->targetLimbScale[i].x, 0.4f, 0.5f);
            Math_ApproachF(&this->limbScale[i].y, this->targetLimbScale[i].y, 0.4f, 0.5f);
            Math_ApproachF(&this->limbScale[i].z, this->targetLimbScale[i].z, 0.4f, 0.5f);
        } else {
            Math_ApproachF(&this->limbScale[i].x, this->targetLimbScale[i].x, 1.0f, 1.0f);
            Math_ApproachF(&this->limbScale[i].y, this->targetLimbScale[i].y, 1.0f, 1.0f);
            Math_ApproachF(&this->limbScale[i].z, this->targetLimbScale[i].z, 1.0f, 1.0f);
        }
    }

    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
    if (this->action < CARNIVOROUS_LILY_ACTION_EXPLODE) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

static s16 sGrabAnimationCheckFrames[] = { 0, 4, 6 };

static Vec3f sMiddleSegmentTargetScaleDuringGrab[] = {
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 2.0f, 1.0f },
    { 0.0f, 1.5f, 0.7f },
};

static Vec3f sUpperSegmentTargetScaleDuringGrab[] = {
    { 1.0f, 1.0f, 1.0f },
    { 3.0f, 2.0f, 1.5f },
    { 1.5f, 1.2f, 0.8f },
};

static s16 sSpitAnimationCheckFrames[] = { 0, 7, 9, 13, 19 };

static Vec3f sMiddleSegmentTargetScaleDuringSpit[] = {
    { 1.0f, 1.5f, 0.7f }, { 1.0f, 2.0f, 1.5f }, { 1.0f, 2.0f, 0.5f }, { 1.0f, 2.0f, 0.5f }, { 1.0f, 1.0f, 1.0f },
};

static Vec3f sUpperSegmentTargetScaleDuringSpit[] = {
    { 1.5f, 1.5f, 1.7f }, { 1.5f, 1.5f, 1.3f }, { 3.0f, 1.0f, 0.5f }, { 1.0f, 1.0f, 0.5f }, { 1.0f, 1.0f, 1.0f },
};

void EnRaf_TransformLimbDraw(PlayState* play2, s32 limbIndex, Actor* thisx) {
    PlayState* play = play2;
    EnRaf* this = (EnRaf*)thisx;
    s32 i;

    switch (this->petalScaleType) {
        case CARNIVOROUS_LILY_PETAL_SCALE_TYPE_GRAB:
            if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_MIDDLE_SEGMENT)) {
                for (i = 0; i < ARRAY_COUNT(sGrabAnimationCheckFrames); i++) {
                    if (TRUNCF_BINANG(this->skelAnime.curFrame) == sGrabAnimationCheckFrames[i]) {
                        Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sMiddleSegmentTargetScaleDuringGrab[i]);
                    }
                }
            }

            if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_UPPER_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_UPPER_SEGMENT)) {
                for (i = 0; i < ARRAY_COUNT(sGrabAnimationCheckFrames); i++) {
                    if (TRUNCF_BINANG(this->skelAnime.curFrame) == sGrabAnimationCheckFrames[i]) {
                        Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sUpperSegmentTargetScaleDuringGrab[i]);
                    }
                }
            }
            break;

        case CARNIVOROUS_LILY_PETAL_SCALE_TYPE_CHEW:
            if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_MIDDLE_SEGMENT)) {
                Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sMiddleSegmentTargetScaleDuringGrab[2]);
            } else if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_UPPER_SEGMENT) ||
                       (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT) ||
                       (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_UPPER_SEGMENT)) {
                Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sUpperSegmentTargetScaleDuringGrab[2]);
            }

            if ((limbIndex > CARNIVOROUS_LILY_PAD_LIMB_FLOWER) && (limbIndex < CARNIVOROUS_LILY_PAD_LIMB_ROOTS)) {
                Matrix_RotateYS((this->chewLimbRot[limbIndex].y * play->gameplayFrames), MTXMODE_APPLY);
                Matrix_RotateXS((this->chewLimbRot[limbIndex].x * play->gameplayFrames), MTXMODE_APPLY);
                Matrix_RotateZS((this->chewLimbRot[limbIndex].z * play->gameplayFrames), MTXMODE_APPLY);
                Matrix_Scale(this->chewScale + 1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
                Matrix_RotateZS(-(this->chewLimbRot[limbIndex].z * play->gameplayFrames), MTXMODE_APPLY);
                Matrix_RotateXS(-(this->chewLimbRot[limbIndex].x * play->gameplayFrames), MTXMODE_APPLY);
                Matrix_RotateYS(-(this->chewLimbRot[limbIndex].y * play->gameplayFrames), MTXMODE_APPLY);
            }
            break;

        case CARNIVOROUS_LILY_PETAL_SCALE_TYPE_IDLE_OR_THROW:
            if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_MIDDLE_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_MIDDLE_SEGMENT)) {
                for (i = 0; i < ARRAY_COUNT(sSpitAnimationCheckFrames); i++) {
                    if (TRUNCF_BINANG(this->skelAnime.curFrame) == sSpitAnimationCheckFrames[i]) {
                        Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sMiddleSegmentTargetScaleDuringSpit[i]);
                    }
                }
            }

            if ((limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_1_UPPER_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_3_UPPER_SEGMENT) ||
                (limbIndex == CARNIVOROUS_LILY_PAD_LIMB_TRAP_2_UPPER_SEGMENT)) {
                for (i = 0; i < 4; i++) {
                    if (TRUNCF_BINANG(this->skelAnime.curFrame) == sSpitAnimationCheckFrames[i]) {
                        Math_Vec3f_Copy(&this->targetLimbScale[limbIndex], &sUpperSegmentTargetScaleDuringSpit[i]);
                    }
                }
            }
            break;

        default:
            break;
    }

    Matrix_Scale(this->limbScale[limbIndex].x, this->limbScale[limbIndex].y, this->limbScale[limbIndex].z,
                 MTXMODE_APPLY);
}

void EnRaf_Draw(Actor* thisx, PlayState* play) {
    EnRaf* this = (EnRaf*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawTransformFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                   this->skelAnime.dListCount, NULL, NULL, EnRaf_TransformLimbDraw, &this->dyna.actor);

    if (this->action == CARNIVOROUS_LILY_ACTION_EXPLODE) {
        EnRaf_DrawEffects(this, play);
    }
}

void EnRaf_InitializeEffect(EnRaf* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 timer) {
    s16 i;
    CarnivorousLilyEffect* effect = this->effects;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isEnabled) {
            effect->isEnabled = true;
            effect->pos = *pos;
            effect->velocity = *velocity;
            effect->accel = *accel;
            effect->scale = scale;
            effect->timer = timer;
            effect->rot.x = Rand_CenteredFloat(30000.0f);
            effect->rot.y = Rand_CenteredFloat(30000.0f);
            effect->rot.z = Rand_CenteredFloat(30000.0f);
            return;
        }
    }
}

void EnRaf_UpdateEffects(EnRaf* this, PlayState* play) {
    s32 i;
    CarnivorousLilyEffect* effect = this->effects;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isEnabled) {
            effect->pos.x += effect->velocity.x;
            effect->pos.y += effect->velocity.y;
            effect->pos.z += effect->velocity.z;
            effect->rot.x += 0xBB8;
            effect->rot.y += 0xBB8;
            effect->rot.z += 0xBB8;
            effect->velocity.x += effect->accel.x;
            effect->velocity.y += effect->accel.y;
            effect->velocity.z += effect->accel.z;

            if (this->mainType != CARNIVOROUS_LILY_TYPE_NO_WATER_INTERACTIONS) {
                if (effect->pos.y < (this->dyna.actor.world.pos.y - 10.0f)) {
                    EffectSsGSplash_Spawn(play, &effect->pos, NULL, NULL, 0, effect->scale * 200000.0f);
                    SoundSource_PlaySfxAtFixedWorldPos(play, &effect->pos, 50, NA_SE_EV_BOMB_DROP_WATER);
                    effect->isEnabled = false;
                }
            } else if (effect->pos.y < (this->dyna.actor.world.pos.y - 10.0f)) {
                Math_ApproachZeroF(&effect->scale, 0.2f, 0.001f);
                if (effect->scale <= 0.0001f) {
                    effect->timer = 0;
                }
            }

            if (effect->timer != 0) {
                effect->timer--;
            } else {
                effect->isEnabled = false;
            }
        }
    }
}

void EnRaf_DrawEffects(EnRaf* this, PlayState* play) {
    s16 i;
    CarnivorousLilyEffect* effect = this->effects;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isEnabled) {
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
            Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gCarnivorousLilyPadParticleDL);
        }
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x13a, En_Raf_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_RAF, En_Raf_Profile, EnRaf_Init, EnRaf_Destroy, EnRaf_Update, EnRaf_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 1, 0x13a, 0x161, 0xf547c0eau };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_RAF_ORIGIN, generatedNativeOrigin);
