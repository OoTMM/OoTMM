#include "mm_native_compat.h"
#include <combo/mask.h>
#include <combo/enemy_flight.h>
#define gBadBatSetupDL (*((Gfx (*)[1])0x060000a0))
#define gBadBatBodyDL (*((Gfx (*)[1])0x060000c8))
#define gBadBatWingsFrame0DL (*((Gfx (*)[1])0x060001b0))
#define gBadBatWingsFrame1DL (*((Gfx (*)[1])0x060002a0))
#define gBadBatWingsFrame2DL (*((Gfx (*)[1])0x06000390))
#define gBadBatWingsFrame3DL (*((Gfx (*)[1])0x06000480))
#define gBadBatWingsFrame4DL (*((Gfx (*)[1])0x06000570))
#define gBadBatWingsFrame5DL (*((Gfx (*)[1])0x06000660))
#define gBadBatWingsFrame6DL (*((Gfx (*)[1])0x06000750))
#define gBadBatWingsFrame7DL (*((Gfx (*)[1])0x06000840))
#define gBadBatWingsFrame8DL (*((Gfx (*)[1])0x06000930))

#include "types/en_bat.h"
#define TATL_HINT_ID_BAD_BAT 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_BAT
#define ACTOR_EN_BAT 0x15b
#undef OBJECT_BAT
#define OBJECT_BAT 0x1172
#undef NA_SE_EV_ICE_FREEZE
#define NA_SE_EV_ICE_FREEZE 0x28b2
#undef NA_SE_EN_FFLY_ATTACK
#define NA_SE_EN_FFLY_ATTACK 0x3840
#undef NA_SE_EN_FFLY_FLY
#define NA_SE_EN_FFLY_FLY 0x3841
#undef NA_SE_EN_FFLY_DEAD
#define NA_SE_EN_FFLY_DEAD 0x3842
#undef NA_SE_EN_EXTINCT
#define NA_SE_EN_EXTINCT 0x3878
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef Collider_InitAndSetSphere
#define Collider_InitAndSetSphere ((s32 (*)(struct PlayState* play, ColliderSphere* dest, struct Actor* actor, ColliderSphereInit* src))(void*)FOREIGN_ENEMY_API_SPHERE_INIT_SET)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Flags_GetSwitch
#define Flags_GetSwitch ((s32 (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef Actor_SpawnAsChildAndCutscene
#define Actor_SpawnAsChildAndCutscene ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 index, f32 x, f32 y, f32 z, s16 rotX, s16 rotY, s16 rotZ, s32 params, u32 csId, u32 halfDaysBits, Actor* parent))(void*)FOREIGN_ENEMY_API_SPAWN_CUTSCENE)
#undef Collider_DestroySphere
#define Collider_DestroySphere ((s32 (*)(struct PlayState* play, ColliderSphere* sph))(void*)FOREIGN_ENEMY_API_SPHERE_DESTROY)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
static int EnBat_PlayerProtected(PlayState* play) {
    return GET_PLAYER(play)->mask == MASK_SKULL || Player_GetMask(play) == PLAYER_MASK_STONE;
}

#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef SubS_FindActor
extern u8 FOREIGN_ENEMY_API_FIND_AFTER[];
#define SubS_FindActor ((Actor* (*)(struct PlayState* play, Actor* actorListStart, u8 actorCategory, s16 actorId))(void*)FOREIGN_ENEMY_API_FIND_AFTER)
#undef Flags_SetSwitch
#define Flags_SetSwitch ((void (*)(struct PlayState* play, s32 flag))(void*)FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH)
#undef Actor_SetDropFlag
#define Actor_SetDropFlag ((void (*)(Actor* actor, ColliderElement* elem))(void*)FOREIGN_ENEMY_API_DROP_FLAG)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_MultVecX
#define Matrix_MultVecX ((void (*)(f32 x, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_X)
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
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Math3D_Dist2DSq
#define Math3D_Dist2DSq ((f32 (*)(f32 x0, f32 y0, f32 x1, f32 y1))0x800a5470u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 angle))0x80021a28u)
#undef Actor_WorldPitchTowardPoint
#define Actor_WorldPitchTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x80021488u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Actor_MoveWithoutGravity
#define Actor_MoveWithoutGravity ((void (*)(Actor* actor))0x800212b8u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)

#define FLAGS \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_IGNORE_QUAKE | ACTOR_FLAG_CAN_ATTACH_TO_ARROW)

#define BAD_BAT_FLAP_FRAME 5

void EnBat_Init(Actor* thisx, PlayState* play);
void EnBat_Destroy(Actor* thisx, PlayState* play);
void EnBat_Update(Actor* thisx, PlayState* play);
void EnBat_Draw(Actor* thisx, PlayState* play);

s32 EnBat_IsGraveyardOnSecondDay(PlayState* play);
void EnBat_SetupPerch(EnBat* this);
void EnBat_Perch(EnBat* this, PlayState* play);
void EnBat_SetupFlyIdle(EnBat* this);
void EnBat_FlyIdle(EnBat* this, PlayState* play);
void EnBat_SetupDiveAttack(EnBat* this);
void EnBat_DiveAttack(EnBat* this, PlayState* play);
void EnBat_Die(EnBat* this, PlayState* play);
void EnBat_Stunned(EnBat* this, PlayState* play);

ActorProfile En_Bat_Profile = {
    0xd5b, ACTORCAT_ENEMY, FLAGS, 0x1172, sizeof(EnBat), EnBat_Init, EnBat_Destroy, EnBat_Update, EnBat_Draw,
};

static ColliderSphereInit sSphereInit = {
    {
        COL_MATERIAL_HIT3,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_HARD,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 1, { { 0, 0, 0 }, 15 }, 100 },
};

typedef enum {
    BAD_BAT_DMGEFF_NONE,
    BAD_BAT_DMGEFF_STUN,
    BAD_BAT_DMGEFF_FIRE,
    BAD_BAT_DMGEFF_ICE,
    BAD_BAT_DMGEFF_LIGHT,
    BAD_BAT_DMGEFF_ELECTRIC
} BatDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, BAD_BAT_DMGEFF_STUN), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(2, BAD_BAT_DMGEFF_FIRE),
    DMG_ENTRY(2, BAD_BAT_DMGEFF_ICE),  DMG_ENTRY(2, BAD_BAT_DMGEFF_LIGHT),    DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(2, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(0, BAD_BAT_DMGEFF_STUN), DMG_ENTRY(0, BAD_BAT_DMGEFF_ELECTRIC), DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE),     DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE),
    DMG_ENTRY(0, BAD_BAT_DMGEFF_NONE), DMG_ENTRY(1, BAD_BAT_DMGEFF_NONE),
} };

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 30, 10 };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_BAD_BAT, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_STOP),
};

static Gfx* sWingsDLs[] = {
    gBadBatWingsFrame0DL, gBadBatWingsFrame1DL, gBadBatWingsFrame2DL, gBadBatWingsFrame3DL, gBadBatWingsFrame4DL,
    gBadBatWingsFrame5DL, gBadBatWingsFrame6DL, gBadBatWingsFrame7DL, gBadBatWingsFrame8DL,
};

#define BAD_BAT_MAX_NUMBER_ATTACKING 3

s32 sNumberAttacking;
s32 sAlreadySpawned;

void EnBat_Init(Actor* thisx, PlayState* play) {
    EnBat* this = (EnBat*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    Collider_InitAndSetSphere(play, &this->collider, thisx, &sSphereInit);
    this->collider.dim.worldSphere.radius = sSphereInit.dim.modelSphere.radius;
    CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&thisx->shape, 2000.0f, ActorShadow_DrawCircle, 25.0f);

    this->animationFrame = Rand_ZeroOne() * 9.0f;

    this->paramFlags = BAD_BAT_GET_PARAMFLAGS(thisx);
    this->switchFlag = BAD_BAT_GET_SWITCH_FLAG(thisx);
    thisx->params = BAD_BAT_GET_TYPE(thisx);

    thisx->depthInWater = BGCHECK_Y_MIN;
    Actor_SetFocus(thisx, 20.0f);

    if (sAlreadySpawned) {
        Actor_Kill(thisx);
    } else if (EnBat_IsGraveyardOnSecondDay(play)) {
        if (Flags_GetSwitch(play, this->switchFlag)) {
            Actor_Kill(thisx);
        } else {
            this->actor.room = -1;
        }
    }

    if (this->paramFlags & BAD_BAT_PARAMFLAG_PERCH) {
        thisx->params = 0;
        EnBat_SetupPerch(this);
    } else {
        if (thisx->params == 0x1F) {
            thisx->params = 0;
        }
        EnBat_SetupFlyIdle(this);
        while (BAD_BAT_GET_NUMBER_TO_SPAWN(thisx) > 1) {
            Actor_SpawnAsChildAndCutscene(
                &play->actorCtx, play, ACTOR_EN_BAT, thisx->world.pos.x + Rand_CenteredFloat(200.0f),
                thisx->world.pos.y + Rand_CenteredFloat(100.0f), thisx->world.pos.z + Rand_CenteredFloat(200.0f),
                Rand_CenteredFloat(0x2000), 0xFFFF * Rand_ZeroOne(), 0,
                BAD_BAT_PARAMS(this->switchFlag, this->paramFlags, 0), CS_ID_NONE, comboEnemySourceHalfDays(thisx),
                NULL);
            BAD_BAT_GET_NUMBER_TO_SPAWN(thisx)--;
        }
    }
}

void EnBat_Destroy(Actor* thisx, PlayState* play) {
    EnBat* this = (EnBat*)thisx;

    Collider_DestroySphere(play, &this->collider);
}

s32 EnBat_IsGraveyardOnSecondDay(PlayState* play) {
    if ((gMmSave.day == 2) && (0)) {
        return true;
    } else {
        return false;
    }
}

void EnBat_StepAnimation(EnBat* this, s32 frameStep) {
    s32 prevFrame = this->animationFrame;

    this->animationFrame += frameStep;
    if (this->animationFrame >= ARRAY_COUNT(sWingsDLs)) {
        this->animationFrame -= ARRAY_COUNT(sWingsDLs);
    }
    if ((prevFrame < BAD_BAT_FLAP_FRAME) && (this->animationFrame >= BAD_BAT_FLAP_FRAME)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_FFLY_FLY);
    }
}

void EnBat_SetupPerch(EnBat* this) {
    this->collider.base.acFlags |= AC_ON;
    this->collider.base.atFlags |= AT_ON;
    this->collider.dim.worldSphere.center.x = this->actor.focus.pos.x;
    this->collider.dim.worldSphere.center.y = this->actor.focus.pos.y;
    this->collider.dim.worldSphere.center.z = this->actor.focus.pos.z;
    this->actor.speed = 0.0f;
    this->actionFunc = EnBat_Perch;
}

void EnBat_Perch(EnBat* this, PlayState* play) {
    EnBat_StepAnimation(this, 1);
}

void EnBat_SetupFlyIdle(EnBat* this) {
    this->timer = 100;
    this->collider.base.acFlags |= AC_ON;
    this->actor.speed = 3.5f;
    this->actionFunc = EnBat_FlyIdle;
}

void EnBat_FlyIdle(EnBat* this, PlayState* play) {
    s32 finishedRotStep;

    EnBat_StepAnimation(this, 1);

    finishedRotStep = Math_ScaledStepToS(&this->actor.shape.rot.y, this->yawTarget, 0x300);

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
        this->yawTarget = this->actor.wallYaw;
    } else if (Math3D_Dist2DSq(this->actor.world.pos.x, this->actor.world.pos.z, this->actor.home.pos.x,
                               this->actor.home.pos.z) > SQ(300.0f)) {
        this->yawTarget = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    } else if (finishedRotStep && (Rand_ZeroOne() < 0.015f)) {
        this->yawTarget =
            this->actor.shape.rot.y + (((s32)(0x1000 * Rand_ZeroOne()) + 0x1000) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1));
    }

    finishedRotStep = Math_ScaledStepToS(&this->actor.shape.rot.x, this->pitchTarget, 0x100);

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.depthInWater > -40.0f)) {
        this->pitchTarget = -0x1000;
    } else if (this->actor.world.pos.y < (this->actor.home.pos.y - 100.0f)) {
        this->pitchTarget = -((s32)(0x800 * Rand_ZeroOne()) + 0x800);
    } else if ((this->actor.home.pos.y + 100.0f) < this->actor.world.pos.y) {
        this->pitchTarget = (s32)(0x800 * Rand_ZeroOne()) + 0x800;
    } else if ((finishedRotStep) && (Rand_ZeroOne() < 0.015f)) {
        this->pitchTarget += (s16)(((s32)(0x400 * Rand_ZeroOne()) + 0x400) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1));
        this->pitchTarget = CLAMP(this->pitchTarget, -0x1000, 0x1000);
    }

    if (this->timer != 0) {
        this->timer--;
    }
    if ((this->actor.xzDistToPlayer < 300.0f) && (this->timer == 0) && !EnBat_PlayerProtected(play) &&
        (sNumberAttacking < BAD_BAT_MAX_NUMBER_ATTACKING) &&
        (!(this->paramFlags & BAD_BAT_PARAMFLAG_CHECK_HEIGHTREL) || (fabsf(this->actor.yDistanceFromLink) < 150.0f))) {
        EnBat_SetupDiveAttack(this);
    }
}

void EnBat_SetupDiveAttack(EnBat* this) {
    this->collider.base.atFlags |= AT_ON;
    this->timer = 300;
    this->actor.speed = 4.0f;
    sNumberAttacking++;
    this->actionFunc = EnBat_DiveAttack;
}

void EnBat_DiveAttack(EnBat* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 isFacingPlayer;
    Vec3f preyPos;

    EnBat_StepAnimation(this, 2);
    isFacingPlayer = Actor_IsFacingPlayer(&this->actor, 0x2800);

    if (isFacingPlayer) {
        s16 pitchTarget;

        preyPos.x = player->actor.world.pos.x;
        preyPos.y = player->actor.world.pos.y + 20.0f;
        preyPos.z = player->actor.world.pos.z;

        pitchTarget = Actor_WorldPitchTowardPoint(&this->actor, &preyPos);
        pitchTarget = CLAMP(pitchTarget, -0x3000, 0x3000);
        Math_SmoothStepToS(&this->actor.shape.rot.x, pitchTarget, 2, 0x400, 0x40);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.x, -0x800, 2, 0x100, 0x10);
    }
    if (isFacingPlayer || (this->actor.xzDistToPlayer > 80.0f)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 4, 0xC00, 0xC0);
    }

    this->timer--;

    if ((this->timer == 0) || (this->collider.base.atFlags & AT_HIT) || EnBat_PlayerProtected(play) ||
        (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (player->stateFlags1 & PLAYER_STATE1_800000) ||
        (this->actor.depthInWater > -40.0f)) {
        if (this->collider.base.atFlags & AT_HIT) {
            this->collider.base.atFlags &= ~AT_HIT;
            Actor_PlaySfx(&this->actor, NA_SE_EN_FFLY_ATTACK);
        }
        this->collider.base.atFlags &= ~AT_ON;
        sNumberAttacking--;
        EnBat_SetupFlyIdle(this);
    } else if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
               (ABS_ALT(BINANG_SUB(this->actor.wallYaw, this->actor.yawTowardsPlayer)) > 0x6800)) {
        sNumberAttacking--;
        this->collider.base.atFlags &= ~AT_ON;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
        this->yawTarget = this->actor.wallYaw;
        EnBat_SetupFlyIdle(this);
    }
}

void EnBat_SetupDie(EnBat* this, PlayState* play) {
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Enemy_StartFinishingBlow(play, &this->actor);
    this->actor.speed *= Math_CosS(this->actor.world.rot.x);
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actor.velocity.y = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_FFLY_DEAD);

    if (this->actor.colChkInfo.damageEffect == BAD_BAT_DMGEFF_ICE) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
        this->drawDmgEffAlpha = 1.0f;
        this->drawDmgEffFrozenSteamScale = 60.0f * (0.45f / 40.0f);
        this->drawDmgEffScale = 0.45f;
    } else if (this->actor.colChkInfo.damageEffect == BAD_BAT_DMGEFF_LIGHT) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
        this->drawDmgEffAlpha = 4.0f;
        this->drawDmgEffScale = 0.45f;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->collider.elem.acDmgInfo.hitPos.x,
                    this->collider.elem.acDmgInfo.hitPos.y, this->collider.elem.acDmgInfo.hitPos.z, 0, 0, 0,
                    CLEAR_TAG_PARAMS(CLEAR_TAG_SMALL_LIGHT_RAYS));
    } else if (this->actor.colChkInfo.damageEffect == BAD_BAT_DMGEFF_FIRE) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->drawDmgEffAlpha = 4.0f;
        this->drawDmgEffScale = 0.45f;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 40);

    if (this->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW) {
        this->actor.speed = 0.0f;
    }

    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actionFunc = EnBat_Die;
}

void EnBat_Die(EnBat* this, PlayState* play) {
    Math_StepToF(&this->actor.speed, 0.0f, 0.5f);
    this->actor.colorFilterTimer = 40;

    if (!(this->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW)) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_ScaledStepToS(&this->actor.shape.rot.x, 0x4000, 0x200);
            this->actor.shape.rot.z += 0x1780;
        }

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.floorHeight == BGCHECK_Y_MIN)) {
            if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
                Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, BAD_BAT_BODYPART_MAX, 2, 0.2f, 0.2f);
            }

            func_800B3030(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f, 100, 0, 0);
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 11, NA_SE_EN_EXTINCT);
            Actor_Kill(&this->actor);

            if (0 == -1) {
                Actor* enemy = NULL;

                do {
                    enemy = SubS_FindActor(play, enemy, ACTORCAT_ENEMY, ACTOR_EN_BAT);
                    if (enemy != NULL) {
                        if (enemy != &this->actor) {
                            break;
                        }
                        enemy = enemy->next;
                    }
                } while (enemy != NULL);

                if (enemy == NULL) {
                    Flags_SetSwitch(play, this->switchFlag);
                }
            }
        }
    }
}

void EnBat_SetupStunned(EnBat* this) {
    if (this->actionFunc != EnBat_Stunned) {
        this->actor.shape.yOffset = 700.0f;
        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
        this->actor.world.pos.y += 13.0f;
    }

    Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, this->timer);
    this->actionFunc = EnBat_Stunned;
}

void EnBat_Stunned(EnBat* this, PlayState* play) {
    Math_ScaledStepToS(&this->actor.shape.rot.x, 0, 0x100);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.floorHeight == BGCHECK_Y_MIN)) {
        if (this->timer != 0) {
            this->timer--;
        }
        if (this->timer == 0) {
            EnBat_SetupFlyIdle(this);
        }
    } else {
        this->actor.colorFilterTimer = 40;
        if (this->drawDmgEffAlpha > 0.0f) {
            this->drawDmgEffAlpha = 2.0f;
        }
    }
}

void EnBat_UpdateDamage(EnBat* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        Actor_SetDropFlag(&this->actor, &this->collider.elem);
        this->collider.base.atFlags &= ~AT_ON;

        if (this->actionFunc == EnBat_DiveAttack) {
            sNumberAttacking--;
        }

        if (this->actor.colChkInfo.damageEffect == BAD_BAT_DMGEFF_ELECTRIC) {
            this->timer = 40;
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE;
            this->drawDmgEffAlpha = 2.0f;
            this->drawDmgEffScale = 0.45f;
            EnBat_SetupStunned(this);
        } else if (this->actor.colChkInfo.damageEffect == BAD_BAT_DMGEFF_STUN) {
            this->timer = 40;
            EnBat_SetupStunned(this);
        } else {
            Actor_ApplyDamage(&this->actor);
            EnBat_SetupDie(this, play);
        }
    }
}

void EnBat_Update(Actor* thisx, PlayState* play) {
    EnBat* this = (EnBat*)thisx;

    if (0 == -1) {
        sAlreadySpawned = true;
    }

    EnBat_UpdateDamage(this, play);
    this->actionFunc(this, play);

    if (this->actionFunc != EnBat_Stunned) {
        Math_StepToF(&this->actor.shape.yOffset, 2000.0f, 200.0f);
    }

    if (this->actionFunc != EnBat_Perch) {
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->actor.world.rot.x = -this->actor.shape.rot.x;

        if ((this->actor.colChkInfo.health != 0) && (this->actionFunc != EnBat_Stunned)) {
            Actor_MoveWithoutGravity(&this->actor);
        } else {
            Actor_MoveWithGravity(&this->actor);
        }

        Vec3f previous = this->actor.prevPos;
        Actor_UpdateBgCheckInfo(play, &this->actor, 12.0f, 15.0f, 50.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4);
        if ((this->actor.colChkInfo.health != 0) && (this->actionFunc != EnBat_Stunned)) {
            enemyFlightCheckWall(play, &this->actor, previous);
        }

        Actor_SetFocus(&this->actor, this->actor.shape.yOffset * 0.01f);
        this->collider.dim.worldSphere.center.x = this->actor.focus.pos.x;
        this->collider.dim.worldSphere.center.y = this->actor.focus.pos.y;
        this->collider.dim.worldSphere.center.z = this->actor.focus.pos.z;
    }

    if (this->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->actionFunc == EnBat_DiveAttack) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->drawDmgEffAlpha > 0.0f) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
            this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.225f;
            this->drawDmgEffScale = CLAMP_MAX(this->drawDmgEffScale, 0.45f);
        } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.45f, 0.45f / 40.0f)) {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        }
    }
}

void EnBat_Draw(Actor* thisx, PlayState* play) {
    EnBat* this = (EnBat*)thisx;
    Gfx* gfx;

    if (this->actor.projectedPos.z > 0.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        gfx = POLY_OPA_DISP;

        Gfx_SetupDL(&gfx[0], 25);
        MATRIX_FINALIZE_AND_LOAD(&gfx[1], play->state.gfxCtx);
        gSPDisplayList(&gfx[2], gBadBatSetupDL);
        gSPDisplayList(&gfx[3], gBadBatBodyDL);
        gSPDisplayList(&gfx[4], sWingsDLs[this->animationFrame]);

        POLY_OPA_DISP = &gfx[5];

        CLOSE_DISPS();
    }

    if (this->drawDmgEffAlpha > 0.0f) {
        s16 rollAngle;

        if (this->animationFrame < BAD_BAT_FLAP_FRAME - 1) {
            rollAngle = this->animationFrame * (15 * (0x10000 / 360));
        } else {
            rollAngle = (this->animationFrame >= BAD_BAT_FLAP_FRAME)
                            ? (this->animationFrame * (15 * (0x10000 / 360))) - (120 * (0x10000 / 360))
                            : 0;
        }
        Matrix_MultZero(&this->bodyPartsPos[BAD_BAT_BODYPART_0]);
        Matrix_RotateZS(rollAngle, MTXMODE_APPLY);
        Matrix_MultVecX(1700.0f, &this->bodyPartsPos[BAD_BAT_BODYPART_1]);
        Matrix_RotateZS(-2 * rollAngle, MTXMODE_APPLY);
        Matrix_MultVecX(-1700.0f, &this->bodyPartsPos[BAD_BAT_BODYPART_2]);
        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, BAD_BAT_BODYPART_MAX, this->drawDmgEffScale,
                                this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha, this->drawDmgEffType);
    }
}

OVL_INFO_NATIVE_ENEMY(0x15b, En_Bat_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_BAT, En_Bat_Profile, EnBat_Init, EnBat_Destroy, EnBat_Update, EnBat_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x15bu, 0x172u, 0x3629d509u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAT_ORIGIN, generatedNativeOrigin);
_Static_assert(__builtin_offsetof(EnBat, paramFlags) == 0x141, "Bad Bat ordinary param flags");
_Static_assert(__builtin_offsetof(EnBat, switchFlag) == 0x142, "Bad Bat switch ownership");
