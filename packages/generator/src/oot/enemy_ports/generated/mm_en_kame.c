#include "mm_native_compat.h"
#define gSnapperDamageAnim (*(AnimationHeader*)0x060008b4)
#define gSnapperDeathAnim (*(AnimationHeader*)0x06000af4)
#define gSpikedSnapperIdleAnim (*(AnimationHeader*)0x06000b30)
#define gSpikedSnapperSkel (*(FlexSkeletonHeader*)0x06001a50)
#define gSnapperRetreatIntoShellAnim (*(AnimationHeader*)0x06001c68)
#define gSnapperFlipUprightAnim (*(AnimationHeader*)0x06002510)
#define gSnapperWiggleLegsAnim (*(AnimationHeader*)0x060027d8)
#define gSnapperFailToFlipUprightAnim (*(AnimationHeader*)0x06002f88)
#define gSnapperEmergeFromShellAnim (*(AnimationHeader*)0x060031dc)
#define gSnapperBouncedUprightAnim (*(AnimationHeader*)0x060035ec)
#define gSnapperFlipOverAnim (*(AnimationHeader*)0x060039c0)
#define gSnapperIdleAnim (*(AnimationHeader*)0x06004210)
#define gSnapperEyeOpenTex (*((u64 (*)[1])0x060055a0))
#define gSnapperEyeHalfTex (*((u64 (*)[1])0x060057a0))
#define gSnapperEyeClosedTex (*((u64 (*)[1])0x060059a0))
#define gSnapperSkel (*(FlexSkeletonHeader*)0x06007c70)
#define gSnapperWalkAnim (*(AnimationHeader*)0x0600823c)

#include "types/en_kame.h"
#define TATL_HINT_ID_SNAPPER 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_KAME
#define ACTOR_EN_KAME 0x1ba
#undef OBJECT_TL
#define OBJECT_TL 0x11a6
#undef NA_SE_EV_ICE_FREEZE
#define NA_SE_EV_ICE_FREEZE 0x28b2
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_HIPLOOP_LAND
#define NA_SE_EN_HIPLOOP_LAND 0x38dd
#undef NA_SE_EN_PAMET_VOICE
#define NA_SE_EN_PAMET_VOICE 0x38f3
#undef NA_SE_EN_PAMET_ROLL
#define NA_SE_EN_PAMET_ROLL 0x38f4
#undef NA_SE_EN_PAMET_WALK
#define NA_SE_EN_PAMET_WALK 0x38f5
#undef NA_SE_EN_PAMET_ROAR
#define NA_SE_EN_PAMET_ROAR 0x38f6
#undef NA_SE_EN_PAMET_WAKEUP
#define NA_SE_EN_PAMET_WAKEUP 0x38f7
#undef NA_SE_EN_PAMET_REVERSE
#define NA_SE_EN_PAMET_REVERSE 0x38f8
#undef NA_SE_EN_PAMET_DAMAGE
#define NA_SE_EN_PAMET_DAMAGE 0x38f9
#undef NA_SE_EN_PAMET_DEAD
#define NA_SE_EN_PAMET_DEAD 0x38fa
#undef NA_SE_EN_PAMET_CUTTER_ON
#define NA_SE_EN_PAMET_CUTTER_ON 0x392b
#undef NA_SE_EN_PAMET_CUTTER_OFF
#define NA_SE_EN_PAMET_CUTTER_OFF 0x392d
#undef NA_SE_EN_COMMON_EXTINCT_LEV
#define NA_SE_EN_COMMON_EXTINCT_LEV 0x3a1f
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef func_800BE504
#define func_800BE504 ((void (*)(Actor* actor, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_HIT_YAW_CYLINDER)
#undef SoundSource_PlaySfxEachFrameAtFixedWorldPos
#define SoundSource_PlaySfxEachFrameAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD_LOOP)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef Actor_SetDropFlag
#define Actor_SetDropFlag ((void (*)(Actor* actor, ColliderElement* elem))(void*)FOREIGN_ENEMY_API_DROP_FLAG)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d21cu)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef SurfaceType_GetMaterial
#define SurfaceType_GetMaterial ((SurfaceMaterial (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x800345b8u)
#undef func_800B1210
#define func_800B1210 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep))0x8001c1c4u)
#undef func_800B0DE0
#define func_800B0DE0 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep))0x8001bd94u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d2d4u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef ActorShadow_DrawCircle
#define ActorShadow_DrawCircle ((void (*)(Actor* actor, struct Lights* lights, struct PlayState* play))0x8001eff4u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER)

void EnKame_Init(Actor* thisx, PlayState* play);
void EnKame_Destroy(Actor* thisx, PlayState* play);
void EnKame_Update(Actor* thisx, PlayState* play);
void EnKame_Draw(Actor* thisx, PlayState* play);

void EnKame_SetupIdle(EnKame* this);
void EnKame_Idle(EnKame* this, PlayState* play);
void EnKame_SetupWalk(EnKame* this);
void EnKame_Walk(EnKame* this, PlayState* play);
void EnKame_SetupRetreatIntoShell(EnKame* this);
void EnKame_RetreatIntoShell(EnKame* this, PlayState* play);
void EnKame_SetupPrepareToAttack(EnKame* this);
void EnKame_PrepareToAttack(EnKame* this, PlayState* play);
void EnKame_SetupAttack(EnKame* this);
void EnKame_Attack(EnKame* this, PlayState* play);
void EnKame_SetupEmergeFromShell(EnKame* this);
void EnKame_EmergeFromShell(EnKame* this, PlayState* play);
void EnKame_Flip(EnKame* this, PlayState* play);
void EnKame_SetupStruggle(EnKame* this);
void EnKame_Struggle(EnKame* this, PlayState* play);
void EnKame_SetupFlipUpright(EnKame* this);
void EnKame_FlipUpright(EnKame* this, PlayState* play);
void EnKame_Stunned(EnKame* this, PlayState* play);
void EnKame_Damaged(EnKame* this, PlayState* play);
void EnKame_SetupDead(EnKame* this, PlayState* play);
void EnKame_Dead(EnKame* this, PlayState* play);
void EnKame_SetupBurstIntoFlames(EnKame* this);
void EnKame_BurstIntoFlames(EnKame* this, PlayState* play);
void EnKame_DrawSpikedSnapper(Actor* thisx, PlayState* play);

typedef enum { EN_KAME_FLIP_TYPE_UPSIDE_DOWN, EN_KAME_FLIP_TYPE_RIGHTSIDE_UP } EnKameFlipType;

typedef enum {
    EN_KAME_EYE_OPEN,
    EN_KAME_EYE_HALF_CLOSING,
    EN_KAME_EYE_CLOSED,
    EN_KAME_EYE_HALF_OPENING,
    EN_KAME_EYE_MAX
} EnKameEyeTexture;

ActorProfile En_Kame_Profile = {
    0xdba, ACTORCAT_ENEMY, FLAGS, 0x11a6, sizeof(EnKame), EnKame_Init, EnKame_Destroy, EnKame_Update, EnKame_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HARD,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CF7FFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 35, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 3, 15, 30, 80 };

typedef enum {
    EN_KAME_DMGEFF_NONE,
    EN_KAME_DMGEFF_STUN,
    EN_KAME_DMGEFF_FIRE,
    EN_KAME_DMGEFF_FREEZE,
    EN_KAME_DMGEFF_LIGHT_ORB,
    EN_KAME_DMGEFF_ELECTRIC_STUN,
    EN_KAME_DMGEFF_HOOKSHOT = 0xD,
    EN_KAME_DMGEFF_INSTANT_KILL,
    EN_KAME_DMGEFF_FLIP_OVER
} EnKameDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_KAME_DMGEFF_STUN),      DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(1, EN_KAME_DMGEFF_FLIP_OVER),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_HOOKSHOT),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(0, EN_KAME_DMGEFF_FLIP_OVER), DMG_ENTRY(2, EN_KAME_DMGEFF_FIRE),
    DMG_ENTRY(2, EN_KAME_DMGEFF_FREEZE),    DMG_ENTRY(2, EN_KAME_DMGEFF_LIGHT_ORB),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_STUN),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(2, EN_KAME_DMGEFF_INSTANT_KILL),
    DMG_ENTRY(0, EN_KAME_DMGEFF_STUN),      DMG_ENTRY(0, EN_KAME_DMGEFF_ELECTRIC_STUN),
    DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(1, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),
    DMG_ENTRY(0, EN_KAME_DMGEFF_NONE),      DMG_ENTRY(1, EN_KAME_DMGEFF_FLIP_OVER),
} };

static TexturePtr sEyeTextures[] = { gSnapperEyeOpenTex, gSnapperEyeHalfTex, gSnapperEyeClosedTex, gSnapperEyeHalfTex };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_SNAPPER, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3500, ICHAIN_STOP),
};

static s32 sTexturesDesegmented = false;

void EnKame_Init(Actor* thisx, PlayState* play) {
    EnKame* this = (EnKame*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    SkelAnime_InitFlex(play, &this->snapperSkelAnime, &gSnapperSkel, &gSnapperIdleAnim, this->snapperJointTable,
                       this->snapperMorphTable, SNAPPER_LIMB_MAX);
    SkelAnime_InitFlex(play, &this->spikedSnapperSkelAnime, &gSpikedSnapperSkel, &gSpikedSnapperIdleAnim,
                       this->spikedSnapperJointTable, this->spikedSnapperMorphTable, SPIKED_SNAPPER_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 55.0f);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    if (!sTexturesDesegmented) {
        s32 i;

        for (i = 0; i < ARRAY_COUNT(sEyeTextures); i++) {
            sEyeTextures[i] = Lib_SegmentedToVirtual(sEyeTextures[i]);
        }

        sTexturesDesegmented = true;
    }

    EnKame_SetupIdle(this);
}

void EnKame_Destroy(Actor* thisx, PlayState* play) {
    EnKame* this = (EnKame*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnKame_Blink(EnKame* this) {
    if (this->eyeIndex != EN_KAME_EYE_OPEN) {
        this->eyeIndex++;
        if (this->eyeIndex == EN_KAME_EYE_MAX) {
            this->eyeIndex = EN_KAME_EYE_OPEN;
        }
    } else if (Rand_ZeroOne() < 0.05f) {
        this->eyeIndex = EN_KAME_EYE_HALF_CLOSING;
    }
}

void EnKame_Freeze(EnKame* this) {
    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
    this->drawDmgEffScale = 0.6f;
    this->drawDmgEffFrozenSteamScale = 900.0f * 0.001f;
    this->drawDmgEffAlpha = 1.0f;
    this->collider.base.colMaterial = COL_MATERIAL_HIT3;
    this->stunTimer = 80;
    this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 80);
}

void EnKame_Thaw(EnKame* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->collider.base.colMaterial = COL_MATERIAL_HIT6;
        this->drawDmgEffAlpha = 0.0f;
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, SNAPPER_BODYPART_MAX, 2, 0.3f, 0.2f);
        this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    }
}

void EnKame_SetupIdle(EnKame* this) {
    Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperIdleAnim, -5.0f);
    this->actor.speed = 0.0f;
    this->actionFunc = EnKame_Idle;
}

void EnKame_Idle(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Animation_OnFrame(&this->snapperSkelAnime, 10.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_VOICE);
        this->voiceTimer = 40;
    }

    if ((Player_GetMask(play) != PLAYER_MASK_STONE) && !(player->stateFlags1 & PLAYER_STATE1_800000) &&
        (this->actor.xzDistToPlayer < 240.0f)) {
        EnKame_SetupRetreatIntoShell(this);
    } else if (SkelAnime_Update(&this->snapperSkelAnime)) {
        EnKame_SetupWalk(this);
    }
}

void EnKame_SetupWalk(EnKame* this) {
    Animation_MorphToLoop(&this->snapperSkelAnime, &gSnapperWalkAnim, -5.0f);
    this->actor.speed = 0.5f;
    this->timer = Animation_GetLastFrame(&gSnapperWalkAnim) * ((s32)Rand_ZeroFloat(5.0f) + 3);
    this->targetRotY = this->actor.shape.rot.y;
    this->collider.base.acFlags |= (AC_HARD | AC_ON);
    this->collider.base.colMaterial = COL_MATERIAL_HARD;
    this->actionFunc = EnKame_Walk;
}

void EnKame_Walk(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((Player_GetMask(play) != PLAYER_MASK_STONE) && !(player->stateFlags1 & PLAYER_STATE1_800000) &&
        (this->actor.xzDistToPlayer < 240.0f)) {
        EnKame_SetupRetreatIntoShell(this);
        return;
    }

    SkelAnime_Update(&this->snapperSkelAnime);

    if (this->targetRotY != this->actor.shape.rot.y) {
        Math_ScaledStepToS(&this->actor.shape.rot.y, this->targetRotY, 0x100);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    } else if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 40.0f) {
        this->targetRotY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) + ((s32)Rand_Next() >> 0x14);
    }

    this->timer--;
    if (this->timer == 0) {
        EnKame_SetupIdle(this);
    } else if (Animation_OnFrame(&this->snapperSkelAnime, 0.0f) || Animation_OnFrame(&this->snapperSkelAnime, 15.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_WALK);
    }
}

void EnKame_SetupRetreatIntoShell(EnKame* this) {
    Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperRetreatIntoShellAnim, -3.0f);
    this->timer = 0;
    this->limbScaleY = 1.0f;
    this->limbScaleXZ = 1.0f;
    this->actor.speed = 0.0f;

    if (this->voiceTimer == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_VOICE);
    }

    this->actionFunc = EnKame_RetreatIntoShell;
}

void EnKame_RetreatIntoShell(EnKame* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (SkelAnime_Update(&this->snapperSkelAnime)) {
        if ((Player_GetMask(play) != PLAYER_MASK_STONE) && !(player->stateFlags1 & PLAYER_STATE1_800000) &&
            ((this->timer == 0) || (this->actor.xzDistToPlayer < 120.0f))) {
            EnKame_SetupPrepareToAttack(this);
        } else {
            this->timer--;
            if (this->timer == 0) {
                EnKame_SetupEmergeFromShell(this);
            }
        }
    } else if (this->snapperSkelAnime.curFrame > 2.0f) {
        this->limbScaleY = 1.5f - ((this->snapperSkelAnime.curFrame - 2.0f) * (7.0f / 30));
        this->limbScaleXZ = 1.5f - ((this->snapperSkelAnime.curFrame - 2.0f) * (1.0f / 12));
    } else {
        f32 curFrame = this->snapperSkelAnime.curFrame;

        this->limbScaleY = (0.25f * curFrame) + 1.0f;
        this->limbScaleXZ = (0.25f * curFrame) + 1.0f;
    }
}

void EnKame_SetAttackSpeed(EnKame* this) {
    this->actor.speed = this->angularVelocityY * (5.0f / 0x1D80);
    this->actor.shape.rot.z = this->angularVelocityY * (13.0f / 118.0f);
}

void EnKame_ProcessAttackEffects(EnKame* this, PlayState* play) {
    static Color_RGBA8 sSnowPrimColor = { 250, 250, 250, 255 };
    static Color_RGBA8 sSnowEnvColor = { 180, 180, 180, 255 };
    static Vec3f sVelocity = { 0.0f, 0.75f, 0.0f };

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.speed >= 3.0f)) {
        if ((play->gameplayFrames % 2) == 0) {
            SurfaceMaterial surfaceMaterial =
                SurfaceType_GetMaterial(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);

            if ((surfaceMaterial == SURFACE_MATERIAL_DIRT) || (surfaceMaterial == SURFACE_MATERIAL_SAND)) {
                func_800B1210(play, &this->actor.world.pos, &sVelocity, &gZeroVec3f, 550, 100);
            } else if (surfaceMaterial == SURFACE_MATERIAL_SNOW) {
                func_800B0DE0(play, &this->actor.world.pos, &sVelocity, &gZeroVec3f, &sSnowPrimColor, &sSnowEnvColor,
                              550, 100);
            }
        }
    }

    if (this->angularVelocityY > 0x1200) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_PAMET_ROLL - SFX_FLAG);
    }
}

void EnKame_SetupPrepareToAttack(EnKame* this) {
    if (this->actor.draw == EnKame_Draw) {
        this->actor.draw = EnKame_DrawSpikedSnapper;
        this->angularVelocityY = 0x3B00;
        this->spikesScale = 0.5f;
        EnKame_SetAttackSpeed(this);
        this->timer = 15;
        this->actor.speed = 0.0f;
        Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_CUTTER_ON);
        this->targetPos.y = this->actor.home.pos.y - 100.0f;
    } else {
        this->actor.world.rot.y = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
        Math_Vec3f_Copy(&this->targetPos, &this->actor.home.pos);
        this->timer = 0;
    }

    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actionFunc = EnKame_PrepareToAttack;
}

void EnKame_PrepareToAttack(EnKame* this, PlayState* play) {
    if (this->timer == 15) {
        this->spikesScale += 0.2f;
        if (this->spikesScale > 1.1f) {
            this->timer--;
            this->collider.base.atFlags |= AT_ON;
            this->spikesScale = 1.0f;
        }
    } else if (this->timer > 0) {
        this->timer--;
    } else if (Math_ScaledStepToS(&this->angularVelocityY, 0x3B00, (s32)(this->angularVelocityY * 0.09f) + 45)) {
        if (this->targetPos.y < this->actor.home.pos.y) {
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            this->targetPos.x = (Math_SinS(this->actor.world.rot.y) * 360.0f) + this->actor.world.pos.x;
            this->targetPos.z = (Math_CosS(this->actor.world.rot.y) * 360.0f) + this->actor.world.pos.z;
        }

        EnKame_SetupAttack(this);
    }

    this->actor.shape.rot.y += this->angularVelocityY;

    if (!(this->targetPos.y < this->actor.home.pos.y) || (this->actionFunc != EnKame_PrepareToAttack)) {
        EnKame_SetAttackSpeed(this);
    }

    EnKame_ProcessAttackEffects(this, play);
}

void EnKame_SetupAttack(EnKame* this) {
    this->targetAngularVelocityY = -1;
    this->actionFunc = EnKame_Attack;
}

void EnKame_Attack(EnKame* this, PlayState* play) {
    this->actor.shape.rot.y += this->angularVelocityY;
    EnKame_ProcessAttackEffects(this, play);

    if (this->targetAngularVelocityY == -1) {
        s32 absYawToTarget;
        s16 yawToTarget = Actor_WorldYawTowardPoint(&this->actor, &this->targetPos) - this->actor.world.rot.y;

        absYawToTarget = ABS_ALT(yawToTarget);

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || (absYawToTarget > 0x3000) ||
            (Actor_WorldDistXZToPoint(&this->actor, &this->targetPos) < 50.0f)) {
            if (this->targetPos.y < this->actor.home.pos.y) {
                this->targetAngularVelocityY = 0x300;
            } else {
                this->targetAngularVelocityY = 0;
            }
        }
        return;
    }

    if (Math_ScaledStepToS(&this->angularVelocityY, this->targetAngularVelocityY,
                           (s32)(this->angularVelocityY * 0.09f) + 45)) {
        if (this->targetAngularVelocityY == 0) {
            if (this->spikesScale >= 1.0f) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_CUTTER_OFF);
            }

            this->spikesScale -= 0.1f;
            this->collider.base.atFlags &= ~AT_ON;
            if (this->spikesScale < 0.5f) {
                this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
                EnKame_SetupEmergeFromShell(this);
            }
        } else {
            EnKame_SetupPrepareToAttack(this);
        }
    }

    EnKame_SetAttackSpeed(this);
}

void EnKame_SetupEmergeFromShell(EnKame* this) {
    this->actor.draw = EnKame_Draw;
    Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperEmergeFromShellAnim, -3.0f);
    this->actor.speed = 0.0f;
    this->limbScaleY = 0.1f;
    this->limbScaleXZ = 1.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = EnKame_EmergeFromShell;
}

void EnKame_EmergeFromShell(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->snapperSkelAnime)) {
        EnKame_SetupWalk(this);
    } else if (this->snapperSkelAnime.curFrame > 7.0f) {
        this->limbScaleY = 1.5f - ((this->snapperSkelAnime.curFrame - 7.0f) * (1.0f / 6));
        this->limbScaleXZ = 1.5f - ((this->snapperSkelAnime.curFrame - 7.0f) * (1.0f / 6));
    } else {
        f32 curFrame = this->snapperSkelAnime.curFrame;

        this->limbScaleY = (0.2f * curFrame) + 0.1f;
        this->limbScaleXZ = ((1.0f / 14) * curFrame) + 1.0f;
    }
}

void EnKame_SetupFlip(EnKame* this) {
    if (this->actionFunc == EnKame_Struggle) {
        Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperBouncedUprightAnim, -3.0f);
        this->flipType = EN_KAME_FLIP_TYPE_RIGHTSIDE_UP;
        this->collider.elem.acDmgInfo.dmgFlags &= ~0x8000;
    } else {
        Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperFlipOverAnim, -3.0f);
        this->flipType = EN_KAME_FLIP_TYPE_UPSIDE_DOWN;
        this->collider.elem.acDmgInfo.dmgFlags |= 0x8000;
    }

    this->actor.draw = EnKame_Draw;
    this->actor.speed = 0.0f;
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
    this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actor.shape.rot.z = 0;
    Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_REVERSE);
    this->actionFunc = EnKame_Flip;
}

void EnKame_Flip(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->snapperSkelAnime)) {
        if (this->flipType == EN_KAME_FLIP_TYPE_RIGHTSIDE_UP) {
            EnKame_SetupWalk(this);
        } else {
            this->timer = 200;
            EnKame_SetupStruggle(this);
        }
    }
}

void EnKame_SetupStruggle(EnKame* this) {
    Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperWiggleLegsAnim, -3.0f);
    this->collider.base.acFlags |= AC_ON;
    this->collider.base.acFlags &= ~AC_HARD;
    this->collider.base.colMaterial = COL_MATERIAL_HIT6;
    this->actor.speed = 0.0f;
    this->actionFunc = EnKame_Struggle;
}

void EnKame_Struggle(EnKame* this, PlayState* play) {
    if (this->timer > 0) {
        this->timer--;
        if (SkelAnime_Update(&this->snapperSkelAnime)) {
            if (Rand_ZeroOne() > 0.5f) {
                Animation_PlayOnce(&this->snapperSkelAnime, &gSnapperWiggleLegsAnim);
            } else {
                Animation_PlayOnce(&this->snapperSkelAnime, &gSnapperFailToFlipUprightAnim);
                Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_ROAR);
            }
        }
    } else {
        EnKame_SetupFlipUpright(this);
    }
}

void EnKame_SetupFlipUpright(EnKame* this) {
    Animation_MorphToPlayOnce(&this->snapperSkelAnime, &gSnapperFlipUprightAnim, -3.0f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_WAKEUP);
    this->actionFunc = EnKame_FlipUpright;
}

void EnKame_FlipUpright(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->snapperSkelAnime)) {
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        EnKame_SetupWalk(this);
    } else if (this->snapperSkelAnime.curFrame >= 10.0f) {
        this->actor.shape.shadowDraw = NULL;
        this->collider.base.acFlags &= ~AC_ON;
        this->collider.elem.acDmgInfo.dmgFlags &= ~0x8000;
    }
}

void EnKame_SetupStunned(EnKame* this) {
    this->actor.speed = 0.0f;
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }

    func_800BE504(&this->actor, &this->collider);
    this->actionFunc = EnKame_Stunned;
}

void EnKame_Stunned(EnKame* this, PlayState* play) {
    if (this->stunTimer != 0) {
        this->stunTimer--;
    }

    if (this->stunTimer == 0) {
        EnKame_Thaw(this, play);
        if (this->actor.colChkInfo.health == 0) {
            EnKame_SetupDead(this, NULL);
        } else {
            this->actor.world.rot.y = this->actor.shape.rot.y;
            EnKame_SetupStruggle(this);
        }
    }
}

void EnKame_SetupDamaged(EnKame* this) {
    s16 endFrame = Animation_GetLastFrame(&gSnapperDamageAnim);

    Animation_Change(&this->snapperSkelAnime, &gSnapperDamageAnim, 1.0f, 0.0f, endFrame, ANIMMODE_ONCE, -3.0f);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, endFrame);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_DAMAGE);
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnKame_Damaged;
}

void EnKame_Damaged(EnKame* this, PlayState* play) {
    if (SkelAnime_Update(&this->snapperSkelAnime)) {
        EnKame_SetupStruggle(this);
    }
}

void EnKame_SetupDead(EnKame* this, PlayState* play) {
    Animation_PlayLoop(&this->snapperSkelAnime, &gSnapperDeathAnim);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 20);
    this->collider.base.acFlags &= ~AC_ON;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~(AC_HARD | AC_HIT);
    this->actor.velocity.y = 15.0f;
    this->actor.speed = 1.5f;

    if (play != NULL) {
        Enemy_StartFinishingBlow(play, &this->actor);
        if (this->actor.draw == EnKame_DrawSpikedSnapper) {
            this->actor.draw = EnKame_Draw;
        } else {
            func_800BE504(&this->actor, &this->collider);
        }
    }

    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    Actor_PlaySfx(&this->actor, NA_SE_EN_PAMET_DEAD);
    this->timer = 0;
    this->actionFunc = EnKame_Dead;
}

void EnKame_Dead(EnKame* this, PlayState* play) {
    SkelAnime_Update(&this->snapperSkelAnime);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.velocity.y < 0.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_HIPLOOP_LAND);
        EnKame_SetupBurstIntoFlames(this);
    } else {
        if (this->timer == 1) {
            this->actor.colorFilterTimer = 100;
        } else if (this->actor.colorFilterTimer == 0) {
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_NONE, 255, COLORFILTER_BUFFLAG_OPA, 100);
        }

        this->actor.shape.rot.x += Rand_S16Offset(0x700, 0x1400);
        this->actor.shape.rot.y += TRUNCF_BINANG(Rand_ZeroFloat(0x1400));
        this->actor.shape.rot.z += Rand_S16Offset(0x700, 0x1400);
    }
}

void EnKame_SetupBurstIntoFlames(EnKame* this) {
    this->timer = 20;
    this->actor.speed = 0.0f;
    this->actionFunc = EnKame_BurstIntoFlames;
}

void EnKame_BurstIntoFlames(EnKame* this, PlayState* play) {
    Vec3f flamePos;

    SkelAnime_Update(&this->snapperSkelAnime);
    this->actor.colorFilterTimer = 100;
    if (this->timer > 0) {
        this->timer--;
        if (this->timer == 0) {
            SoundSource_PlaySfxEachFrameAtFixedWorldPos(play, &this->actor.world.pos, 21,
                                                        NA_SE_EN_COMMON_EXTINCT_LEV - SFX_FLAG);
        }
    } else {
        this->actor.scale.x -= 0.001f;
        if (this->actor.scale.x <= 0.0f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0x60);
            Actor_Kill(&this->actor);
        } else {
            this->actor.scale.y = this->actor.scale.x;
            this->actor.scale.z = this->actor.scale.x;
        }

        flamePos.x = Rand_CenteredFloat(40.0f) + this->actor.world.pos.x;
        flamePos.y = this->actor.world.pos.y + 15.0f;
        flamePos.z = Rand_CenteredFloat(40.0f) + this->actor.world.pos.z;
        func_800B3030(play, &flamePos, &gZeroVec3f, &gZeroVec3f, 100, 0, 2);
    }
}

void EnKame_UpdateDamage(EnKame* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        Actor_SetDropFlag(&this->actor, &this->collider.elem);
        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) &&
            (this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 0xDB0B3)) {
            return;
        }

        EnKame_Thaw(this, play);
        if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_HOOKSHOT) {
            return;
        }

        if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_INSTANT_KILL) {
            EnKame_SetupDead(this, play);
        } else if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_FLIP_OVER) {
            if (this->collider.base.acFlags & AC_HARD) {
                EnKame_SetupFlip(this);
            } else if (!Actor_ApplyDamage(&this->actor)) {
                EnKame_SetupDead(this, play);
            } else {
                EnKame_SetupDamaged(this);
            }
        } else if ((this->actionFunc == EnKame_Idle) || (this->actionFunc == EnKame_Walk)) {
            EnKame_SetupRetreatIntoShell(this);
            this->timer = 21;
        } else if (!(this->collider.base.acFlags & AC_HARD)) {
            if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_ELECTRIC_STUN) {
                this->stunTimer = 40;
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                this->drawDmgEffScale = 0.6f;
                this->drawDmgEffAlpha = 2.0f;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
                EnKame_SetupStunned(this);
            } else if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_STUN) {
                this->stunTimer = 40;
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                EnKame_SetupStunned(this);
            } else if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_FREEZE) {
                EnKame_Freeze(this);
                if (!Actor_ApplyDamage(&this->actor)) {
                    this->stunTimer = 3;
                    this->collider.base.acFlags &= ~AC_ON;
                }

                EnKame_SetupStunned(this);
            } else {
                if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_FIRE) {
                    this->drawDmgEffScale = 0.6f;
                    this->drawDmgEffAlpha = 4.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                } else if (this->actor.colChkInfo.damageEffect == EN_KAME_DMGEFF_LIGHT_ORB) {
                    this->drawDmgEffScale = 0.6f;
                    this->drawDmgEffAlpha = 4.0f;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->collider.elem.acDmgInfo.hitPos.x,
                                this->collider.elem.acDmgInfo.hitPos.y, this->collider.elem.acDmgInfo.hitPos.z, 0, 0, 0,
                                CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                }

                if (!Actor_ApplyDamage(&this->actor)) {
                    EnKame_SetupDead(this, play);
                } else {
                    EnKame_SetupDamaged(this);
                }
            }
        }
    }

    if ((play->actorCtx.unk_02 != 0) && (this->actor.xyzDistToPlayerSq < SQ(200.0f)) &&
        (this->collider.base.acFlags & AC_ON)) {
        EnKame_Thaw(this, play);
        EnKame_SetupFlip(this);
    }
}

void EnKame_Update(Actor* thisx, PlayState* play) {
    EnKame* this = (EnKame*)thisx;

    EnKame_Blink(this);

    DECR(this->voiceTimer);

    EnKame_UpdateDamage(this, play);

    if ((this->collider.base.atFlags & AT_HIT) && (this->collider.base.atFlags & AT_BOUNCED)) {
        this->collider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        EnKame_SetupPrepareToAttack(this);
        if (Actor_WorldDistXZToPoint(&this->actor, &this->targetPos) < 50.0f) {
            this->collider.base.atFlags &= ~AT_ON;
        }

        this->angularVelocityY = 0x3B00;
        EnKame_SetAttackSpeed(this);
    }

    this->actionFunc(this, play);

    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 40.0f, 60.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);

    if (this->actor.shape.shadowDraw != NULL) {
        Actor_SetFocus(&this->actor, 25.0f);
        Collider_UpdateCylinder(&this->actor, &this->collider);
    }

    if (this->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if (this->drawDmgEffAlpha > 0.0f) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
            this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.3f;
            this->drawDmgEffScale = CLAMP_MAX(this->drawDmgEffScale, 0.6f);
        } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.6f, 15.0f * 0.001f)) {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        }
    }
}

s32 EnKame_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnKame* this = (EnKame*)thisx;

    if ((this->actionFunc == EnKame_RetreatIntoShell) || (this->actionFunc == EnKame_EmergeFromShell)) {
        if (limbIndex == SNAPPER_LIMB_HEAD) {
            Matrix_Scale(this->limbScaleXZ, this->limbScaleY, this->limbScaleXZ, MTXMODE_APPLY);
        } else if ((limbIndex == SNAPPER_LIMB_BACK_RIGHT_LEG) || (limbIndex == SNAPPER_LIMB_BACK_LEFT_LEG) ||
                   (limbIndex == SNAPPER_LIMB_FRONT_RIGHT_LEG) || (limbIndex == SNAPPER_LIMB_FRONT_LEFT_LEG)) {
            Matrix_Scale(this->limbScaleXZ, this->limbScaleY, this->limbScaleY, MTXMODE_APPLY);
        }
    }

    return false;
}

static Vec3f sBodyLimbBodyPartOffsets[] = {
    { 1500.0f, 0.0f, -2000.0f }, { 1500.0f, 0.0f, 2000.0f }, { 1500.0f, 2000.0f, 0.0f },
    { 1500.0f, -2000.0f, 0.0f }, { 2500.0f, 0.0f, 0.0f },
};

static s8 sLimbToBodyParts[SNAPPER_LIMB_MAX] = {
    BODYPART_NONE,
    BODYPART_NONE,
    BODYPART_NONE,
    SNAPPER_BODYPART_JAW,
    BODYPART_NONE,
    BODYPART_NONE,
    SNAPPER_BODYPART_FRONT_LEFT_FOOT,
    BODYPART_NONE,
    SNAPPER_BODYPART_FRONT_RIGHT_FOOT,
    BODYPART_NONE,
    SNAPPER_BODYPART_BACK_LEFT_FOOT,
    BODYPART_NONE,
    SNAPPER_BODYPART_BACK_RIGHT_FOOT,
};

void EnKame_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnKame* this = (EnKame*)thisx;

    if (sLimbToBodyParts[limbIndex] != BODYPART_NONE) {
        Matrix_MultZero(&this->bodyPartsPos[sLimbToBodyParts[limbIndex]]);
    }

    if (limbIndex == SNAPPER_LIMB_BODY) {
        s32 i;
        Vec3f* bodyPartsPosPtr;
        Vec3f* bodyLimbOffsetsPtr;

        if (this->actor.shape.shadowDraw == NULL) {
            Matrix_MultZero(&this->actor.world.pos);
        }

        bodyLimbOffsetsPtr = sBodyLimbBodyPartOffsets;
        bodyPartsPosPtr = &this->bodyPartsPos[SNAPPER_BODYPART_BODY_1];
        for (i = 0; i < ARRAY_COUNT(sBodyLimbBodyPartOffsets); i++) {
            Matrix_MultVec3f(bodyLimbOffsetsPtr, bodyPartsPosPtr);
            bodyLimbOffsetsPtr++;
            bodyPartsPosPtr++;
        }
    }
}

void EnKame_Draw(Actor* thisx, PlayState* play) {
    EnKame* this = (EnKame*)thisx;
    Vec3f originalPos;

    if (this->actor.shape.shadowDraw == NULL) {
        Math_Vec3f_Copy(&originalPos, &this->actor.world.pos);
    }

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, sEyeTextures[this->eyeIndex]);

    SkelAnime_DrawFlexOpa(play, this->snapperSkelAnime.skeleton, this->snapperSkelAnime.jointTable,
                          this->snapperSkelAnime.dListCount, EnKame_OverrideLimbDraw, EnKame_PostLimbDraw,
                          &this->actor);
    Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, SNAPPER_BODYPART_MAX, this->drawDmgEffScale,
                            this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha, this->drawDmgEffType);

    if (this->actor.shape.shadowDraw == NULL) {
        ActorShadow_DrawCircle(&this->actor, NULL, play);
        Actor_SetFocus(&this->actor, 25.0f);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        Math_Vec3f_Copy(&this->actor.world.pos, &originalPos);
    }

    CLOSE_DISPS();
}

s32 EnKame_SpikedSnapperOverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                         Actor* thisx) {
    EnKame* this = (EnKame*)thisx;

    if (limbIndex == SPIKED_SNAPPER_LIMB_BODY) {
        pos->y -= 700.0f;
    }

    if ((this->spikesScale != 1.0f) && (limbIndex == SPIKED_SNAPPER_LIMB_SPIKES)) {
        Matrix_Scale(1.0f, this->spikesScale, this->spikesScale, MTXMODE_APPLY);
    }

    return false;
}

void EnKame_DrawSpikedSnapper(Actor* thisx, PlayState* play) {
    EnKame* this = (EnKame*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->spikedSnapperSkelAnime.skeleton, this->spikedSnapperSkelAnime.jointTable,
                          this->spikedSnapperSkelAnime.dListCount, EnKame_SpikedSnapperOverrideLimbDraw, NULL,
                          &this->actor);
}

OVL_INFO_NATIVE_ENEMY(0x1ba, En_Kame_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_KAME, En_Kame_Profile, EnKame_Init, EnKame_Destroy, EnKame_Update,
                     EnKame_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x1bau, 0x1a6u, 0x7b405b21u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_KAME_ORIGIN, generatedNativeOrigin);
