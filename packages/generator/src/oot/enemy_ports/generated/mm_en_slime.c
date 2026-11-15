#include "mm_native_compat.h"

static Gfx sEmptyDisplayList[] = { gsSPEndDisplayList() };

#define gDropArrows1Tex (*((u64 (*)[1])0x0405bef0))
#define gDropRecoveryHeartTex (*((u64 (*)[1])0x0405e6f0))
#define gItemDropDL (*((Gfx (*)[1])0x0405f6f0))
#define gDropMagicSmallTex (*((u64 (*)[1])0x0405ffc0))
#define gChuchuBodyDL (*((Gfx (*)[1])0x060004c0))
#define gChuchuEyesDL (*((Gfx (*)[1])0x06000650))
#define gChuchuSlimeFlowTexAnim (*((u8 (*)[1])0x06000828))
#define gChuchuPuddleDL (*((Gfx (*)[1])0x06000a10))
#define gChuchuEyeOpenTex (*((u64 (*)[1])0x06000f70))
#define gChuchuEyeHalfTex (*((u64 (*)[1])0x06001070))
#define gChuchuEyeClosedTex (*((u64 (*)[1])0x06001170))

#include "types/en_slime.h"
#define TATL_HINT_ID_BLUE_CHUCHU 0xff
#define TATL_HINT_ID_GREEN_CHUCHU 0xff
#define TATL_HINT_ID_RED_CHUCHU 0xff
#define TATL_HINT_ID_YELLOW_CHUCHU 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_OBJ_ICEBLOCK
#define ACTOR_OBJ_ICEBLOCK 0x143
#undef ACTOR_EN_SLIME
#define ACTOR_EN_SLIME 0x14a
#undef OBJECT_SLIME
#define OBJECT_SLIME 0x116a
#undef NA_SE_EV_ICE_FREEZE
#define NA_SE_EV_ICE_FREEZE 0x28b2
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_SLIME_JUMP
#define NA_SE_EN_SLIME_JUMP 0x38b9
#undef NA_SE_EN_SLIME_DAMAGE
#define NA_SE_EN_SLIME_DAMAGE 0x38ba
#undef NA_SE_EN_SLIME_BREAK
#define NA_SE_EN_SLIME_BREAK 0x38bb
#undef NA_SE_EN_SLIME_DEAD
#define NA_SE_EN_SLIME_DEAD 0x38be
#undef NA_SE_EN_SLIME_DEFENCE
#define NA_SE_EN_SLIME_DEFENCE 0x38bf
#undef NA_SE_EN_SLIME_SURFACE
#define NA_SE_EN_SLIME_SURFACE 0x39fe
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
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef func_800BE504
#define func_800BE504 ((void (*)(Actor* actor, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_HIT_YAW_CYLINDER)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef Item_DropCollectible
#define Item_DropCollectible ((Actor* (*)(struct PlayState* play, Vec3f* spawnPos, u32 params))(void*)FOREIGN_ENEMY_API_DROP)
#undef Actor_SpawnAsChild
#define Actor_SpawnAsChild ((Actor* (*)(ActorContext* actorCtx, Actor* parent, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN_CHILD)
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
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef AnimatedMat_Draw
#define AnimatedMat_Draw ((void (*)(struct PlayState* play, AnimatedMaterial* matAnim))(void*)FOREIGN_ENEMY_API_MATERIAL_DRAW)
#undef Matrix_RotateXFApply
#define Matrix_RotateXFApply ((void (*)(f32 x))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_APPLY)
#undef Matrix_RotateZF
#define Matrix_RotateZF ((void (*)(f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Scene_SetRenderModeXlu
#define Scene_SetRenderModeXlu ((void (*)(struct PlayState* play, s32 index, u32 flags))(void*)FOREIGN_ENEMY_API_RENDER_XLU)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Math_SinF
#define Math_SinF ((f32 (*)(f32 rad))0x800a45fcu)
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 rad))0x800a4650u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 angle))0x80021a28u)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Actor_WorldYawTowardActor
#define Actor_WorldYawTowardActor ((s16 (*)(Actor* actorA, Actor* actorB))0x800213b4u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef EffectSsDtBubble_SpawnCustomColor
#define EffectSsDtBubble_SpawnCustomColor ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 life, s16 randXZ))0x8001d18cu)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef func_800B8118
#define func_800B8118 ((void (*)(Actor* actor, struct PlayState* play, s32 flag))0x80022554u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Play_SetFog
#define Play_SetFog ((Gfx* (*)(PlayState* this, Gfx* gfx))0x8009a574u)
#undef Gfx_SetupDL66
#define Gfx_SetupDL66 ((Gfx* (*)(Gfx* gfx))0x8007e6e0u)

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR)

#define ICE_BLOCK_TIMER_MAX 254
#define ICE_BLOCK_UNUSED (ICE_BLOCK_TIMER_MAX + 1)

typedef enum EnSlimeEyeTexture {
    EN_SLIME_EYETEX_OPEN,
    EN_SLIME_EYETEX_HALF,
    EN_SLIME_EYETEX_CLOSED,
    EN_SLIME_EYETEX_HALF2,
    EN_SLIME_EYETEX_MAX
} EnSlimeEyeTexture;

typedef enum EnSlimeDamageEffect {
    EN_SLIME_DMGEFF_NORMAL,
    EN_SLIME_DMGEFF_STUN,
    EN_SLIME_DMGEFF_FREEZE = 0x3,
    EN_SLIME_DMGEFF_LIGHT,
    EN_SLIME_DMGEFF_ELECTRIC,
    EN_SLIME_DMGEFF_HOOKSHOT = 0xE,
    EN_SLIME_DMGEFF_BLUNT
} EnSlimeDamageEffect;

void EnSlime_Init(Actor* thisx, PlayState* play);
void EnSlime_Destroy(Actor* thisx, PlayState* play);
void EnSlime_Update(Actor* thisx, PlayState* play);
void EnSlime_Draw(Actor* thisx, PlayState* play);

void EnSlime_SetupInitializeIdle(EnSlime* this);
void EnSlime_InitializeIdle(EnSlime* this, PlayState* play);
void EnSlime_SetupIdle(EnSlime* this);
void EnSlime_Idle(EnSlime* this, PlayState* play);
void EnSlime_SetupMoveInDirection(EnSlime* this);
void EnSlime_MoveInDirection(EnSlime* this, PlayState* play);
void EnSlime_SetupMoveToHome(EnSlime* this);
void EnSlime_MoveToHome(EnSlime* this, PlayState* play);
void EnSlime_SetupTurnToPlayer(EnSlime* this);
void EnSlime_TurnToPlayer(EnSlime* this, PlayState* play);
void EnSlime_SetupJump(EnSlime* this);
void EnSlime_Jump(EnSlime* this, PlayState* play);
void EnSlime_SetupLand(EnSlime* this);
void EnSlime_Land(EnSlime* this, PlayState* play);
void EnSlime_ReactToBluntHit(EnSlime* this, PlayState* play);
void EnSlime_Damaged(EnSlime* this, PlayState* play);
void EnSlime_SetupDead(EnSlime* this);
void EnSlime_Dead(EnSlime* this, PlayState* play);
void EnSlime_SpawnIceBlock(EnSlime* this, PlayState* play);
void EnSlime_SetupIceBlock(EnSlime* this);
void EnSlime_IceBlock(EnSlime* this, PlayState* play);
void EnSlime_Stun(EnSlime* this, PlayState* play);
void EnSlime_SetupIceBlockThaw(EnSlime* this);
void EnSlime_IceBlockThaw(EnSlime* this, PlayState* play);
void EnSlime_SetupWaitForRevive(EnSlime* this);
void EnSlime_WaitForRevive(EnSlime* this, PlayState* play);
void EnSlime_SetupRevive(EnSlime* this);
void EnSlime_Revive(EnSlime* this, PlayState* play);

ActorProfile En_Slime_Profile = {
    0xd4a, ACTORCAT_ENEMY, FLAGS, 0x116a, sizeof(EnSlime), EnSlime_Init, EnSlime_Destroy, EnSlime_Update, EnSlime_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_HARD,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 22, 35, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_SLIME_DMGEFF_STUN),   DMG_ENTRY(3, EN_SLIME_DMGEFF_NORMAL),   DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),   DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_HOOKSHOT), DMG_ENTRY(0, EN_SLIME_DMGEFF_BLUNT),
    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_BLUNT),    DMG_ENTRY(2, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(2, EN_SLIME_DMGEFF_FREEZE), DMG_ENTRY(2, EN_SLIME_DMGEFF_LIGHT),    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_BLUNT),    DMG_ENTRY(2, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(0, EN_SLIME_DMGEFF_STUN),   DMG_ENTRY(1, EN_SLIME_DMGEFF_ELECTRIC), DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_BLUNT),    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL),   DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL),   DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL),
    DMG_ENTRY(0, EN_SLIME_DMGEFF_NORMAL), DMG_ENTRY(1, EN_SLIME_DMGEFF_NORMAL),
} };

static CollisionCheckInfoInit sColChkInfoInit = { 1, 22, 35, 60 };

static TexturePtr sEyeTextures[] = {
    gChuchuEyeOpenTex,
    gChuchuEyeHalfTex,
    gChuchuEyeClosedTex,
    gChuchuEyeHalfTex,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(gravity, -2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 6000, ICHAIN_STOP),
};

static s32 sTexturesDesegmented = false;

static Color_RGBA8 sBubblePrimColor = { 255, 255, 255, 255 };
static Color_RGBA8 sBubbleEnvColor = { 150, 150, 150, 0 };
static Vec3f sBubbleAccel = { 0.0f, -0.8f, 0.0f };

AnimatedMaterial* sSlimeTexAnim;

void EnSlime_Init(Actor* thisx, PlayState* play) {
    EnSlime* this = (EnSlime*)thisx;
    s32 reviveTimeSeconds;
    s32 i;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 38.0f);

    this->iceBlockTimer = ICE_BLOCK_UNUSED;
    this->eyeTexIndex = EN_SLIME_EYETEX_OPEN;
    this->wobbleRot.y = 0.0f;

    if (this->actor.shape.rot.x <= 0) {
        this->distLimit = 30000.0f;
    } else {
        this->distLimit = this->actor.shape.rot.x * 40.0f;
    }

    this->actor.shape.rot.x = 0;
    reviveTimeSeconds = EN_SLIME_GET_REVIVE_TIME(&this->actor);
    this->actor.shape.rot.z = 0;
    this->actor.params &= 0xFF;
    if (reviveTimeSeconds == 0xFF) {
        reviveTimeSeconds = 0;
    }

    this->reviveTime = (reviveTimeSeconds * 20) + 200;

    if (!sTexturesDesegmented) {
        for (i = 0; i < EN_SLIME_EYETEX_MAX; i++) {
            sEyeTextures[i] = Lib_SegmentedToVirtual(sEyeTextures[i]);
        }
        sSlimeTexAnim = Lib_SegmentedToVirtual(gChuchuSlimeFlowTexAnim);
        sTexturesDesegmented = true;
    }

    if (EN_SLIME_GET_TYPE(&this->actor) == EN_SLIME_TYPE_YELLOW) {
        this->itemDropTex = Lib_SegmentedToVirtual(gDropArrows1Tex);
        this->actor.hintId = TATL_HINT_ID_YELLOW_CHUCHU;
    } else if (EN_SLIME_GET_TYPE(&this->actor) == EN_SLIME_TYPE_GREEN) {
        this->itemDropTex = Lib_SegmentedToVirtual(gDropMagicSmallTex);
        this->actor.hintId = TATL_HINT_ID_GREEN_CHUCHU;
    } else if (EN_SLIME_GET_TYPE(&this->actor) == EN_SLIME_TYPE_RED) {
        this->itemDropTex = Lib_SegmentedToVirtual(gDropRecoveryHeartTex);
        this->actor.hintId = TATL_HINT_ID_RED_CHUCHU;
    } else {
        this->actor.hintId = TATL_HINT_ID_BLUE_CHUCHU;
    }

    EnSlime_SetupInitializeIdle(this);
}

void EnSlime_Destroy(Actor* thisx, PlayState* play) {
    EnSlime* this = (EnSlime*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnSlime_Freeze(EnSlime* this) {
    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
    this->drawDmgEffScale = 0.4f;
    this->collider.base.colMaterial = COL_MATERIAL_HIT3;
    this->drawDmgEffFrozenSteamScale = 0.6f;
    this->drawDmgEffAlpha = 1.0f;
    this->timer = 80;
    this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 80);
}

void EnSlime_Thaw(EnSlime* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        this->drawDmgEffType = 0;
        this->collider.base.colMaterial = COL_MATERIAL_NONE;
        this->drawDmgEffAlpha = 0.0f;
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_SLIME_BODYPART_MAX, 2, 0.2f, 0.2f);
        this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
    }
}

void EnSlime_Blink(EnSlime* this) {
    if (this->eyeTexIndex != EN_SLIME_EYETEX_OPEN) {
        this->eyeTexIndex++;
        if (this->eyeTexIndex == EN_SLIME_EYETEX_MAX) {
            this->eyeTexIndex = EN_SLIME_EYETEX_OPEN;
        }
    } else if (Rand_ZeroOne() < 0.05f) {
        this->eyeTexIndex = EN_SLIME_EYETEX_HALF;
    }
}

void EnSlime_SetupInitializeIdle(EnSlime* this) {
    this->actor.scale.x = 0.008f;
    this->actor.scale.z = 0.008f;
    this->actor.scale.y = 0.011f;
    this->actionFunc = EnSlime_InitializeIdle;
}

void EnSlime_InitializeIdle(EnSlime* this, PlayState* play) {
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        EnSlime_SetupIdle(this);
    }
}

void EnSlime_SetupIdle(EnSlime* this) {
    this->timer = 25;
    this->actor.speed = 0.0f;
    this->actionFunc = EnSlime_Idle;
}

void EnSlime_Idle(EnSlime* this, PlayState* play) {
    f32 timerFactor;
    f32 scale;

    this->timer--;
    timerFactor = sqrtf(this->timer) * 0.2f;
    EnSlime_Blink(this);

    scale = ((Math_SinF(this->timer * (2 * M_PIf / 5)) * (0.13f * timerFactor)) + 1.0f) * 0.01f;
    this->actor.scale.x = scale;
    this->actor.scale.z = scale;
    if (this->timer < 24) {
        this->actor.scale.y = ((Math_CosF(this->timer * (2 * M_PIf / 5)) * (0.05f * timerFactor)) + 1.0f) * 0.01f;
    }

    this->actor.shape.rot.x = Rand_CenteredFloat(0x200) * timerFactor;
    this->actor.shape.rot.z = Rand_CenteredFloat(0x200) * timerFactor;

    if ((Player_GetMask(play) != PLAYER_MASK_STONE) && (this->actor.xzDistToPlayer < 280.0f) &&
        Actor_IsFacingPlayer(&this->actor, 0x5000)) {
        EnSlime_SetupTurnToPlayer(this);
    } else if (this->timer == 0) {
        EnSlime_SetupMoveInDirection(this);
    }
}

void EnSlime_SetupMoveInDirection(EnSlime* this) {
    this->timer = 120;

    if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 120.0f) {
        this->idleRotY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    } else if (Rand_ZeroOne() < 0.7f) {
        this->idleRotY = (s32)Rand_CenteredFloat(0x4000) + this->actor.shape.rot.y;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = EnSlime_MoveInDirection;
}

void EnSlime_MoveInDirection(EnSlime* this, PlayState* play) {
    f32 sinFactor;

    EnSlime_Blink(this);
    this->timer--;
    Math_ApproachS(&this->actor.shape.rot.y, this->idleRotY, 4, 0x100);

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->idleRotY = this->actor.wallYaw;
    } else if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 120.0f) {
        this->idleRotY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    }

    sinFactor = fabsf(Math_SinF(this->timer * (M_PIf / 24)));
    Math_StepToF(&this->actor.scale.z, ((0.15f * sinFactor) + 1.0f) * 0.01f, 0.0002f);
    Math_StepToF(&this->actor.scale.x, (1.0f - (0.2f * sinFactor)) * 0.01f, 0.0002f);
    Math_StepToF(&this->actor.scale.y, (1.0f - (0.1f * sinFactor)) * 0.01f, 0.0002f);

    this->actor.speed = (0.8f * sinFactor) + 0.2f;
    this->actor.shape.rot.x = 0x800 * sinFactor;
    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > this->distLimit) {
        EnSlime_SetupMoveToHome(this);
    } else if ((Player_GetMask(play) != PLAYER_MASK_STONE) && (this->actor.xzDistToPlayer < 280.0f) &&
               Actor_IsFacingPlayer(&this->actor, 0x5000)) {
        EnSlime_SetupTurnToPlayer(this);
    } else if (this->timer == 0) {
        EnSlime_SetupIdle(this);
    }
}

void EnSlime_SetupMoveToHome(EnSlime* this) {
    this->timer = 24;
    this->idleRotY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = EnSlime_MoveToHome;
}

void EnSlime_MoveToHome(EnSlime* this, PlayState* play) {
    f32 timerFactor;

    EnSlime_Blink(this);
    this->timer--;
    Math_ApproachS(&this->actor.shape.rot.y, this->idleRotY, 4, 0x400);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->idleRotY = this->actor.wallYaw;
    } else {
        this->idleRotY = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    }

    timerFactor = fabsf(Math_SinF(this->timer * (M_PIf / 24)));
    Math_StepToF(&this->actor.scale.z, ((0.15f * timerFactor) + 1.0f) * 0.01f, 0.0002f);
    Math_StepToF(&this->actor.scale.x, (1.0f - (0.2f * timerFactor)) * 0.01f, 0.0002f);
    Math_StepToF(&this->actor.scale.y, (1.0f - (0.1f * timerFactor)) * 0.01f, 0.0002f);

    this->actor.speed = (0.8f * timerFactor) + 0.2f;
    this->actor.shape.rot.x = 0x800 * timerFactor;
    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (this->timer == 0) {
        this->timer = 24;
    }

    if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) < (this->distLimit * 0.8f)) {
        EnSlime_SetupIdle(this);
    }
}

void EnSlime_SetupTurnToPlayer(EnSlime* this) {
    this->actor.shape.rot.x = 0;
    this->actor.shape.rot.z = 0;
    this->timer = 8;
    this->actionFunc = EnSlime_TurnToPlayer;
    this->actor.speed = 0.0f;
}

void EnSlime_TurnToPlayer(EnSlime* this, PlayState* play) {
    f32 factorY;
    f32 scaleXZ;

    this->timer--;
    EnSlime_Blink(this);
    Math_ApproachS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 4, 0x1000);
    if (this->timer >= 0) {
        factorY = 8 - this->timer;
        scaleXZ = ((factorY * 0.04f) + 1.0f) * 0.01f;
        this->actor.scale.x = scaleXZ;
        this->actor.scale.y = (1.0f - (factorY * 0.05f)) * 0.01f;
        this->actor.scale.z = scaleXZ;
    }
    if (this->timer < -2) {
        EnSlime_SetupJump(this);
    }
}

void EnSlime_SetupJump(EnSlime* this) {
    if (this->actor.xzDistToPlayer > 120.0f) {
        this->actor.velocity.y = 11.0f;
        this->actor.speed = 5.0f;
        this->actor.gravity = -2.0f;
    } else {
        this->actor.velocity.y = 18.0f;
        this->actor.speed = 7.0f;
        this->actor.gravity = -3.5f;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->timer = 12;
    this->eyeTexIndex = EN_SLIME_EYETEX_OPEN;
    Math_StepToF(&this->actor.scale.x, 0.008f, 0.0025f);
    Math_StepToF(&this->actor.scale.y, 0.011f, 0.0025f);
    this->actor.scale.z = this->actor.scale.x;
    Actor_PlaySfx(&this->actor, NA_SE_EN_SLIME_JUMP);
    this->actionFunc = EnSlime_Jump;
}

void EnSlime_Jump(EnSlime* this, PlayState* play) {
    this->timer--;
    if (this->collider.base.atFlags & AT_HIT) {
        if (this->actor.speed > 0.0f) {
            this->actor.speed *= -1.2f;
            this->collider.base.atFlags &= ~AT_HIT;
        }
    }
    Math_StepToF(&this->actor.scale.x, 0.008f, 0.0025f);
    Math_StepToF(&this->actor.scale.y, 0.011f, 0.0025f);
    this->actor.scale.z = this->actor.scale.x;
    if ((this->actor.velocity.y < 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        EnSlime_SetupLand(this);
    }
}

void EnSlime_SetupLand(EnSlime* this) {
    this->timer = 15;
    this->actor.scale.x = 0.0132f;
    this->actor.scale.y = 0.0074399994f;
    this->actor.scale.z = 0.0132f;
    this->actor.speed = 0.0f;
    this->actionFunc = EnSlime_Land;
}

void EnSlime_Land(EnSlime* this, PlayState* play) {
    f32 scaleY;
    f32 rotXZ;
    f32 scaleXZ;

    EnSlime_Blink(this);
    this->timer--;
    scaleY = ((this->timer / 5) + 1) * 1.6f;
    rotXZ = sqrtf(this->timer) * 0.2f;
    scaleXZ = ((Math_CosF(this->timer * (2 * M_PIf / 5)) * (0.05f * scaleY)) + 1.0f) * 0.01f;
    this->actor.scale.x = scaleXZ;
    this->actor.scale.z = scaleXZ;
    if (this->timer < 15) {
        this->actor.scale.y = (1.0f - (Math_SinF(this->timer * (2 * M_PIf / 5)) * (0.04f * scaleY))) * 0.01f;
    }
    this->actor.shape.rot.x = Rand_CenteredFloat(0x200) * rotXZ;
    this->actor.shape.rot.z = Rand_CenteredFloat(0x200) * rotXZ;

    if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > this->distLimit) {
        EnSlime_SetupMoveToHome(this);
    } else if ((Player_GetMask(play) != PLAYER_MASK_STONE) && (this->actor.xzDistToPlayer < 280.0f) &&
               (this->timer < 12)) {
        EnSlime_SetupTurnToPlayer(this);
    } else if (this->timer == 0) {
        EnSlime_SetupIdle(this);
    }
}

void EnSlime_SetupReactToBluntHit(EnSlime* this) {
    this->actor.speed = 10.0f;
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }
    this->collider.base.acFlags &= ~AC_ON;
    this->timer = 30;
    Actor_PlaySfx(&this->actor, NA_SE_EN_SLIME_DEFENCE);
    if (this->collider.base.ac != NULL) {
        this->actor.world.rot.y = Actor_WorldYawTowardActor(&this->actor, this->collider.base.ac) + 0x8000;
    } else {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer + 0x8000;
    }
    this->actionFunc = EnSlime_ReactToBluntHit;
}

void EnSlime_ReactToBluntHit(EnSlime* this, PlayState* play) {
    f32 timerFactor;
    f32 scale;

    EnSlime_Blink(this);
    this->timer--;
    Math_StepToF(&this->actor.speed, 0.0f, 1.0f);
    timerFactor = sqrtf(this->timer);
    if (this->timer < 30) {
        scale = ((Math_CosF(this->timer * (2 * M_PIf / 5)) * (0.08f * timerFactor)) + 1.0f) * 0.01f;
        this->actor.scale.x = scale;
        this->actor.scale.z = scale;
    }
    if (this->timer == 15) {
        this->collider.base.acFlags |= AC_ON;
    }
    this->actor.scale.y = ((Math_SinF((f32)this->timer * (2 * M_PIf / 5)) * (0.07f * timerFactor)) + 1.0f) * 0.01f;
    this->actor.shape.rot.x = Rand_CenteredFloat(0x200) * timerFactor;
    this->actor.shape.rot.z = Rand_CenteredFloat(0x200) * timerFactor;

    if (this->timer == 0) {
        if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > this->distLimit) {
            EnSlime_SetupMoveToHome(this);
        } else if ((this->actor.xzDistToPlayer < 280.0f) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
            EnSlime_SetupTurnToPlayer(this);
        } else {
            EnSlime_SetupIdle(this);
        }
    }
}

void EnSlime_SetupDamaged(EnSlime* this, PlayState* play, s32 arg2) {
    s32 i;
    Vec3f effectVelocity;
    Vec3f effectPos;
    f32 ySin;
    f32 yCos;

    this->collider.base.acFlags &= ~AC_ON;
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 20);
    this->timer = 20;
    this->actor.speed = 10.0f;
    if (arg2 == true) {
        func_800BE504(&this->actor, &this->collider);
    }

    this->eyeTexIndex = EN_SLIME_EYETEX_OPEN;
    Actor_SetScale(&this->actor, 0.01f);
    this->wobbleRot.x = Rand_ZeroOne() * (M_PIf * 2);
    this->wobbleRot.z = Rand_ZeroOne() * (M_PIf * 2);
    ySin = Math_SinS(this->actor.world.rot.y) * 10.0f;
    yCos = Math_CosS(this->actor.world.rot.y) * 10.0f;
    effectPos.x = this->actor.world.pos.x + ySin;
    effectPos.y = this->actor.world.pos.y + 20.0f;
    effectPos.z = this->actor.world.pos.z + yCos;
    EffectSsGSplash_Spawn(play, &effectPos, NULL, NULL, 1, 550);

    for (i = 0; i < 10; i++) {
        effectPos.x = Rand_CenteredFloat(40.0f) + this->actor.world.pos.x + ySin;
        effectPos.y = Rand_CenteredFloat(10.0f) + this->actor.world.pos.y + 40.0f;
        effectPos.z = Rand_CenteredFloat(40.0f) + this->actor.world.pos.z + yCos;

        effectVelocity.x = ((Rand_ZeroOne() * 3.5f) + 1.0f) * -Math_SinS(this->actor.world.rot.y);
        effectVelocity.z = ((Rand_ZeroOne() * 3.5f) + 1.0f) * -Math_CosS(this->actor.world.rot.y);

        effectVelocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        EffectSsDtBubble_SpawnCustomColor(play, &effectPos, &effectVelocity, &sBubbleAccel, &sBubblePrimColor,
                                          &sBubbleEnvColor, Rand_S16Offset(40, 20), 20, 0);
    }

    if (this->actor.colChkInfo.health == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_SLIME_DEAD);
    } else {
        Actor_PlaySfx(&this->actor, NA_SE_EN_SLIME_DAMAGE);
    }
    this->actionFunc = EnSlime_Damaged;
}

void EnSlime_Damaged(EnSlime* this, PlayState* play) {
    this->timer--;
    Math_StepToF(&this->actor.speed, 0.0f, 1.0f);
    if ((this->timer % 5) == 0) {
        this->wobbleRot.x = Rand_ZeroOne() * (M_PIf * 2);
        this->wobbleRot.z = Rand_ZeroOne() * (M_PIf * 2);
    }
    if (this->timer == 0) {
        if (this->actor.colChkInfo.health != 0) {
            this->collider.base.acFlags |= AC_ON;
            if ((this->actor.xzDistToPlayer < 280.0f) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
                EnSlime_SetupTurnToPlayer(this);
            } else {
                EnSlime_SetupIdle(this);
            }
        } else {
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EN_SLIME_BREAK);
            EnSlime_SetupDead(this);
        }
    }
}

void EnSlime_SetupDead(EnSlime* this) {
    this->collider.base.acFlags &= ~AC_ON;
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.speed = 0.0f;
    this->actionFunc = EnSlime_Dead;
}

void EnSlime_Dead(EnSlime* this, PlayState* play) {
    s32 i;
    Vec3f velocity;
    Vec3f effectPos;
    f32 randFloat;
    s32 randSign;

    if (Math_StepToF(&this->actor.scale.x, 0.0f, 0.002f)) {
        effectPos.x = this->actor.world.pos.x;
        effectPos.y = this->actor.world.pos.y + 20.0f;
        effectPos.z = this->actor.world.pos.z;
        EffectSsGSplash_Spawn(play, &effectPos, NULL, NULL, 1, 800);

        for (i = 0; i < 15; i++) {
            randFloat = Rand_ZeroOne();
            randSign = Rand_ZeroOne() < 0.5f ? -1 : 1;
            velocity.x = randSign * ((randFloat * 2.5f) + 2.0f);

            randFloat = Rand_ZeroOne();
            randSign = Rand_ZeroOne() < 0.5f ? -1 : 1;
            velocity.z = randSign * ((randFloat * 2.5f) + 2.0f);

            velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
            EffectSsDtBubble_SpawnCustomColor(play, &effectPos, &velocity, &sBubbleAccel, &sBubblePrimColor,
                                              &sBubbleEnvColor, Rand_S16Offset(40, 20), 20, 0);
        }

        if (this->actor.params == EN_SLIME_TYPE_YELLOW) {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_ARROWS_10);
        } else if (this->actor.params == EN_SLIME_TYPE_GREEN) {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_MAGIC_JAR_SMALL);
        } else if (this->actor.params == EN_SLIME_TYPE_RED) {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RECOVERY_HEART);
        }
        EnSlime_SetupWaitForRevive(this);
    }
    this->actor.scale.y = this->actor.scale.x;
    this->actor.scale.z = this->actor.scale.x;
}

f32 EnSlime_SnapIceBlockPosition(f32 currentPosition, f32 homePosition) {
    s32 dist = currentPosition - homePosition;

    if (dist > 0) {
        dist += 30;
    } else {
        dist -= 30;
    }

    return ((dist / 60) * 60.0f) + homePosition;
}

void EnSlime_SetupSpawnIceBlock(EnSlime* this) {
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->drawDmgEffAlpha = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = 0.0f;
    this->iceBlockSnapPos.x = EnSlime_SnapIceBlockPosition(this->actor.world.pos.x, this->actor.home.pos.x);
    this->iceBlockSnapPos.z = EnSlime_SnapIceBlockPosition(this->actor.world.pos.z, this->actor.home.pos.z);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, COLORFILTER_INTENSITY_FLAG | 255,
                         COLORFILTER_BUFFLAG_XLU, 10);
    this->eyeTexIndex = EN_SLIME_EYETEX_OPEN;
    this->iceBlockTimer = ICE_BLOCK_TIMER_MAX;
    this->actionFunc = EnSlime_SpawnIceBlock;
}

void EnSlime_SpawnIceBlock(EnSlime* this, PlayState* play) {
    s32 stepCheck;

    stepCheck = Math_StepToF(&this->actor.world.pos.x, this->iceBlockSnapPos.x, 10.0f);
    stepCheck &= Math_StepToF(&this->actor.world.pos.z, this->iceBlockSnapPos.z, 10.0f);

    this->actor.colorFilterTimer = 10;
    if (stepCheck) {
        this->actor.child =
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_OBJ_ICEBLOCK, this->actor.world.pos.x,
                               this->actor.world.pos.y + 30.0f, this->actor.world.pos.z, 0, 0, 0, 0);
        if (this->actor.child != NULL) {

            EnSlime_SetupIceBlock(this);
        } else {
            this->actor.colorFilterTimer = 0;
            this->collider.base.acFlags |= AC_ON;
            this->iceBlockTimer = ICE_BLOCK_UNUSED;
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            this->actor.gravity = -2.0f;
            EnSlime_SetupIdle(this);
        }
    }
}

void EnSlime_SetupIceBlock(EnSlime* this) {
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actionFunc = EnSlime_IceBlock;
}

void EnSlime_IceBlock(EnSlime* this, PlayState* play) {
    if (this->iceBlockTimer == 0) {
        if ((this->actor.child == NULL) || (this->actor.child->update == NULL) ||
            !(this->actor.child->scale.y >= 0.1f)) {
            EnSlime_SetupIceBlockThaw(this);
        }
    } else {
        this->actor.colorFilterTimer = 10;
        this->iceBlockTimer = CLAMP_MIN(this->iceBlockTimer - 5, 0);
    }
}

void EnSlime_SetupStun(EnSlime* this) {
    this->actor.speed = 0.0f;
    func_800BE504(&this->actor, &this->collider);
    if (this->actor.velocity.y > 0.0f) {
        this->actor.velocity.y = 0.0f;
    }
    this->actionFunc = EnSlime_Stun;
}

void EnSlime_Stun(EnSlime* this, PlayState* play) {
    this->timer--;
    if (this->timer == 0) {
        EnSlime_Thaw(this, play);
        if (this->actor.colChkInfo.health == 0) {
            EnSlime_SetupDamaged(this, play, false);
        } else {
            this->actor.world.rot.y = this->actor.shape.rot.y;
            EnSlime_SetupMoveInDirection(this);
        }
    }
}

void EnSlime_SetupIceBlockThaw(EnSlime* this) {
    this->actor.colorFilterTimer = 0;
    this->actor.gravity = -2.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, COLORFILTER_INTENSITY_FLAG | 255,
                         COLORFILTER_BUFFLAG_XLU, 10);
    this->actionFunc = EnSlime_IceBlockThaw;
}

void EnSlime_IceBlockThaw(EnSlime* this, PlayState* play) {
    s32 iceBlockTimerAdj;

    this->actor.colorFilterTimer = 10;
    if ((this->actor.child != NULL) && (this->actor.child->update != NULL)) {
        iceBlockTimerAdj = (0.1f - this->actor.child->scale.y) * 10.0f * 255.0f;
        this->iceBlockTimer = CLAMP(iceBlockTimerAdj, 0, ICE_BLOCK_UNUSED);
    } else {
        this->actor.child = NULL;
        iceBlockTimerAdj = this->iceBlockTimer + 10;
        this->iceBlockTimer = CLAMP_MAX(iceBlockTimerAdj, ICE_BLOCK_UNUSED);
    }

    if (this->iceBlockTimer == ICE_BLOCK_UNUSED) {
        this->collider.base.acFlags |= AC_ON;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        EnSlime_SetupIdle(this);
    }
}

void EnSlime_SetupWaitForRevive(EnSlime* this) {

    this->actionFunc = EnSlime_WaitForRevive;
    Actor_Kill(&this->actor);
}

void EnSlime_WaitForRevive(EnSlime* this, PlayState* play) {
    if (this->timer == 0) {
        EnSlime_SetupRevive(this);
    } else {
        this->timer--;
    }
}

void EnSlime_SetupRevive(EnSlime* this) {
    this->actor.draw = EnSlime_Draw;
    this->actor.colChkInfo.health = sColChkInfoInit.health;
    this->iceBlockTimer = ICE_BLOCK_UNUSED;
    Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
    this->timer = 0;
    this->actor.colorFilterTimer = 0;
    this->actor.home.rot.y = this->actor.yawTowardsPlayer;
    Actor_PlaySfx(&this->actor, NA_SE_EN_SLIME_SURFACE);
    this->actor.gravity = -2.0f;
    this->actionFunc = EnSlime_Revive;
}

void EnSlime_Revive(EnSlime* this, PlayState* play) {
    f32 rescaleFactor1;
    f32 rescaleFactor2;

    this->timer++;
    if (this->timer == 28) {
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->collider.base.acFlags |= AC_ON;
        this->actor.shape.rot.y = this->actor.home.rot.y;
        EnSlime_SetupMoveInDirection(this);
    } else {
        if (this->timer < 12) {
            rescaleFactor1 = this->timer * (0.01f / 12.0f);
            this->reviveScale.x = rescaleFactor1;
            this->reviveScale.z = rescaleFactor1;
            this->reviveScale.y = 2.0f * rescaleFactor1;
            this->reviveRotY = this->timer * (0x4000 / 10.0f);
        } else if (this->timer < 20) {
            rescaleFactor1 = (this->timer - 12) * (0.01f / 16.0f);
            rescaleFactor2 = 0.01f + rescaleFactor1;
            this->reviveScale.x = rescaleFactor2;
            this->reviveScale.z = rescaleFactor2;
            this->reviveScale.y = 2.0f * (0.01f - rescaleFactor1);
            this->reviveRotY = this->timer * (0x4000 / 10.0f);
        } else if (this->timer < 24) {
            rescaleFactor1 = (this->timer - 20) * (0.01f / 3.0f);
            rescaleFactor2 = 0.015f - rescaleFactor1;
            this->reviveScale.x = rescaleFactor2;
            this->reviveScale.z = rescaleFactor2;
            this->reviveScale.y = (2.0f * rescaleFactor1) + 0.01f;
            Actor_SetScale(&this->actor, 1.5f * rescaleFactor1);
            this->reviveRotY = ((23 - this->timer) * 7281.778f) + 10922.667f;
            this->actor.shape.rot.y = (28 - this->timer) * (f32)0x1000;
        } else {
            Actor_SetScale(&this->actor, (((28 - this->timer) * 0.1f) + 1.0f) * 0.01f);
            Math_Vec3f_Copy(&this->reviveScale, &gZeroVec3f);
            this->actor.shape.rot.y = (28 - this->timer) * (f32)0x1000;
        }

        if (this->timer == 21) {
            this->collider.base.ocFlags1 |= OC1_ON;
        }

        this->reviveRotY += this->actor.home.rot.y;
        this->actor.shape.rot.y += this->actor.home.rot.y;
    }
}

void EnSlime_UpdateDamage(EnSlime* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        if ((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) ||
            !(this->collider.elem.acHitElem->atDmgInfo.dmgFlags & 0xDB0B3)) {

            EnSlime_Thaw(this, play);
            if ((this->actor.params == EN_SLIME_TYPE_BLUE) &&
                (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_FREEZE)) {
                this->actor.colChkInfo.damage = 0;
            }

            if (Actor_ApplyDamage(&this->actor) == 0) {
                Actor_SetDropFlag(&this->actor, &this->collider.elem);
                Enemy_StartFinishingBlow(play, &this->actor);
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            }

            if (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_BLUNT) {
                EnSlime_SetupReactToBluntHit(this);
            } else if (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_STUN) {
                this->timer = 40;
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_XLU, 40);
                Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                EnSlime_SetupStun(this);
            } else if (this->actor.colChkInfo.damageEffect != EN_SLIME_DMGEFF_HOOKSHOT) {
                if (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_FREEZE) {
                    if (this->actor.params == EN_SLIME_TYPE_BLUE) {
                        EnSlime_SetupSpawnIceBlock(this);
                    } else {
                        EnSlime_Freeze(this);
                        if (this->actor.colChkInfo.health == 0) {
                            this->timer = 3;
                            this->collider.base.acFlags &= ~AC_ON;
                        }
                        EnSlime_SetupStun(this);
                    }
                } else {
                    if (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_LIGHT) {
                        this->drawDmgEffAlpha = 4.0f;
                        this->drawDmgEffScale = 0.4f;
                        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->collider.elem.acDmgInfo.hitPos.x,
                                    this->collider.elem.acDmgInfo.hitPos.y, this->collider.elem.acDmgInfo.hitPos.z, 0,
                                    0, 0, CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                    } else if (this->actor.colChkInfo.damageEffect == EN_SLIME_DMGEFF_ELECTRIC) {
                        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE;
                        this->drawDmgEffAlpha = 4.0f;
                        this->drawDmgEffScale = 0.4f;
                    }
                    EnSlime_SetupDamaged(this, play, true);
                }
            }
        }
    }
}

void EnSlime_Update(Actor* thisx, PlayState* play) {
    EnSlime* this = (EnSlime*)thisx;

    EnSlime_UpdateDamage(this, play);
    this->actionFunc(this, play);

    thisx->shape.shadowAlpha = this->iceBlockTimer;
    if (this->iceBlockTimer == ICE_BLOCK_UNUSED) {
        if (thisx->scale.y > 0.0001f) {
            thisx->targetArrowOffset = 60.0f / thisx->scale.y;
        }

        if (this->collider.base.ocFlags1 & OC1_ON) {
            Actor_MoveWithGravity(thisx);
            Actor_UpdateBgCheckInfo(play, thisx, 20.0f, 35.0f, 40.0f,
                                    UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                        UPDBGCHECKINFO_FLAG_10);
        }

        Collider_UpdateCylinder(thisx, &this->collider);
        this->collider.dim.radius = sCylinderInit.dim.radius * (100.0f * thisx->scale.z);
        this->collider.dim.height = sCylinderInit.dim.height * (100.0f * thisx->scale.y);

        if (this->actionFunc == EnSlime_Jump) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }

        if (this->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }

        if (0) {
            this->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
        } else {
            this->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }

        if (this->collider.base.ocFlags1 & OC1_ON) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }

        Actor_SetFocus(thisx, 15.0f);
        if (this->drawDmgEffAlpha > 0.0f) {
            if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
                Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
                this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.2f;
                this->drawDmgEffScale = CLAMP_MAX(this->drawDmgEffScale, 0.4f);
            } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.4f, 0.01f)) {
                Actor_PlaySfx_Flagged(thisx, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
            }
        }
    }
}

static Color_RGBA8 sPrimColors[EN_SLIME_TYPE_MAX] = {
    { 255, 255, 255, 255 },
    { 255, 255, 0, 255 },
    { 255, 255, 200, 255 },
    { 225, 200, 255, 255 },
};

static Color_RGBA8 sEnvColors[EN_SLIME_TYPE_MAX] = {
    { 140, 255, 195, 255 },
    { 50, 255, 0, 255 },
    { 255, 180, 0, 255 },
    { 255, 50, 155, 255 },
};

static Vec3f sBodyPartPosOffsets[EN_SLIME_BODYPART_MAX] = {
    { 2000.0f, 2000.0f, 0.0f }, { -1500.0f, 2500.0f, -500.0f }, { -500.0f, 1000.0f, 2500.0f },
    { 0.0f, 4000.0f, 0.0f },    { 0.0f, 2000.0f, -2000.0f },
};

void EnSlime_Draw(Actor* thisx, PlayState* play) {
    s32 i;
    EnSlime* this = (EnSlime*)thisx;
    Vec3f wobbleScale;
    Color_RGBA8* primColor;
    Color_RGBA8* envColor;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    func_800B8118(&this->actor, play, 0);
    if (this->iceBlockTimer != ICE_BLOCK_UNUSED) {
        gSPSegment(POLY_XLU_DISP++, 0x0A, sEmptyDisplayList);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 170, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 150, 255, 255, this->iceBlockTimer);
    } else {
        primColor = &sPrimColors[this->actor.params];
        envColor = &sEnvColors[this->actor.params];
        AnimatedMat_Draw(play, sSlimeTexAnim);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 100, primColor->r, primColor->g, primColor->b, primColor->a);
        gDPSetEnvColor(POLY_XLU_DISP++, envColor->r, envColor->g, envColor->b, 255);
    }

    if (this->actionFunc == EnSlime_Damaged) {
        wobbleScale.x = 1.0f - (Math_SinF((f32)this->timer * (M_PIf / 2)) * 0.3f);
        wobbleScale.y = (Math_SinF((f32)this->timer * (M_PIf / 2)) * 0.3f) + 1.0f;
        wobbleScale.z = 1.0f - (Math_CosF((f32)this->timer * (M_PIf / 2)) * 0.3f);

        Matrix_RotateXFApply(this->wobbleRot.x);
        Matrix_RotateZF(this->wobbleRot.z, MTXMODE_APPLY);
        Matrix_Scale(wobbleScale.x, wobbleScale.y, wobbleScale.z, MTXMODE_APPLY);
        Matrix_RotateZF(-this->wobbleRot.z, MTXMODE_APPLY);
        Matrix_RotateXFApply(-this->wobbleRot.x);
    }

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gChuchuBodyDL);

    if (this->iceBlockTimer == ICE_BLOCK_UNUSED) {
        Scene_SetRenderModeXlu(play, 0, 1);

        gSPSegment(POLY_OPA_DISP++, 0x09, sEyeTextures[this->eyeTexIndex]);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 30, 70, 255);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gChuchuEyesDL);

    } else {
        Scene_SetRenderModeXlu(play, 1, 2);
        gSPSegment(POLY_XLU_DISP++, 0x09, sEyeTextures[this->eyeTexIndex]);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gChuchuEyesDL);
    }

    for (i = 0; i < EN_SLIME_BODYPART_MAX; i++) {
        Matrix_MultVec3f(&sBodyPartPosOffsets[i], &this->bodyPartsPos[i]);
    }

    if (this->actionFunc == EnSlime_Revive) {
        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_RotateYS(this->reviveRotY, MTXMODE_APPLY);
        Matrix_Scale(this->reviveScale.x, this->reviveScale.y, this->reviveScale.z, MTXMODE_APPLY);

        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gChuchuPuddleDL);
    }

    if ((this->actor.params != EN_SLIME_TYPE_BLUE) && (this->actor.scale.x > 0.0f)) {
        POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
        POLY_OPA_DISP = Gfx_SetupDL66(POLY_OPA_DISP);

        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + (2000.0f * this->actor.scale.y),
                         this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);

        gSPSegment(POLY_OPA_DISP++, 0x08, this->itemDropTex);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gItemDropDL);
    }

    Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EN_SLIME_BODYPART_MAX, this->drawDmgEffScale,
                            this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha, this->drawDmgEffType);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x14a, En_Slime_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_SLIME, En_Slime_Profile, EnSlime_Init, EnSlime_Destroy, EnSlime_Update,
                     EnSlime_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x14au, 0x16au, 0xe81d2667u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_SLIME_ORIGIN, generatedNativeOrigin);
static const u32 generatedNativeDefeat[] = { __builtin_offsetof(EnSlime, actionFunc), (u32)EnSlime_Damaged,
                                             (u32)EnSlime_WaitForRevive };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_SLIME_DEFEAT, generatedNativeDefeat);
