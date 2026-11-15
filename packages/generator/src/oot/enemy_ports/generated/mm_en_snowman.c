#include "mm_native_compat.h"
#define gEenoLargeSnowballCreateAnim (*(AnimationHeader*)0x06000404)
#define gEenoSmallSnowballCreateAnim (*(AnimationHeader*)0x060007b4)
#define gEenoLargeSnowballDL (*((Gfx (*)[1])0x060010b0))
#define gEenoSmallSnowballDL (*((Gfx (*)[1])0x06004400))
#define gEenoSkel (*(FlexSkeletonHeader*)0x060045a0)
#define gEenoDamageAnim (*(AnimationHeader*)0x06004628)
#define gEenoSnowPileMoveAnim (*(AnimationHeader*)0x060046d8)
#define gEenoSnowPileSkel (*(FlexSkeletonHeader*)0x06004a90)
#define gEenoLargeSnowballThrowAnim (*(AnimationHeader*)0x06004f14)
#define gEenoSmallSnowballThrowAnim (*(AnimationHeader*)0x0600544c)
#define gEenoEmergeAnim (*(AnimationHeader*)0x0600554c)
#define gEenoIdleAnim (*(AnimationHeader*)0x060058cc)
#define gEenoSnowballFragment1DL (*((Gfx (*)[1])0x06005cb0))
#define gEenoSnowballFragment2DL (*((Gfx (*)[1])0x06006190))
#define gEenoSnowballFragment3DL (*((Gfx (*)[1])0x06006620))

#include "types/en_snowman.h"
#define TATL_HINT_ID_EENO 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_SNOWMAN
#define ACTOR_EN_SNOWMAN 0x1e6
#undef OBJECT_SNOWMAN
#define OBJECT_SNOWMAN 0x11c4
#undef NA_SE_IT_REFLECTION_WOOD
#define NA_SE_IT_REFLECTION_WOOD 0x1837
#undef NA_SE_EV_SNOWBALL_BROKEN
#define NA_SE_EV_SNOWBALL_BROKEN 0x28f5
#undef NA_SE_EV_SMALL_SNOWBALL_BROKEN
#define NA_SE_EV_SMALL_SNOWBALL_BROKEN 0x28fd
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_YMAJIN_HOLD_SNOW
#define NA_SE_EN_YMAJIN_HOLD_SNOW 0x3944
#undef NA_SE_EN_YMAJIN_MINI_HOLD
#define NA_SE_EN_YMAJIN_MINI_HOLD 0x3967
#undef NA_SE_EN_YMAJIN_MINI_THROW
#define NA_SE_EN_YMAJIN_MINI_THROW 0x3968
#undef NA_SE_EN_YMAJIN_MOVE
#define NA_SE_EN_YMAJIN_MOVE 0x3969
#undef NA_SE_EN_YMAJIN_MINI_MOVE
#define NA_SE_EN_YMAJIN_MINI_MOVE 0x396a
#undef NA_SE_EN_YMAJIN_SURFACE
#define NA_SE_EN_YMAJIN_SURFACE 0x396b
#undef NA_SE_EN_YMAJIN_HIDE
#define NA_SE_EN_YMAJIN_HIDE 0x396c
#undef NA_SE_EN_YMAJIN_SPLIT
#define NA_SE_EN_YMAJIN_SPLIT 0x396d
#undef NA_SE_EN_YMAJIN_UNITE
#define NA_SE_EN_YMAJIN_UNITE 0x396e
#undef NA_SE_EN_YMAJIN_DEAD_BREAK
#define NA_SE_EN_YMAJIN_DEAD_BREAK 0x396f
#undef NA_SE_EN_YMAJIN_MINI_DAMAGE
#define NA_SE_EN_YMAJIN_MINI_DAMAGE 0x39f9
#undef NA_SE_EN_YMAJIN_DAMAGE
#define NA_SE_EN_YMAJIN_DAMAGE 0x39fa
#undef NA_SE_EN_YMAJIN_THROW
#define NA_SE_EN_YMAJIN_THROW 0x3a50
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Actor_SpawnAsChild
#define Actor_SpawnAsChild ((Actor* (*)(ActorContext* actorCtx, Actor* parent, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN_CHILD)
#undef Actor_SpawnAsChildAndCutscene
#define Actor_SpawnAsChildAndCutscene ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 index, f32 x, f32 y, f32 z, s16 rotX, s16 rotY, s16 rotZ, s32 params, u32 csId, u32 halfDaysBits, Actor* parent))(void*)FOREIGN_ENEMY_API_SPAWN_CUTSCENE)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef EffectSsHahen_Spawn
extern u8 FOREIGN_ENEMY_API_SNOW_FRAGMENT[];
#define EffectSsHahen_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 flags, s16 scale, s16 objectId, s16 life, Gfx* dList))(void*)FOREIGN_ENEMY_API_SNOW_FRAGMENT)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef func_800BE504
#define func_800BE504 ((void (*)(Actor* actor, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_HIT_YAW_CYLINDER)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef Actor_SetDropFlag
#define Actor_SetDropFlag ((void (*)(Actor* actor, ColliderElement* elem))(void*)FOREIGN_ENEMY_API_DROP_FLAG)
#undef Math_Atan2S_XY
#define Math_Atan2S_XY ((s16 (*)(f32 x, f32 y))(void*)FOREIGN_ENEMY_API_ATAN2S_XY)
#undef func_800BE3D0
extern u8 FOREIGN_ENEMY_API_FLOOR_ROTATION[];
#define func_800BE3D0 ((void (*)(Actor* actor, s16 angle, Vec3s* arg2))(void*)FOREIGN_ENEMY_API_FLOOR_ROTATION)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef EffectSsHahen_SpawnBurst
extern u8 FOREIGN_ENEMY_API_SNOW_FRAGMENT_BURST[];
#define EffectSsHahen_SpawnBurst ((void (*)(struct PlayState* play, Vec3f* pos, f32 burstScale, s16 flags, s16 scale, s16 randScaleRange, s16 count, s16 objectId, s16 life, Gfx* dList))(void*)FOREIGN_ENEMY_API_SNOW_FRAGMENT_BURST)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_WorldYawTowardActor
#define Actor_WorldYawTowardActor ((s16 (*)(Actor* actorA, Actor* actorB))0x800213b4u)
#undef Actor_WorldDistXZToActor
#define Actor_WorldDistXZToActor ((f32 (*)(Actor* actorA, Actor* actorB))0x800214fcu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef func_800B0DE0
#define func_800B0DE0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep))0x8001bd94u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d2d4u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef EffectSsIceSmoke_Spawn
#define EffectSsIceSmoke_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale))0x8001e560u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef Gfx_DrawDListOpa
#define Gfx_DrawDListOpa ((void (*)(struct PlayState* play, Gfx* dList))0x80028048u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnSnowman_Init(Actor* thisx, PlayState* play);
void EnSnowman_Destroy(Actor* thisx, PlayState* play);
void EnSnowman_Update(Actor* thisx, PlayState* play);
void EnSnowman_Draw(Actor* thisx, PlayState* play);

void EnSnowman_SetupMoveSnowPile(EnSnowman* this);
void EnSnowman_MoveSnowPile(EnSnowman* this, PlayState* play);
void EnSnowman_SetupEmerge(EnSnowman* this, PlayState* play);
void EnSnowman_Emerge(EnSnowman* this, PlayState* play);
void EnSnowman_SetupReadySnowball(EnSnowman* this);
void EnSnowman_ReadySnowball(EnSnowman* this, PlayState* play);
void EnSnowman_SetupThrowSnowball(EnSnowman* this);
void EnSnowman_ThrowSnowball(EnSnowman* this, PlayState* play);
void EnSnowman_SetupIdle(EnSnowman* this);
void EnSnowman_Idle(EnSnowman* this, PlayState* play);
void EnSnowman_SetupSubmerge(EnSnowman* this, PlayState* play);
void EnSnowman_Submerge(EnSnowman* this, PlayState* play);
void EnSnowman_Melt(EnSnowman* this, PlayState* play);
void EnSnowman_Stun(EnSnowman* this, PlayState* play);
void EnSnowman_Damaged(EnSnowman* this, PlayState* play);
void EnSnowman_SetupDead(EnSnowman* this);
void EnSnowman_Dead(EnSnowman* this, PlayState* play);
void EnSnowman_SetupSplitDoNothing(EnSnowman* this);
void EnSnowman_SplitDoNothing(EnSnowman* this, PlayState* play);
void EnSnowman_SetupKill(EnSnowman* this);
void EnSnowman_Kill(EnSnowman* this, PlayState* play);
int FOREIGN_ENEMY_API_EENO_REPLACE_SPLIT(Actor*, PlayState*);
void EnSnowman_CreateSplitEeno(EnSnowman* this, Vec3f* basePos, s32 yRot);
void EnSnowman_SetupCombine(EnSnowman* this, PlayState* play, Vec3f* combinePos);
void EnSnowman_Combine(EnSnowman* this, PlayState* play);
void EnSnowman_UpdateSnowball(Actor* thisx, PlayState* play);
void EnSnowman_DrawSnowPile(Actor* thisx, PlayState* play);
void EnSnowman_DrawSnowball(Actor* thisx, PlayState* play);

typedef enum {
    EN_SNOWMAN_COMBINE_STATE_NONE,

    EN_SNOWMAN_COMBINE_STATE_ACTIVE,

    EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION,

    EN_SNOWMAN_COMBINE_STATE_BEING_ABSORBED_OR_DONE
} EnSnowmanCombineState;

ActorProfile En_Snowman_Profile = {
    0xde6,
    ACTORCAT_ENEMY,
    FLAGS,
    0x11c4,
    sizeof(EnSnowman),
    EnSnowman_Init,
    EnSnowman_Destroy,
    EnSnowman_Update,
    EnSnowman_Draw,
};

static ColliderCylinderInit sEenoCylinderInit = {
    {
        COL_MATERIAL_HIT4,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0.0f, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 60, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sSnowballCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 60, 80, 0, { 0, 0, 0 } },
};

typedef enum {
    EN_SNOWMAN_DMGEFF_NONE,
    EN_SNOWMAN_DMGEFF_STUN,
    EN_SNOWMAN_DMGEFF_MELT,
    EN_SNOWMAN_DMGEFF_LIGHT_ORB = 0x4,
    EN_SNOWMAN_DMGEFF_ELECTRIC_STUN,
    EN_SNOWMAN_DMGEFF_HOOKSHOT = 0xF
} EnTalkGibudDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_STUN), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_HOOKSHOT),
    DMG_ENTRY(2, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(2, EN_SNOWMAN_DMGEFF_MELT),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(2, EN_SNOWMAN_DMGEFF_LIGHT_ORB),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_STUN),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(2, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_STUN), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_ELECTRIC_STUN),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE),
    DMG_ENTRY(0, EN_SNOWMAN_DMGEFF_NONE), DMG_ENTRY(1, EN_SNOWMAN_DMGEFF_NONE),
} };

static CollisionCheckInfoInit sColChkInfoInit = { 2, 60, 80, 150 };

static Color_RGBA8 sDustPrimColor = { 250, 250, 250, 255 };

static Color_RGBA8 sDustEnvColor = { 180, 180, 180, 255 };

static Vec3f sDustVelocity = { 0.0f, 1.5f, 0.0f };

static Gfx* sSnowballDLs[] = {
    gEenoSmallSnowballDL,
    gEenoLargeSnowballDL,
    gEenoSmallSnowballDL,
};

static Gfx* sSnowballFragmentDLs[] = {
    gEenoSnowballFragment1DL,
    gEenoSnowballFragment2DL,
    gEenoSnowballFragment3DL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_EENO, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_STOP),
};

void EnSnowman_Init(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;
    s32 attackRange;

    Actor_ProcessInitChain(thisx, sInitChain);
    attackRange = EN_SNOWMAN_GET_ATTACK_RANGE(thisx);
    if (attackRange == 0xFF) {
        attackRange = 0;
    }

    thisx->params &= 7;
    if (EN_SNOWMAN_GET_TYPE(thisx) < EN_SNOWMAN_TYPE_SMALL_SNOWBALL) {
        SkelAnime_InitFlex(play, &this->skelAnime, &gEenoSkel, &gEenoEmergeAnim, this->jointTable, this->morphTable,
                           EENO_LIMB_MAX);
        SkelAnime_InitFlex(play, &this->snowPileSkelAnime, &gEenoSnowPileSkel, &gEenoSnowPileMoveAnim,
                           this->snowPileJointTable, this->snowPileMorphTable, EENO_SNOW_PILE_LIMB_MAX);
        CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);
        Collider_InitAndSetCylinder(play, &this->collider, thisx, &sEenoCylinderInit);
        if (EN_SNOWMAN_GET_TYPE(thisx) == EN_SNOWMAN_TYPE_LARGE) {
            thisx->flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
            Actor_SpawnAsChild(&play->actorCtx, thisx, play, ACTOR_EN_SNOWMAN, thisx->world.pos.x, thisx->world.pos.y,
                               thisx->world.pos.z, 0, 0, 0, EN_SNOWMAN_TYPE_SPLIT);
            thisx->parent = Actor_SpawnAsChildAndCutscene(
                &play->actorCtx, play, ACTOR_EN_SNOWMAN, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 0,
                0, 0, EN_SNOWMAN_TYPE_SPLIT, CS_ID_NONE, comboEnemySourceHalfDays(thisx), NULL);
            if ((thisx->child == NULL) || (thisx->parent == NULL)) {
                if (thisx->child != NULL) {
                    Actor_Kill(thisx->child);
                }

                if (thisx->parent != NULL) {
                    Actor_Kill(thisx->parent);
                }

                Actor_Kill(thisx);
                return;
            }

            thisx->parent->child = thisx;
            thisx->child->child = thisx->parent;
            thisx->parent->parent = thisx->child;

            if (1) {
            }

            Actor_SetScale(thisx, 0.02f);
        }

        this->eenoScale = thisx->scale.x * 100.0f;
        this->attackRange = (240.0f * this->eenoScale) + (attackRange * 0.1f * 40.0f);
        if (EN_SNOWMAN_GET_TYPE(thisx) == EN_SNOWMAN_TYPE_SPLIT) {
            EnSnowman_SetupSplitDoNothing(this);
        } else {
            EnSnowman_SetupMoveSnowPile(this);
        }
    } else {
        Player* player = GET_PLAYER(play);

        thisx->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        Collider_InitAndSetCylinder(play, &this->collider, thisx, &sSnowballCylinderInit);
        thisx->world.rot.y = Actor_WorldYawTowardActor(thisx, &player->actor);
        thisx->velocity.y = (Actor_WorldDistXZToActor(thisx, &player->actor) * 0.035f) + -5.0f;
        thisx->velocity.y = CLAMP_MAX(thisx->velocity.y, 3.5f);
        if (EN_SNOWMAN_GET_TYPE(thisx) == EN_SNOWMAN_TYPE_SMALL_SNOWBALL) {
            thisx->speed = 15.0f;
        } else {
            thisx->speed = 22.5f;
            thisx->velocity.y *= 1.5f;
        }

        thisx->world.pos.x += thisx->speed * Math_SinS(thisx->world.rot.y);
        thisx->world.pos.y += thisx->velocity.y;
        thisx->world.pos.z += thisx->speed * Math_CosS(thisx->world.rot.y);

        if (EN_SNOWMAN_GET_TYPE(thisx) == EN_SNOWMAN_TYPE_SMALL_SNOWBALL) {
            this->collider.dim.radius = 8;
            this->collider.dim.height = 12;
            this->collider.dim.yShift = -6;
            ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 10.0f);
        } else {
            this->collider.dim.radius = 50;
            this->collider.dim.height = 122;
            this->collider.dim.yShift = -8;
            this->collider.elem.atDmgInfo.damage = 16;
            thisx->world.pos.y -= 32.0f;
            Actor_SetScale(thisx, 0.006f);
            ActorShape_Init(&thisx->shape, 16000.0f / 3.0f, ActorShadow_DrawCircle, 170.0f);
            thisx->gravity = -1.5f;
        }

        thisx->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        thisx->update = EnSnowman_UpdateSnowball;
        thisx->draw = EnSnowman_DrawSnowball;
        this->work.timer = 5;
    }
}

void EnSnowman_Destroy(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnSnowman_SpawnCircularDustEffect(EnSnowman* this, PlayState* play) {
    s16 angle = 0;
    Vec3f pos;
    f32 offset;
    s32 i;

    pos.y = (Rand_ZeroFloat(10.0f) * this->eenoScale) + this->actor.world.pos.y;
    for (i = 0; i < 16; i++) {
        offset = (Rand_ZeroFloat(10.0f) + 20.0f) * this->eenoScale;
        pos.x = (Math_SinS(angle) * offset) + this->actor.world.pos.x;
        pos.z = (Math_CosS(angle) * offset) + this->actor.world.pos.z;
        func_800B0DE0(play, &pos, &sDustVelocity, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor,
                      this->eenoScale * 400.0f, 10);
        angle += 0x1000;
    }
}

void EnSnowman_SpawnBigSnowballFragmentEffects(EnSnowman* this, PlayState* play) {
    static Vec3f sAccel = { 0.0f, -1.0f, 0.0f };
    s16 altitude;
    s16 azimuth;
    Vec3f pos;
    Vec3f velocity;
    f32 speed;
    s32 i;

    for (i = 0; i < 15; i++) {
        altitude = Rand_S16Offset(0x1800, 0x2800);
        azimuth = Rand_Next() >> 0x10;
        speed = Rand_ZeroFloat(3.0f) + 8.0f;
        velocity.x = (speed * Math_CosS(altitude)) * Math_SinS(azimuth);
        velocity.y = speed * Math_SinS(altitude);
        velocity.z = (speed * Math_CosS(altitude)) * Math_CosS(azimuth);
        pos.x = (Rand_ZeroFloat(10.0f) * velocity.x) + this->snowballPos.x;
        pos.y = (Rand_ZeroFloat(8.0f) * velocity.y) + this->snowballPos.y;
        pos.z = (Rand_ZeroFloat(10.0f) * velocity.z) + this->snowballPos.z;
        EffectSsHahen_Spawn(play, &pos, &velocity, &sAccel, 0,
                            Rand_S16Offset((((i % 3) * 50) + 50), (((i % 3) * 25) + 25)), OBJECT_SNOWMAN, 20,
                            sSnowballFragmentDLs[i % 3]);
    }

    func_800B0DE0(play, &this->snowballPos, &gZeroVec3f, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor, 1000, 150);
}

void EnSnowman_SetupMoveSnowPile(EnSnowman* this) {
    Animation_PlayLoop(&this->snowPileSkelAnime, &gEenoSnowPileMoveAnim);
    this->actor.scale.y = this->actor.scale.x;
    this->actor.speed = 2.0f;
    this->actor.draw = EnSnowman_DrawSnowPile;
    this->work.timer = 40;
    this->turningOnSteepSlope = false;
    this->collider.dim.radius = this->eenoScale * 30.0f;
    this->collider.dim.height = this->eenoScale * 10.0f;
    this->actionFunc = EnSnowman_MoveSnowPile;
}

void EnSnowman_MoveSnowPile(EnSnowman* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f combinePos;

    SkelAnime_Update(&this->snowPileSkelAnime);
    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_YMAJIN_MOVE - SFX_FLAG);
    } else {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_YMAJIN_MINI_MOVE - SFX_FLAG);
    }

    if (this->work.timer > 0) {
        this->work.timer--;
    }

    if (this->work.timer == 0) {
        this->collider.base.acFlags |= AC_ON;
    }

    if ((this->combineTimer == 0) && (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_SPLIT)) {
        if ((this->actor.parent->colChkInfo.health != 0) && (this->actor.child->colChkInfo.health != 0)) {
            combinePos.x =
                (this->actor.parent->world.pos.x + this->actor.child->world.pos.x + this->actor.world.pos.x) *
                (1.0f / 3.0f);
            combinePos.z =
                (this->actor.parent->world.pos.z + this->actor.child->world.pos.z + this->actor.world.pos.z) *
                (1.0f / 3.0f);
        } else if (this->actor.parent->colChkInfo.health != 0) {
            combinePos.x = (this->actor.parent->world.pos.x + this->actor.world.pos.x) / 2.0f;
            combinePos.z = (this->actor.parent->world.pos.z + this->actor.world.pos.z) / 2.0f;
        } else if (this->actor.child->colChkInfo.health != 0) {
            combinePos.x = (this->actor.child->world.pos.x + this->actor.world.pos.x) / 2.0f;
            combinePos.z = (this->actor.child->world.pos.z + this->actor.world.pos.z) / 2.0f;
        } else {
            combinePos.x = this->actor.world.pos.x;
            combinePos.z = this->actor.world.pos.z;
        }

        combinePos.y = this->actor.world.pos.y;
        EnSnowman_SetupCombine((EnSnowman*)this->actor.parent, play, &combinePos);
        EnSnowman_SetupCombine((EnSnowman*)this->actor.child, play, &combinePos);
        EnSnowman_SetupCombine(this, play, &combinePos);
    } else if ((this->work.timer == 0) && (fabsf(this->actor.yDistanceFromLink) < 60.0f) &&
               (this->actor.xzDistToPlayer < this->attackRange) && (Player_GetMask(play) != PLAYER_MASK_STONE) &&
               !(player->stateFlags1 & PLAYER_STATE1_800000)) {
        EnSnowman_SetupEmerge(this, play);
    } else if (this->snowPileTargetRotY != this->actor.shape.rot.y) {
        if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->snowPileTargetRotY, 0x100)) {
            this->turningOnSteepSlope = false;
        }

        this->actor.world.rot.y = this->actor.shape.rot.y;
    } else if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->snowPileTargetRotY = this->actor.wallYaw;
    } else if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 200.0f) {
        this->snowPileTargetRotY =
            Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) + ((s32)Rand_Next() >> 0x14);
    } else if (Rand_ZeroOne() < 0.02f) {
        this->snowPileTargetRotY += (s16)(((Rand_Next() >> 0x13) + 0x1000) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1));
    }
}

void EnSnowman_SetupEmerge(EnSnowman* this, PlayState* play) {
    Animation_PlayOnce(&this->skelAnime, &gEenoEmergeAnim);
    Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_SURFACE);
    this->collider.dim.radius = this->eenoScale * 40.0f;
    this->collider.dim.height = this->eenoScale * 25.0f;
    this->actor.draw = EnSnowman_Draw;
    this->actor.scale.y = this->actor.scale.x * 0.4f;
    this->actor.speed = 0.0f;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    EnSnowman_SpawnCircularDustEffect(this, play);
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnSnowman_Emerge;
}

void EnSnowman_Emerge(EnSnowman* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_StepToF(&this->actor.scale.y, this->actor.scale.x,
                 (this->actor.scale.x * 0.6f) / Animation_GetLastFrame(&gEenoEmergeAnim));
    Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0x1000);

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE) {
            EnSnowman_SetupSubmerge(this, play);
        } else if (!(player->stateFlags1 & PLAYER_STATE1_800000) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
            this->collider.base.acFlags |= AC_ON;
            this->work.snowballsToThrowBeforeIdling = 3;
            EnSnowman_SetupReadySnowball(this);
        } else {
            this->collider.base.acFlags |= AC_ON;
            this->actor.scale.y = this->actor.scale.x;
            EnSnowman_SetupIdle(this);
        }
    }
}

void EnSnowman_SetupReadySnowball(EnSnowman* this) {
    this->actor.scale.y = this->actor.scale.x;
    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        this->fwork.frameToStartHoldingSnowball = 15.0f;
        Animation_PlayOnce(&this->skelAnime, &gEenoLargeSnowballCreateAnim);
    } else {
        this->fwork.frameToStartHoldingSnowball = 6.0f;
        Animation_PlayOnce(&this->skelAnime, &gEenoSmallSnowballCreateAnim);
    }

    this->actionFunc = EnSnowman_ReadySnowball;
}

void EnSnowman_ReadySnowball(EnSnowman* this, PlayState* play) {
    Vec3f pos;

    Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0x1000);
    if ((EN_SNOWMAN_GET_TYPE(&this->actor) != EN_SNOWMAN_TYPE_LARGE) && this->isHoldingSnowball &&
        ((play->gameplayFrames % 2) != 0)) {
        pos.x = Rand_CenteredFloat(10.0f) + this->snowballPos.x;
        pos.y = Rand_CenteredFloat(10.0f) + this->snowballPos.y;
        pos.z = Rand_CenteredFloat(10.0f) + this->snowballPos.z;
        func_800B0DE0(play, &pos, &sDustVelocity, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor, 500, 30);
    } else if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        if ((this->skelAnime.curFrame > 3.0f) && (this->skelAnime.curFrame < 14.0f) &&
            ((play->gameplayFrames % 2) != 0)) {
            pos.x =
                (this->actor.world.pos.x + (70.0f * Math_SinS(this->actor.shape.rot.y))) + Rand_CenteredFloat(40.0f);
            pos.y = this->actor.world.pos.y + Rand_CenteredFloat(20.0f);
            pos.z =
                (this->actor.world.pos.z + (70.0f * Math_CosS(this->actor.shape.rot.y))) + Rand_CenteredFloat(40.0f);
            func_800B0DE0(play, &pos, &sDustVelocity, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor, 1000, 150);
        }
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        EnSnowman_SetupThrowSnowball(this);
    } else if (Animation_OnFrame(&this->skelAnime, this->fwork.frameToStartHoldingSnowball)) {
        if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_HOLD_SNOW);
        } else {
            Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_MINI_HOLD);
        }

        this->isHoldingSnowball = true;
    }
}

void EnSnowman_SetupThrowSnowball(EnSnowman* this) {
    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        Animation_PlayOnce(&this->skelAnime, &gEenoLargeSnowballThrowAnim);
        this->fwork.frameToThrowSnowball = 17.0f;
    } else {
        Animation_PlayOnce(&this->skelAnime, &gEenoSmallSnowballThrowAnim);
        this->fwork.frameToThrowSnowball = 15.0f;
    }

    this->work.snowballsToThrowBeforeIdling--;
    this->actionFunc = EnSnowman_ThrowSnowball;
}

void EnSnowman_ThrowSnowball(EnSnowman* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 params;

    Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0x1000);
    if (SkelAnime_Update(&this->skelAnime)) {
        if ((this->work.snowballsToThrowBeforeIdling != 0) && (Player_GetMask(play) != PLAYER_MASK_STONE) &&
            !(player->stateFlags1 & PLAYER_STATE1_800000)) {
            EnSnowman_SetupReadySnowball(this);
        } else {
            this->work.snowballsToThrowBeforeIdling = 0;
            EnSnowman_SetupIdle(this);
        }
    } else if (Animation_OnFrame(&this->skelAnime, this->fwork.frameToThrowSnowball)) {
        this->isHoldingSnowball = false;
        if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
            params = EN_SNOWMAN_TYPE_LARGE_SNOWBALL;
            Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_THROW);
        } else {
            params = EN_SNOWMAN_TYPE_SMALL_SNOWBALL;
            Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_MINI_THROW);
        }

        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SNOWMAN, this->snowballPos.x, this->snowballPos.y,
                    this->snowballPos.z, 0, this->actor.yawTowardsPlayer, 0, params);
    }
}

void EnSnowman_SetupIdle(EnSnowman* this) {
    Animation_PlayLoop(&this->skelAnime, &gEenoIdleAnim);
    this->work.timer = 60;
    this->actionFunc = EnSnowman_Idle;
}

void EnSnowman_Idle(EnSnowman* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (this->work.timer > 0) {
        this->work.timer--;
    } else {
        EnSnowman_SetupSubmerge(this, play);
    }
}

void EnSnowman_SetupSubmerge(EnSnowman* this, PlayState* play) {
    Animation_Change(&this->skelAnime, &gEenoEmergeAnim, -1.0f, Animation_GetLastFrame(&gEenoEmergeAnim), 0.0f,
                     ANIMMODE_ONCE, -3.0f);
    EnSnowman_SpawnCircularDustEffect(this, play);
    Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_HIDE);
    this->actionFunc = EnSnowman_Submerge;
}

void EnSnowman_Submerge(EnSnowman* this, PlayState* play) {
    Math_StepToF(&this->actor.scale.y, this->actor.scale.x * 0.4f,
                 (this->actor.scale.x * 0.6f) / Animation_GetLastFrame(&gEenoEmergeAnim));

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE) {
            this->actor.draw = EnSnowman_DrawSnowPile;
            this->collider.base.acFlags |= AC_ON;

            EnSnowman_SetupCombine(this, play, &this->combinePos);
        } else {
            EnSnowman_SetupMoveSnowPile(this);
        }
    }
}

void EnSnowman_SetupMelt(EnSnowman* this) {
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 50);
    this->collider.base.acFlags &= ~AC_ON;
    this->work.timer = 50;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actor.scale.y = this->actor.scale.x;
    this->actor.speed = 0.0f;
    this->actionFunc = EnSnowman_Melt;
}

void EnSnowman_Melt(EnSnowman* this, PlayState* play) {
    Vec3f smokeVelocity;
    Vec3f smokePos;

    this->work.timer--;
    if ((this->work.timer >= 38) && !(this->work.timer & 1)) {
        smokeVelocity.y = (this->work.timer - 38) * (1.0f / 12.0f);
        smokeVelocity.x = Rand_CenteredFloat(1.5f) * smokeVelocity.y;
        smokeVelocity.z = Rand_CenteredFloat(1.5f) * smokeVelocity.y;
        smokeVelocity.y += 0.8f;
        smokePos.x = ((smokeVelocity.x >= 0.0f ? 1.0f : -1.0f) * Rand_ZeroFloat(20.0f) * this->eenoScale) +
                     this->actor.world.pos.x;
        smokePos.z = ((smokeVelocity.z >= 0.0f ? 1.0f : -1.0f) * Rand_ZeroFloat(20.0f) * this->eenoScale) +
                     this->actor.world.pos.z;
        smokePos.y = this->actor.world.pos.y + 3.0f;
        EffectSsIceSmoke_Spawn(play, &smokePos, &smokeVelocity, &gZeroVec3f, this->eenoScale * 300.0f);
    }

    if (this->work.timer == 0) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x60);
        if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_SPLIT) {
            EnSnowman_SetupSplitDoNothing(this);
        } else if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
            Actor_Kill(this->actor.parent);
            Actor_Kill(this->actor.child);
            Actor_Kill(&this->actor);
        } else {
            Actor_Kill(&this->actor);
        }
    }

    this->actor.scale.y = this->work.timer * 0.0002f * this->eenoScale;
    this->actor.scale.x = (this->eenoScale * (1.4f * 0.01f)) - (0.4f * this->actor.scale.y);
    this->actor.scale.z = (this->eenoScale * (1.4f * 0.01f)) - (0.4f * this->actor.scale.y);
}

void EnSnowman_SetupStun(EnSnowman* this) {
    if (this->actionFunc != EnSnowman_Stun) {
        this->prevActionFunc = this->actionFunc;
    }

    this->actionFunc = EnSnowman_Stun;
}

void EnSnowman_Stun(EnSnowman* this, PlayState* play) {
    if (this->actor.colorFilterTimer == 0) {
        this->actionFunc = this->prevActionFunc;
    }
}

void EnSnowman_SetupDamaged(EnSnowman* this) {
    Animation_PlayLoop(&this->skelAnime, &gEenoDamageAnim);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 20);
    this->collider.base.acFlags &= ~AC_ON;
    this->work.timer = 20;
    this->actor.draw = EnSnowman_Draw;
    this->actor.scale.y = this->actor.scale.x;
    this->actor.speed = 10.0f;
    func_800BE504(&this->actor, &this->collider);

    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_DAMAGE);
    } else {
        Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_MINI_DAMAGE);
    }

    this->actionFunc = EnSnowman_Damaged;
}

void EnSnowman_Damaged(EnSnowman* this, PlayState* play) {
    s32 angularVelocityScale;

    SkelAnime_Update(&this->skelAnime);
    angularVelocityScale = CLAMP_MAX(this->work.timer, 10);
    this->actor.shape.rot.y += angularVelocityScale * 0x300;
    Math_StepToF(&this->actor.speed, 0.0f, 0.5f);

    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        Math_StepToF(&this->actor.scale.y, 1.3f * 0.01f, 0.7f * 0.001f);
        this->actor.scale.x = this->actor.scale.y;
        this->actor.scale.z = this->actor.scale.y;
    }

    if (this->work.timer > 0) {
        this->work.timer--;
    } else if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_YMAJIN_SPLIT);
        EnSnowman_SpawnCircularDustEffect(this, play);
        this->drawDmgEffAlpha = 0.0f;
        if (!FOREIGN_ENEMY_API_EENO_REPLACE_SPLIT(&this->actor, play)) {
            EnSnowman_CreateSplitEeno((EnSnowman*)this->actor.parent, &this->actor.world.pos,
                                      this->actor.shape.rot.y + 0x10000 / 3);
            EnSnowman_CreateSplitEeno((EnSnowman*)this->actor.child, &this->actor.world.pos,
                                      this->actor.shape.rot.y - 0x10000 / 3);
            EnSnowman_CreateSplitEeno(this, &this->actor.world.pos, this->actor.shape.rot.y);
        }

        Math_Vec3f_Copy(&this->snowballPos, &this->actor.world.pos);
        EnSnowman_SpawnBigSnowballFragmentEffects(this, play);
    } else if (this->actor.colChkInfo.health != 0) {
        this->collider.base.acFlags |= AC_ON;
        EnSnowman_SetupSubmerge(this, play);
    } else {
        EnSnowman_SetupDead(this);
    }
}

void EnSnowman_SetupDead(EnSnowman* this) {
    this->drawDmgEffAlpha = 0.0f;
    this->actionFunc = EnSnowman_Dead;
}

void EnSnowman_Dead(EnSnowman* this, PlayState* play) {
    static Vec3f sAccel = { 0.0f, -0.5f, 0.0f };
    Vec3f velocity;
    Vec3f pos;
    s16 altitude;
    s16 azimuth;
    f32 speed;
    s32 i;

    for (i = 0; i < 15; i++) {
        altitude = Rand_S16Offset(0x1000, 0x3000);
        azimuth = (Rand_Next() >> 0x10);
        speed = Rand_ZeroFloat(2.0f) + 4.0f;
        velocity.x = (speed * Math_CosS(altitude)) * Math_SinS(azimuth);
        velocity.y = speed * Math_SinS(altitude);
        velocity.z = (speed * Math_CosS(altitude)) * Math_CosS(azimuth);
        pos.x = (Rand_ZeroFloat(6.0f) * velocity.x) + this->actor.world.pos.x;
        pos.y = (Rand_ZeroFloat(3.0f) * velocity.y) + this->actor.world.pos.y;
        pos.z = (Rand_ZeroFloat(6.0f) * velocity.z) + this->actor.world.pos.z;
        EffectSsHahen_Spawn(play, &pos, &velocity, &sAccel, 0,
                            Rand_S16Offset((((i % 3) * 20) + 20), (((i % 3) * 10) + 10)), OBJECT_SNOWMAN, 20,
                            sSnowballFragmentDLs[i % 3]);
    }

    func_800B0DE0(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor, 500, 30);
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_YMAJIN_DEAD_BREAK);
    Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x60);
    if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_SPLIT) {
        EnSnowman_SetupSplitDoNothing(this);
    } else {
        Actor_Kill(&this->actor);
    }
}

void EnSnowman_SetupSplitDoNothing(EnSnowman* this) {
    this->collider.base.acFlags &= ~AC_HIT;
    this->collider.base.acFlags &= ~AC_HIT;
    this->actor.draw = NULL;
    if (this->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE) {
        this->combineState = EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION;
    }

    this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_UPDATE_CULLING_DISABLED);
    if ((this->actor.parent != NULL) && (((EnSnowman*)this->actor.parent)->actionFunc == EnSnowman_SplitDoNothing)) {
        if ((this->actor.child != NULL) && (((EnSnowman*)this->actor.child)->actionFunc == EnSnowman_SplitDoNothing)) {
            EnSnowman_SetupKill((EnSnowman*)this->actor.parent);
            EnSnowman_SetupKill((EnSnowman*)this->actor.child);
            Actor_Kill(&this->actor);
        }
    }

    this->actionFunc = EnSnowman_SplitDoNothing;
}

void EnSnowman_SplitDoNothing(EnSnowman* this, PlayState* play) {}

void EnSnowman_SetupKill(EnSnowman* this) {
    this->collider.base.acFlags &= ~(AC_ON | AC_HIT);
    this->collider.base.ocFlags1 &= ~(OC1_ON | OC1_HIT);
    this->actionFunc = EnSnowman_Kill;
}

void EnSnowman_Kill(EnSnowman* this, PlayState* play) {
    Actor_Kill(&this->actor);
}

void EnSnowman_CreateSplitEeno(EnSnowman* this, Vec3f* basePos, s32 yRot) {
    this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.shape.rot.y = yRot;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->combineState = EN_SNOWMAN_COMBINE_STATE_NONE;
    this->actor.colChkInfo.health = 2;
    this->eenoScale = 1.0f;
    this->actor.world.pos.x = (Math_SinS(yRot) * 40.0f) + basePos->x;
    this->actor.world.pos.y = basePos->y;
    this->actor.world.pos.z = (Math_CosS(yRot) * 40.0f) + basePos->z;
    this->combineTimer = 600;
    this->actor.params = EN_SNOWMAN_TYPE_SPLIT;
    this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    this->collider.base.ocFlags1 |= OC1_ON;
    this->collider.base.acFlags &= ~AC_ON;
    EnSnowman_SetupMoveSnowPile(this);
}

void EnSnowman_AbsorbEeno(EnSnowman* smallerEeno, EnSnowman* largerEeno) {
    Actor_PlaySfx(&largerEeno->actor, NA_SE_EN_YMAJIN_UNITE);
    largerEeno->fwork.targetScaleDuringCombine += 0.005f;
    smallerEeno->combineState = EN_SNOWMAN_COMBINE_STATE_BEING_ABSORBED_OR_DONE;
    smallerEeno->collider.base.ocFlags1 &= ~OC1_HIT;
    smallerEeno->collider.base.acFlags &= ~AC_HIT;
    smallerEeno->collider.base.ocFlags1 &= ~OC1_ON;
    smallerEeno->collider.base.acFlags &= ~AC_ON;
    smallerEeno->fwork.targetScaleDuringCombine = 0.0f;
}

void EnSnowman_SetupCombine(EnSnowman* this, PlayState* play, Vec3f* combinePos) {
    if (this->actor.colChkInfo.health == 0) {
        this->combineState = EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION;
    } else {
        this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        Math_Vec3f_Copy(&this->combinePos, combinePos);
        this->combineState = EN_SNOWMAN_COMBINE_STATE_ACTIVE;

        if (this->actionFunc != EnSnowman_Submerge) {
            this->combineTimer = 400;

            this->fwork.targetScaleDuringCombine = 0.01f;
        }

        if (this->actor.draw == EnSnowman_DrawSnowPile) {
            this->actor.speed = 3.0f;
            this->actionFunc = EnSnowman_Combine;
        } else {
            this->isHoldingSnowball = false;
            this->actor.speed = 0.0f;

            EnSnowman_SetupSubmerge(this, play);
        }
    }
}

void EnSnowman_Combine(EnSnowman* this, PlayState* play) {
    EnSnowman* parent;
    EnSnowman* child;

    SkelAnime_Update(&this->snowPileSkelAnime);
    parent = (EnSnowman*)this->actor.parent;
    child = (EnSnowman*)this->actor.child;
    Math_ScaledStepToS(&this->actor.shape.rot.y, Actor_WorldYawTowardPoint(&this->actor, &this->combinePos), 0x1000);
    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (this->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE) {
        if (this->collider.base.ocFlags1 & OC1_HIT) {
            if ((this->collider.base.oc == this->actor.parent) &&
                (parent->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE)) {
                if (this->actor.scale.x < this->actor.parent->scale.x) {
                    EnSnowman_AbsorbEeno(this, parent);
                } else {
                    EnSnowman_AbsorbEeno(parent, this);
                }
            } else if ((this->collider.base.oc == this->actor.child) &&
                       (child->combineState == EN_SNOWMAN_COMBINE_STATE_ACTIVE)) {
                if (this->actor.scale.x < this->actor.child->scale.x) {
                    EnSnowman_AbsorbEeno(this, child);
                } else {
                    EnSnowman_AbsorbEeno(child, this);
                }
            }
        }

        if (parent->combineState == EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION) {
            EnSnowman_AbsorbEeno(parent, this);
        }

        if (child->combineState == EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION) {
            EnSnowman_AbsorbEeno(child, this);
        }
    }

    if ((this->combineTimer == 0) && (parent->fwork.targetScaleDuringCombine > 0.0f) &&
        (child->fwork.targetScaleDuringCombine > 0.0f) && (this->fwork.targetScaleDuringCombine < 0.011f) &&
        (this->combineState != EN_SNOWMAN_COMBINE_STATE_BEING_ABSORBED_OR_DONE)) {
        this->combineState = EN_SNOWMAN_COMBINE_STATE_NO_ABSORPTION;
        this->fwork.targetScaleDuringCombine = 0.0f;
    }

    if (Actor_WorldDistXZToPoint(&this->actor, &this->combinePos) < 20.0f) {
        this->actor.speed = 0.0f;
    }

    if (Math_StepToF(&this->actor.scale.x, this->fwork.targetScaleDuringCombine, 0.0005f)) {
        if (this->fwork.targetScaleDuringCombine < 0.01f) {
            EnSnowman_SetupSplitDoNothing(this);
        } else if (this->fwork.targetScaleDuringCombine > 0.018f) {
            Actor_SetScale(&this->actor, 0.02f);
            this->actor.params = EN_SNOWMAN_TYPE_LARGE;
            this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
            this->collider.base.ocFlags1 |= OC1_ON;
            this->combineState = EN_SNOWMAN_COMBINE_STATE_BEING_ABSORBED_OR_DONE;
            this->eenoScale = 2.0f;
            EnSnowman_SetupMoveSnowPile(this);
        }
    }

    this->actor.scale.y = this->actor.scale.x;
    this->actor.scale.z = this->actor.scale.x;
}

void EnSnowman_UpdateDamage(EnSnowman* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.elem);
        if ((this->actor.colChkInfo.damageEffect != EN_SNOWMAN_DMGEFF_HOOKSHOT) ||
            (EN_SNOWMAN_GET_TYPE(&this->actor) != EN_SNOWMAN_TYPE_LARGE)) {
            if (this->actor.colChkInfo.damageEffect == EN_SNOWMAN_DMGEFF_MELT) {
                Enemy_StartFinishingBlow(play, &this->actor);
                Actor_ApplyDamage(&this->actor);
                EnSnowman_SetupMelt(this);
            } else {
                if ((this->actionFunc == EnSnowman_MoveSnowPile) || (this->actionFunc == EnSnowman_Combine)) {
                    EnSnowman_SetupEmerge(this, play);
                } else if (this->actor.colChkInfo.damageEffect == EN_SNOWMAN_DMGEFF_STUN) {
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                    Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                    EnSnowman_SetupStun(this);
                } else if (this->actor.colChkInfo.damageEffect == EN_SNOWMAN_DMGEFF_ELECTRIC_STUN) {
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                    this->drawDmgEffScale = 0.55f;
                    this->drawDmgEffAlpha = 2.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE;
                    Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                    EnSnowman_SetupStun(this);
                } else if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) {
                    if (this->isHoldingSnowball == true) {
                        this->isHoldingSnowball = false;
                        EnSnowman_SpawnBigSnowballFragmentEffects(this, play);
                    }

                    EnSnowman_SetupDamaged(this);
                } else {
                    if (Actor_ApplyDamage(&this->actor) == 0) {
                        Enemy_StartFinishingBlow(play, &this->actor);
                    }

                    EnSnowman_SetupDamaged(this);
                }
            }

            if (this->actor.colChkInfo.damageEffect == EN_SNOWMAN_DMGEFF_LIGHT_ORB) {
                this->drawDmgEffScale = 0.55f;
                this->drawDmgEffAlpha = 4.0f;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->collider.elem.acDmgInfo.hitPos.x,
                            this->collider.elem.acDmgInfo.hitPos.y, this->collider.elem.acDmgInfo.hitPos.z, 0, 0, 0,
                            CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
            }
        }
    }
}

void EnSnowman_Update(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;
    f32 wallCheckRadius;

    if (this->actionFunc != EnSnowman_SplitDoNothing) {
        DECR(this->combineTimer);

        EnSnowman_UpdateDamage(this, play);
        this->actionFunc(this, play);

        if (this->actionFunc != EnSnowman_Kill) {
            Actor_MoveWithGravity(&this->actor);
            if ((EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_LARGE) &&
                (this->actionFunc == EnSnowman_ReadySnowball)) {
                wallCheckRadius = (this->skelAnime.curFrame * (1.0f / 60.0f)) + 1.0f;
                wallCheckRadius = CLAMP_MAX(wallCheckRadius, 1.3f);
                wallCheckRadius *= this->collider.dim.radius;
            } else {
                wallCheckRadius = this->collider.dim.radius;
            }

            Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, wallCheckRadius, 0.0f,
                                    UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                        UPDBGCHECKINFO_FLAG_10);
            if ((this->actor.floorPoly != NULL) && (COLPOLY_GET_NORMAL(this->actor.floorPoly->normal.y) < 0.7f)) {
                Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.prevPos);
                if (!this->turningOnSteepSlope) {
                    this->snowPileTargetRotY = Math_Atan2S_XY(COLPOLY_GET_NORMAL(this->actor.floorPoly->normal.z),
                                                              COLPOLY_GET_NORMAL(this->actor.floorPoly->normal.x));
                    this->turningOnSteepSlope = true;
                }
            } else {
                func_800BE3D0(&this->actor, this->actor.shape.rot.y, &this->actor.shape.rot);
            }

            Collider_UpdateCylinder(&this->actor, &this->collider);
            if (this->collider.base.acFlags & AC_ON) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            }
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

            if (this->actor.draw == EnSnowman_Draw) {
                Actor_SetFocus(&this->actor, this->actor.scale.y * 1800.0f);
            } else {
                Actor_SetFocus(&this->actor, this->actor.scale.y * 720.0f);
            }

            if (this->drawDmgEffAlpha > 0.0f) {
                Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
                this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.275f;
                this->drawDmgEffScale = CLAMP_MAX(this->drawDmgEffScale, 0.55f);
            }
        }
    }
}

void EnSnowman_UpdateSnowball(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;
    s16 scale;
    s32 i;

    if (this->work.timer > 0) {
        this->work.timer--;
    } else {
        this->collider.base.ocFlags1 |= OC1_ON;
    }

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) ||
        (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING) || (this->collider.base.atFlags & AT_HIT) ||
        (this->collider.base.acFlags & AC_HIT) || (this->collider.base.ocFlags1 & OC1_HIT)) {
        if (EN_SNOWMAN_GET_TYPE(&this->actor) == EN_SNOWMAN_TYPE_SMALL_SNOWBALL) {
            scale = 10;
            for (i = 0; i < 3; i++) {
                EffectSsHahen_SpawnBurst(play, &thisx->world.pos, 5.0f, 0, scale, scale >> 1, 3, OBJECT_SNOWMAN, 20,
                                         sSnowballFragmentDLs[i]);
                scale *= 2;
            }

            func_800B0DE0(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f, &sDustPrimColor, &sDustEnvColor, 500,
                          30);
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_SMALL_SNOWBALL_BROKEN);
        } else {
            Math_Vec3f_Copy(&this->snowballPos, &this->actor.world.pos);
            EnSnowman_SpawnBigSnowballFragmentEffects(this, play);
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EV_SNOWBALL_BROKEN);
        }

        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_IT_REFLECTION_WOOD);
        Actor_Kill(&this->actor);
        return;
    }

    this->actor.shape.rot.x += 0xF00;
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, this->collider.dim.radius * 0.6f,
                            this->collider.dim.height - this->collider.dim.yShift,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

static s8 sLimbToBodyParts[EENO_LIMB_MAX] = {
    BODYPART_NONE, BODYPART_NONE,         BODYPART_NONE,         BODYPART_NONE,
    BODYPART_NONE, BODYPART_NONE,         EN_SNOWMAN_BODYPART_0, EN_SNOWMAN_BODYPART_1,
    BODYPART_NONE, EN_SNOWMAN_BODYPART_2, EN_SNOWMAN_BODYPART_3, EN_SNOWMAN_BODYPART_4,
};

static Vec3f sBodyBottomBodyPartOffsets[] = {
    { 2000.0f, 3000.0f, 0.0f }, { 2000.0f, -2000.0f, 0.0f }, { 3000.0f, 0.0f, 0.0f },
    { 1000.0f, 0.0f, 3000.0f }, { 1000.0f, 0.0f, -3000.0f },
};

void EnSnowman_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnSnowman* this = (EnSnowman*)thisx;
    Gfx* gfx;
    s32 i;

    if (sLimbToBodyParts[limbIndex] != BODYPART_NONE) {
        if (sLimbToBodyParts[limbIndex] == EN_SNOWMAN_BODYPART_4) {
            for (i = 0; i < ARRAY_COUNT(sBodyBottomBodyPartOffsets); i++) {
                Matrix_MultVec3f(&sBodyBottomBodyPartOffsets[i], &this->bodyPartsPos[EN_SNOWMAN_BODYPART_4 + i]);
            }
        } else {
            Matrix_MultZero(&this->bodyPartsPos[sLimbToBodyParts[limbIndex]]);
        }
    }

    if ((limbIndex == EENO_LIMB_RIGHT_HAND) && (this->isHoldingSnowball == true)) {
        OPEN_DISPS(play->state.gfxCtx);

        gfx = POLY_OPA_DISP;

        if (EN_SNOWMAN_GET_TYPE(thisx) != EN_SNOWMAN_TYPE_LARGE) {
            Matrix_Translate(800.0f, -600.0f, 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Translate(300.0f, -2300.0f, -1900.0f, MTXMODE_APPLY);
            Matrix_Scale(0.3f, 0.3f, 0.3f, MTXMODE_APPLY);
        }

        MATRIX_FINALIZE_AND_LOAD(&gfx[0], play->state.gfxCtx);
        gSPDisplayList(&gfx[1], sSnowballDLs[EN_SNOWMAN_GET_TYPE(&this->actor)]);

        POLY_OPA_DISP = &gfx[2];

        CLOSE_DISPS();

        Matrix_MultZero(&this->snowballPos);
    }
}

void EnSnowman_Draw(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          EnSnowman_PostLimbDraw, &this->actor);
    Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EN_SNOWMAN_BODYPART_MAX,
                            this->drawDmgEffScale * this->eenoScale, 0.0f, this->drawDmgEffAlpha, this->drawDmgEffType);
}

void EnSnowman_DrawSnowPile(Actor* thisx, PlayState* play) {
    EnSnowman* this = (EnSnowman*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->snowPileSkelAnime.skeleton, this->snowPileSkelAnime.jointTable,
                          this->snowPileSkelAnime.dListCount, NULL, NULL, &this->actor);
}

void EnSnowman_DrawSnowball(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, sSnowballDLs[EN_SNOWMAN_GET_TYPE(thisx) - EN_SNOWMAN_TYPE_SMALL_SNOWBALL]);
}

OVL_INFO_NATIVE_ENEMY(0x1e6, En_Snowman_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_SNOWMAN, En_Snowman_Profile, EnSnowman_Init, EnSnowman_Destroy,
                     EnSnowman_Update, EnSnowman_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x1e6u, 0x1c4u, 0xe0e4e793u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_SNOWMAN_ORIGIN, generatedNativeOrigin);
