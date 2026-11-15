#include "lizalfos.h"
#include <combo/enemy_port.h>
#include <combo/mm/actor_arms_hook.h>

#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389e
#undef NA_SE_EN_RIZA_ATTACK
#define NA_SE_EN_RIZA_ATTACK 0x382a
#undef NA_SE_EN_RIZA_CRY
#define NA_SE_EN_RIZA_CRY 0x3829
#undef NA_SE_EN_RIZA_DAMAGE
#define NA_SE_EN_RIZA_DAMAGE 0x382b
#undef NA_SE_EN_RIZA_DEAD
#define NA_SE_EN_RIZA_DEAD 0x382d
#undef NA_SE_EN_RIZA_DOWN
#define NA_SE_EN_RIZA_DOWN 0x387a
#undef NA_SE_EN_RIZA_JUMP
#define NA_SE_EN_RIZA_JUMP 0x382f
#define OOT_RIZA_LAND_SFX 0x3828
#undef NA_SE_EN_RIZA_WALK
#define NA_SE_EN_RIZA_WALK 0x382e

#define ActorShadow_DrawFeet ((void*)0x800b42f8)
#define Effect_Delete ((void (*)(PlayState*,s32))0x800afdcc)
#define EffectBlure_AddSpace ((void (*)(void*))0x800a8514)
#define Actor_OtherIsLockedOn ((s32 (*)(PlayState*,Actor*))0x800bc5ec)
#define Actor_SetFeetPos ((void (*)(Actor*,s32,s32,Vec3f*,s32,Vec3f*))0x800b4a98)
#define EnemyLizalfosSkelDraw ((Gfx* (*)(PlayState*,void**,Vec3s*,void*,void*,void*,Gfx*))0x80134990)
#define UPDBGCHECKINFO_FLAG_0 UPDBGCHECKINFO_FLAG_MM_1
#define UPDBGCHECKINFO_FLAG_1 UPDBGCHECKINFO_FLAG_MM_2
#define UPDBGCHECKINFO_FLAG_2 UPDBGCHECKINFO_FLAG_MM_4
#define UPDBGCHECKINFO_FLAG_3 UPDBGCHECKINFO_FLAG_MM_8
#define UPDBGCHECKINFO_FLAG_4 UPDBGCHECKINFO_FLAG_MM_10
#define COLLECTIBLE_DROP_RANDOM_PARAMS(table,flag) ((table)<<4)
#define COLLECTIBLE_DROP_TABLE_4 4

#define gZfWalkAnim (*(AnimationHeader*)0x06008138)
#define gZfLandOnFeetAnim (*(AnimationHeader*)0x06008c6c)
#define gZfJumpAnim (*(AnimationHeader*)0x06009530)
#define gZfSlashAnim (*(AnimationHeader*)0x0600a3d4)
#define gZfIdleAnim (*(AnimationHeader*)0x0600b10c)
#define gLizalfosRightHandDL ((Gfx*)0x0600e198)
#define gLizalfosSheathedSwordDL ((Gfx*)0x06010060)
#define gLizalfosSkel (*(SkeletonHeader*)0x060104b8)
#define gZfDieAnim (*(AnimationHeader*)0x06014e60)
#define gZfDamagedAnim (*(AnimationHeader*)0x060157f8)
#define gZfWalkSidewaysAnim (*(AnimationHeader*)0x06016388)

typedef enum EnZfAction {
    ENZF_ACTION_WAIT_APPEAR,
    ENZF_ACTION_IDLE = 3,
    ENZF_ACTION_MAIN_WALK = 5,
    ENZF_ACTION_TURN_TOWARDS_PLAYER,
    ENZF_ACTION_SIDESTEP,
    ENZF_ACTION_ATTACK_BOUNCED,
    ENZF_ACTION_ATTACK,
    ENZF_ACTION_CIRCLE_PLAYER,
    ENZF_ACTION_JUMP_FORWARDS,
    ENZF_ACTION_STUNNED,
    ENZF_ACTION_DIE,
    ENZF_ACTION_DAMAGED,
    ENZF_ACTION_PAIRED_JUMP_AWAY_START_TURN_AROUND = 18,
    ENZF_ACTION_PAIRED_JUMP_AWAY,
    ENZF_ACTION_PAIRED_IDLE_HOP,
    ENZF_ACTION_PAIRED_ENGAGE_COMBAT,
    ENZF_ACTION_JUMP_ATTACK
} EnZfAction;

static ColliderCylinderInit sBodyCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK1,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sSwordColliderQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x08 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum EnZfDamageReaction {
    ENZF_DMG_REACT_NONE,
    ENZF_DMG_REACT_STUN,
    ENZF_DMG_REACT_6 = 6,
    ENZF_DMG_REACT_FLEE = 13,
    ENZF_DMG_REACT_ICE = 15
} EnZfDamageReaction;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, ENZF_DMG_REACT_STUN), DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(0, ENZF_DMG_REACT_6),
    DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(0, ENZF_DMG_REACT_STUN), DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(0, ENZF_DMG_REACT_STUN), DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
    DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(2, ENZF_DMG_REACT_FLEE),
    DMG_ENTRY(4, ENZF_DMG_REACT_ICE),  DMG_ENTRY(2, ENZF_DMG_REACT_FLEE), DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
    DMG_ENTRY(0, ENZF_DMG_REACT_STUN), DMG_ENTRY(1, ENZF_DMG_REACT_FLEE), DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(0, ENZF_DMG_REACT_STUN), DMG_ENTRY(0, ENZF_DMG_REACT_6),
    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(1, ENZF_DMG_REACT_NONE), DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
    DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(2, ENZF_DMG_REACT_NONE), DMG_ENTRY(0, ENZF_DMG_REACT_6),
    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(0, ENZF_DMG_REACT_6),
    DMG_ENTRY(0, ENZF_DMG_REACT_6),    DMG_ENTRY(2, ENZF_DMG_REACT_NONE),
} };

static Vec3f D_80B4A298 = { 300.0f, 0.0f, 0.0f };
static Vec3f D_80B4A2A4 = { 300.0f, -1700.0f, 0.0f };
static Vec3f D_80B4A2B0 = { -600.0f, 300.0f, 0.0f };
static Vec3f D_80B4A2BC = { 0.0f, 1500.0f, 0.0f };
static Vec3f D_80B4A2C8 = { -600.0f, -3000.0f, 1000.0f };
static Vec3f D_80B4A2D4 = { -600.0f, -3000.0f, -1000.0f };
static Vec3f D_80B4A2E0 = { 1500.0f, -3000.0f, 0.0f };
static Vec3f D_80B4A2EC = { 0.0f, 0.0f, 0.0f };
static Gfx D_80B4A2F8[] = {
    gsSPTexture(0xA00, 0xA00, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

static void enemyLizBlurSpace(void* blur) {
    if (blur)
        EffectBlure_AddSpace(blur);
}

static void enemyLizBlurVertex(void* blur, Vec3f* a, Vec3f* b) {
    if (blur)
        EffectBlure_AddVertex(blur, a, b);
}

static Gfx enemyLizalfosOpaque[] = { gsSPEndDisplayList() };
static Gfx enemyLizalfosTranslucent[] = {
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2 | Z_UPD),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPEndDisplayList(),
};
static void EnZf_SetupAction(EnZf* this, EnZfActionFunc actionFunc);
static s32 EnZf_TestNoFloorAhead(EnZf* this, PlayState* play, f32 distance);
static s16 EnZf_TestNoFloorAheadShape(EnZf* this, PlayState* play, f32 distance);
static void EnZf_PickAction(EnZf* this, PlayState* play);
static void EnZf_SetupIdle(EnZf* this);
static void EnZf_Idle(EnZf* this, PlayState* play);
static void EnZf_SetupMainWalk(EnZf* this, PlayState* play);
static void EnZf_MainWalk(EnZf* this, PlayState* play);
static void EnZf_SetupJumpForwards(EnZf* this);
static void EnZf_JumpForwards(EnZf* this, PlayState* play);
static void EnZf_SetupTurnTowardsPlayer(EnZf* this);
static void EnZf_TurnTowardsPlayer(EnZf* this, PlayState* play);
static void EnZf_SetupSidestepIfFloor(EnZf* this, PlayState* play);
static void EnZf_Sidestep(EnZf* this, PlayState* play);
static void EnZf_SetupAttack(EnZf* this);
static void EnZf_Attack(EnZf* this, PlayState* play);
static void EnZf_SetupAttackBounced(EnZf* this);
static void EnZf_AttackBounced(EnZf* this, PlayState* play);
static void EnZf_SetupStunned(EnZf* this);
static void EnZf_Stunned(EnZf* this, PlayState* play);
static void EnZf_SetupDamaged(EnZf* this);
static void EnZf_Damaged(EnZf* this, PlayState* play);
static void EnZf_SetupJumpAttack(EnZf* this);
static void EnZf_JumpAttack(EnZf* this, PlayState* play);
static void EnZf_SetupCirclePlayerIfFloor(EnZf* this, PlayState* play);
static void EnZf_CirclePlayer(EnZf* this, PlayState* play);
static void EnZf_SetupDie(EnZf* this);
static void EnZf_Die(EnZf* this, PlayState* play);
static void EnZf_UpdateHeadRot(EnZf* this, PlayState* play);
static void EnZf_CheckCollide(EnZf* this, PlayState* play);
static s32 EnZf_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                                 Gfx** gfx);
static void EnZf_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx);
static void EnZf_SetupCirclePlayer(EnZf* this, f32 speed);
static s32 EnZf_ReactToProjectile(PlayState* play, EnZf* this);
static void EnZf_SetupIdle(EnZf* this);
static void EnZf_Draw(Actor*, PlayState*);

static void EnZf_WaitAppear(EnZf* this, PlayState* play) {
    Actor* actor = &this->actor;
    if (this->timer) {
        actor->velocity.y = 0.f;
        if (actor->xzDistToPlayer > 160.f)
            return;
        this->timer = 0;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        Actor_PlaySfx(actor, NA_SE_EN_RIZA_CRY);
    } else if (this->alpha < 255) {
        this->alpha += 51;
    }
    if (this->actionState && (actor->bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH))) {
        this->actionState = 0;
        actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
        actor->world.pos.y = actor->floorHeight;
        actor->velocity.y = 0.f;
        Animation_Change(&this->skelAnime, &gZfLandOnFeetAnim, 1.f, 0.f, 17.f, ANIMMODE_ONCE, 0.f);
        Actor_PlaySfx(actor, OOT_RIZA_LAND_SFX);
        Actor_SpawnFloorDustRing(play, actor, &this->leftFootPos, 3.f, 2, 2.f, 0, 0, 0);
        Actor_SpawnFloorDustRing(play, actor, &this->rightFootPos, 3.f, 2, 2.f, 0, 0, 0);
    }
    actor->shape.shadowAlpha = this->alpha;
    if (SkelAnime_Update(&this->skelAnime)) {
        this->alpha = actor->shape.shadowAlpha = 255;
        EnZf_SetupIdle(this);
    }
}

static void EnZf_Init(Actor* thisx, PlayState* play) {
    EnZf* this = (EnZf*)thisx;
    EffectBlureInit1 swordBlure = { 0 };
    if ((u16)thisx->params != 0xff80) {
        g.roomEnemyLackSoul = 1;
        Actor_Kill(thisx);
        return;
    }
    thisx->attentionRangeType = 3;
    thisx->targetArrowOffset = 2000;
    thisx->gravity = -3.5f;
    Actor_SetScale(thisx, 0.015f);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);
    this->unk_3E0 = 0;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    swordBlure.p1StartColor[0] = swordBlure.p1StartColor[1] = swordBlure.p1StartColor[2] = swordBlure.p1StartColor[3] =
        swordBlure.p2StartColor[0] = swordBlure.p2StartColor[1] = swordBlure.p2StartColor[2] =
            swordBlure.p1EndColor[0] = swordBlure.p1EndColor[1] = swordBlure.p1EndColor[2] = swordBlure.p2EndColor[0] =
                swordBlure.p2EndColor[1] = swordBlure.p2EndColor[2] = 255;
    swordBlure.p2StartColor[3] = 64;
    swordBlure.p1EndColor[3] = 0;
    swordBlure.p2EndColor[3] = 0;
    swordBlure.elemDuration = 8;
    swordBlure.unkFlag = 0;
    swordBlure.calcMode = 2;
    Effect_Add(play, &this->swordBlureEffectIndex, EFFECT_BLURE1, 0, 0, &swordBlure);
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 45.0f, 45.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    this->alpha = 255;
    this->actor.colChkInfo.cylRadius = 40;
    this->actor.colChkInfo.cylHeight = 100;
    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, &this->actor, &sBodyCylinderInit);
    Collider_InitQuad(play, &this->swordCollider);
    Collider_SetQuad(play, &this->swordCollider, &this->actor, &sSwordColliderQuadInit);

    thisx->colChkInfo.health = 6;
    thisx->hintId = 0xff;
    SkelAnime_Init(play, &this->skelAnime, &gLizalfosSkel, &gZfIdleAnim, this->jointTable, this->morphTable,
                   ZF_LIMB_MAX);
    this->homePlatform = this->curPlatform = -1;
    this->actionState = 1;
    Vec3f bottom = thisx->world.pos;
    Vec3f top = bottom;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    bottom.y += 5.f;
    top.y = bottom.y + 360.f;
    if (BgCheck_EntityLineTest1(&play->colCtx, &bottom, &top, &hit, &poly, 1, 1, 1, 0, &bgId))
        top = hit;
    f32 height = top.y - 65.f;
    if ((thisx->bgCheckFlags & BGCHECKFLAG_GROUND) && height > thisx->world.pos.y + 45.f) {
        thisx->world.pos.y = height;
        this->leftFootPos = this->rightFootPos = thisx->world.pos;
        thisx->prevPos = thisx->world.pos;
        thisx->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
        thisx->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
        this->alpha = thisx->shape.shadowAlpha = 0;
        this->timer = 10;
        this->action = ENZF_ACTION_WAIT_APPEAR;
        this->actionFunc = EnZf_WaitAppear;
        Animation_Change(&this->skelAnime, &gZfJumpAnim, 0.f, 9.f, Animation_GetLastFrame(&gZfJumpAnim), ANIMMODE_LOOP,
                         0.f);
    } else {
        EnZf_SetupIdle(this);
    }
}

static void EnZf_Destroy(Actor* actor, PlayState* play) {
    EnZf* this = (EnZf*)actor;
    if ((u16)actor->params != 0xff80)
        return;
    Effect_Delete(play, this->swordBlureEffectIndex);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->swordCollider);
}

static s32 EnZf_CanAttack(PlayState* play, EnZf* this) {
    return !Actor_OtherIsLockedOn(play, &this->actor);
}

static void EnZf_SetupAction(EnZf* this, EnZfActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

static s32 EnZf_TestFloorHeading(EnZf* this, PlayState* play, f32 distance, s16 yaw) {
    Actor* actor = &this->actor;
    Vec3f probe = {
        actor->world.pos.x + Math_SinS(yaw) * distance,
        actor->prevPos.y + 50.f,
        actor->world.pos.z + Math_CosS(yaw) * distance,
    };
    CollisionPoly* floorPoly;
    s32 bgId;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &floorPoly, &bgId, actor, &probe);
    f32 tolerance = (actor->bgCheckFlags & BGCHECKFLAG_GROUND) && actor->velocity.y < 0.f ? 11.f : 0.f;

    return floor > BGCHECK_Y_MIN && actor->velocity.y <= 0.f && floor >= actor->world.pos.y - tolerance;
}

static s32 EnZf_TestNoFloorAhead(EnZf* this, PlayState* play, f32 distance) {
    if (distance == 0.f) {
        distance = this->actor.speed >= 0.f ? 30.f : -30.f;
    }
    return !EnZf_TestFloorHeading(this, play, distance, this->actor.world.rot.y);
}

static s16 EnZf_TestNoFloorAheadShape(EnZf* this, PlayState* play, f32 distance) {
    if (this->actor.speed != 0.f && EnZf_TestNoFloorAhead(this, play, this->actor.speed)) {
        return true;
    }
    return !EnZf_TestFloorHeading(this, play, distance, this->actor.shape.rot.y);
}

static void EnZf_PickAction(EnZf* this, PlayState* play) {
    s16 absRelYawTowardsPlayer;

    absRelYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    absRelYawTowardsPlayer = ABS(absRelYawTowardsPlayer);
    if (absRelYawTowardsPlayer >= 0x1B58) {
        EnZf_SetupCirclePlayerIfFloor(this, play);
    } else if ((this->actor.xzDistToPlayer <= 100.0f) && ((play->gameplayFrames % 8) != 0) &&
               EnZf_CanAttack(play, this)) {
        EnZf_SetupAttack(this);
    } else {
        EnZf_SetupIdle(this);
    }
}

static void EnZf_SetupIdle(EnZf* this) {
    Animation_Change(&this->skelAnime, &gZfIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gZfIdleAnim),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    this->action = ENZF_ACTION_IDLE;
    this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    this->actor.speed = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnZf_SetupAction(this, EnZf_Idle);
}

static void EnZf_Idle(EnZf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    s16 absYawTowardsPlayerRelToHead;
    s16 absYawDiffWithPlayer;

    absYawTowardsPlayerRelToHead = this->actor.yawTowardsPlayer - this->headRot - this->actor.shape.rot.y;
    absYawTowardsPlayerRelToHead = ABS(absYawTowardsPlayerRelToHead);
    SkelAnime_Update(&this->skelAnime);
    if (!EnZf_ReactToProjectile(play, this)) {

        absYawDiffWithPlayer = player->actor.shape.rot.y - this->actor.shape.rot.y;
        absYawDiffWithPlayer = ABS(absYawDiffWithPlayer);
        if ((this->actor.xzDistToPlayer < 100.0f) && (player->meleeWeaponState != 0) &&
            (absYawDiffWithPlayer >= 0x1F40)) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            if (absYawDiffWithPlayer) {
            }
            EnZf_SetupCirclePlayerIfFloor(this, play);
        } else if (this->timer != 0) {
            this->timer--;
        } else {
            if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                if ((this->actor.xzDistToPlayer < 200.0f) && (this->actor.xzDistToPlayer > 100.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    {
                        EnZf_SetupCirclePlayerIfFloor(this, play);
                    }
                } else if (Rand_ZeroOne() > 0.3f) {
                    EnZf_SetupMainWalk(this, play);
                } else {
                    EnZf_SetupCirclePlayerIfFloor(this, play);
                }
            } else {
                EnZf_SetupTurnTowardsPlayer(this);
            }
            if ((play->gameplayFrames & 0x5F) == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
            }
        }
    }
}

static void EnZf_SetupMainWalk(EnZf* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &gZfWalkAnim, -4.0f);
    this->action = ENZF_ACTION_MAIN_WALK;

    this->actor.speed = 0.0f;
    EnZf_SetupAction(this, EnZf_MainWalk);
}

static void EnZf_MainWalk(EnZf* this, PlayState* play) {
    s32 prevFrame;
    s32 beforeCurFrame;

    s16 absYawDiff;
    f32 extraDistFromPlayer = 0.0f;
    Player* player = GET_PLAYER(play);
    f32 absPlaySpeed;

    if (!EnZf_ReactToProjectile(play, this)) {

        if (Actor_OtherIsLockedOn(play, &this->actor)) {
            extraDistFromPlayer = 100.0f;
        }
        if (this->actor.xzDistToPlayer <= (70.0f + extraDistFromPlayer)) {
            Math_SmoothStepToF(&this->actor.speed, -8.0f, 1.0f, 0.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->actor.speed, 8.0f, 1.0f, 0.5f, 0.0f);
        }
        this->skelAnime.playSpeed = this->actor.speed * 1.2f;
        absYawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;
        if (absYawDiff < 0) {
            absYawDiff *= -1;
        }
        if ((1) && (this->actor.xzDistToPlayer < 150.0f) && (player->meleeWeaponState != 0) && (absYawDiff >= 0x1F40)) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnZf_SetupCirclePlayerIfFloor(this, play);
                return;
            }
        }
        prevFrame = (s32)this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
        absPlaySpeed = ((void)0, ABS(this->skelAnime.playSpeed));
        {
            if (!Actor_IsFacingPlayer(&this->actor, 0x11C7)) {
                if (Rand_ZeroOne() > 0.5f) {
                    EnZf_SetupSidestepIfFloor(this, play);
                } else {
                    EnZf_SetupIdle(this);
                }
            } else if (this->actor.xzDistToPlayer < 100.0f) {
                if ((Rand_ZeroOne() > 0.05f) && EnZf_CanAttack(play, this)) {
                    EnZf_SetupAttack(this);
                } else if (Rand_ZeroOne() > 0.5f) {
                    EnZf_SetupCirclePlayerIfFloor(this, play);
                } else {
                    EnZf_SetupIdle(this);
                }
            } else if (this->noFloorAhead) {
                EnZf_SetupSidestepIfFloor(this, play);
            } else if (Rand_ZeroOne() < 0.1f) {
                EnZf_SetupIdle(this);
            }
        }

        if ((play->gameplayFrames & 0x5F) == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
        }
        if ((prevFrame != (s32)this->skelAnime.curFrame) &&
            (((beforeCurFrame < 2) && (((s32)absPlaySpeed + prevFrame) >= 4)) ||
             ((beforeCurFrame < 32) && (((s32)absPlaySpeed + prevFrame) >= 34)))) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_WALK);
        }
    }
}

static void EnZf_SetupJumpForwards(EnZf* this) {
    Animation_Change(&this->skelAnime, &gZfJumpAnim, 1.0f, 0.0f, 3.0f, ANIMMODE_ONCE, -3.0f);
    this->timer = 0;
    this->actionState = 1;
    this->actor.velocity.y = 15.0f;
    { this->actor.speed = 10.0f; }
    this->action = ENZF_ACTION_JUMP_FORWARDS;
    Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_JUMP);
    EnZf_SetupAction(this, EnZf_JumpForwards);
}

static void EnZf_JumpForwards(EnZf* this, PlayState* play) {
    if (this->timer != 0) {
        if (this->actor.world.pos.y <= this->actor.floorHeight) {
            this->actor.world.pos.y = this->actor.floorHeight;
            this->actionState = 0;
            this->actor.velocity.y = 0.0f;
            this->actor.speed = 0.0f;
        }
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer == 0) {
            Animation_Change(&this->skelAnime, &gZfLandOnFeetAnim, 3.0f, 0.0f, 17.0f, ANIMMODE_ONCE, -3.0f);
            this->timer = 10;
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_JUMP);
        } else {
            this->actor.speed = 0.0f;
            this->actionState = 0;
            EnZf_SetupMainWalk(this, play);
        }
    }
    if ((play->gameplayFrames & 0x5F) == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
    }
}

static void EnZf_SetupTurnTowardsPlayer(EnZf* this) {
    Animation_MorphToLoop(&this->skelAnime, &gZfWalkAnim, -4.0f);
    this->action = ENZF_ACTION_TURN_TOWARDS_PLAYER;
    EnZf_SetupAction(this, EnZf_TurnTowardsPlayer);
}

static void EnZf_TurnTowardsPlayer(EnZf* this, PlayState* play) {

    s16 relYawTowardsPlayer;
    s16 yawSpeed;
    f32 animPlaySpeed;

    if (!EnZf_ReactToProjectile(play, this) && !((0) && 0)) {
        relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        if (relYawTowardsPlayer > 0) {
            yawSpeed = (relYawTowardsPlayer * 0.25f) + 2000.0f;
        } else {
            yawSpeed = (relYawTowardsPlayer * 0.25f) - 2000.0f;
        }
        this->actor.shape.rot.y += yawSpeed;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        if (relYawTowardsPlayer > 0) {
            animPlaySpeed = yawSpeed * 1.5f;
            if (animPlaySpeed > 2.0f) {
                animPlaySpeed = 2.0f;
            }
        } else {
            animPlaySpeed = yawSpeed * 1.5f;
            if (animPlaySpeed < -2.0f) {
                animPlaySpeed = -2.0f;
            }
        }
        this->skelAnime.playSpeed = -animPlaySpeed;
        SkelAnime_Update(&this->skelAnime);

        if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnZf_SetupSidestepIfFloor(this, play);
            } else {
                EnZf_SetupMainWalk(this, play);
            }
        }
        if ((play->gameplayFrames & 0x5F) == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
        }
    }
}

static void EnZf_SetupSidestepIfFloor(EnZf* this, PlayState* play) {
    {
        Animation_PlayLoop(&this->skelAnime, &gZfWalkSidewaysAnim);
        this->actor.speed = Rand_CenteredFloat(12.0f);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->timer = (Rand_ZeroOne() * 10.0f) + 20.0f;
        this->actionState = 0;
        this->action = ENZF_ACTION_SIDESTEP;
        this->dodgeSpeed = 0.0f;
        EnZf_SetupAction(this, EnZf_Sidestep);
    }
}

static void EnZf_Sidestep(EnZf* this, PlayState* play) {
    s16 playerYawOpposite;
    s32 beforeCurFrame;
    s32 prevFrame;

    f32 absPlaySpeed;
    Player* player = GET_PLAYER(play);
    f32 sp3C = 0.0f;
    s16 var_v0_3_real;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (!EnZf_ReactToProjectile(play, this) && !((0) && 0)) {
        this->actor.world.rot.y = this->actor.shape.rot.y + 0x3A98;
        playerYawOpposite = player->actor.shape.rot.y + 0x8000;
        if (Math_SinS(playerYawOpposite - this->actor.shape.rot.y) >= 0.0f) {
            this->actor.speed -= 0.25f;
            if (this->actor.speed < -8.0f) {
                this->actor.speed = -8.0f;
            }
        } else if (Math_SinS(playerYawOpposite - this->actor.shape.rot.y) < 0.0f) {
            this->actor.speed += 0.25f;
            if (this->actor.speed > 8.0f) {
                this->actor.speed = 8.0f;
            }
        }
        {
            if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
                !EnZf_TestFloorHeading(this, play, this->actor.speed, this->actor.shape.rot.y + 0x3FFF)) {
                if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                    if (this->actor.speed >= 0.0f) {
                        var_v0_3_real = (this->actor.shape.rot.y + 0x3FFF);
                    } else {
                        var_v0_3_real = (this->actor.shape.rot.y - 0x3FFF);
                    }
                    var_v0_3_real = this->actor.wallYaw - var_v0_3_real;
                } else {
                    var_v0_3_real = 0;
                    this->actor.speed *= -0.8f;
                }
                if (ABS(var_v0_3_real) > 0x4000) {
                    this->actor.speed *= -0.8f;
                    if (this->actor.speed < 0.0f) {
                        this->actor.speed -= 0.5f;
                    } else {
                        this->actor.speed += 0.5f;
                    }
                }
            }
        }
        if (Actor_OtherIsLockedOn(play, &this->actor)) {
            sp3C = 100.0f;
        }
        if (this->actor.xzDistToPlayer <= (70.0f + sp3C)) {
            Math_SmoothStepToF(&this->dodgeSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
        } else if ((90.0f + sp3C) < this->actor.xzDistToPlayer) {
            Math_SmoothStepToF(&this->dodgeSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->dodgeSpeed, 0.0f, 1.0f, 5.65f, 0.0f);
        }
        if ((this->dodgeSpeed != 0.0f) && !EnZf_TestNoFloorAheadShape(this, play, this->dodgeSpeed)) {
            this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->dodgeSpeed;
            this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->dodgeSpeed;
        }
        if (ABS(this->actor.speed) >= ABS(this->dodgeSpeed)) {
            this->skelAnime.playSpeed = this->actor.speed * 0.75f;
        } else if (this->skelAnime.playSpeed < 0.0f) {
            this->skelAnime.playSpeed = this->dodgeSpeed * -0.75f;
        } else {
            this->skelAnime.playSpeed = this->dodgeSpeed * 0.75f;
        }
        prevFrame = (s32)this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
        absPlaySpeed = ABS(this->skelAnime.playSpeed);
        if ((prevFrame != (s32)this->skelAnime.curFrame) &&
            (((beforeCurFrame < 0xE) && (((s32)absPlaySpeed + prevFrame) >= 0x10)) ||
             ((beforeCurFrame < 0x1B) && (((s32)absPlaySpeed + prevFrame) >= 0x1D)))) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_WALK);
        }
        if ((play->gameplayFrames & 0x5F) == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
        }
        if ((Math_CosS(playerYawOpposite - this->actor.shape.rot.y) < -0.85f) || (this->timer == 0)) {
            this->actor.world.rot.y = this->actor.shape.rot.y;
            if ((this->actor.xzDistToPlayer <= 100.0f) && ((play->gameplayFrames % 4) == 0) &&
                EnZf_CanAttack(play, this)) {
                EnZf_SetupAttack(this);
            } else {
                EnZf_SetupIdle(this);
            }
        } else if (this->timer != 0) {
            this->timer--;
        }
    }
}

static void EnZf_SetupAttack(EnZf* this) {
    Animation_Change(&this->skelAnime, &gZfSlashAnim, 1.25f, 0.0f, Animation_GetLastFrame(&gZfSlashAnim), ANIMMODE_ONCE,
                     -4.0f);

    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->action = ENZF_ACTION_ATTACK;
    Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
    this->actor.speed = 0.0f;
    EnZf_SetupAction(this, EnZf_Attack);
}

static void EnZf_Attack(EnZf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 absYawDiffWithPlayer;
    s16 absRelYawTowardsPlayer;

    this->actor.speed = 0.0f;
    if ((s32)this->skelAnime.curFrame == 10) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_ATTACK);
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        enemyLizBlurSpace(Effect_GetByIndex(this->swordBlureEffectIndex));
        if ((Rand_ZeroOne() > 0.7f) || (this->actor.xzDistToPlayer >= 120.0f)) {
            EnZf_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else {
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnZf_SetupCirclePlayerIfFloor(this, play);
            } else {
                absYawDiffWithPlayer = player->actor.shape.rot.y - this->actor.shape.rot.y;
                absYawDiffWithPlayer = ABS(absYawDiffWithPlayer);
                if (absYawDiffWithPlayer <= 0x2710) {
                    absRelYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
                    absRelYawTowardsPlayer = ABS(absRelYawTowardsPlayer);
                    if (absRelYawTowardsPlayer > 0x3E80) {
                        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                        EnZf_SetupCirclePlayerIfFloor(this, play);
                    } else if (((player->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) ||
                                (player->stateFlags1 & (PLAYER_STATE1_MM_2000 | PLAYER_STATE1_MM_4000)))) {
                        if (this->actor.isTargeted) {
                            EnZf_SetupAttack(this);
                        } else {
                            EnZf_SetupCirclePlayerIfFloor(this, play);
                        }
                    } else {
                        EnZf_SetupAttack(this);
                    }
                } else {
                    EnZf_SetupCirclePlayerIfFloor(this, play);
                }
            }
        }
    }
}

static void EnZf_SetupAttackBounced(EnZf* this) {
    f32 f = this->skelAnime.curFrame - 3.0f;

    Animation_Change(&this->skelAnime, &gZfSlashAnim, -1.0f, f, 0.0f, ANIMMODE_ONCE, 0.0f);
    this->action = ENZF_ACTION_ATTACK_BOUNCED;
    EnZf_SetupAction(this, EnZf_AttackBounced);
}

static void EnZf_AttackBounced(EnZf* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            EnZf_SetupIdle(this);
        } else if ((Rand_ZeroOne() > 0.2f) && EnZf_CanAttack(play, this)) {
            EnZf_SetupAttack(this);
        } else {
            EnZf_SetupCirclePlayerIfFloor(this, play);
        }
    }
}

static void EnZf_SetupStunned(EnZf* this) {
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        (((this->actor.velocity.y == 0.0f)) || (this->actor.velocity.y == -4.0f))) {
        this->actor.speed = 0.0f;
        this->actionState = 0;
    } else {
        this->actionState = 1;
    }
    if (this->lastDamageReaction == ENZF_DMG_REACT_ICE) {
        this->frozenTimer = 36;
    } else {
        Animation_PlayOnceSetSpeed(&this->skelAnime, &gZfDamagedAnim, 0.0f);
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->action = ENZF_ACTION_STUNNED;
    EnZf_SetupAction(this, EnZf_Stunned);
}

static void EnZf_Stunned(EnZf* this, PlayState* play) {
    s16 absRelWallYaw;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        this->actor.speed = 0.0f;
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.speed < 0.0f) {
            this->actor.speed += 0.05f;
        }
        this->actionState = 0;
    }
    if ((this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        if (this->actor.colChkInfo.health == 0) {
            EnZf_SetupDie(this);
        } else {
            {
                absRelWallYaw = this->actor.wallYaw - this->actor.shape.rot.y;
                absRelWallYaw = ABS(absRelWallYaw);
                if (!EnZf_ReactToProjectile(play, this)) {
                    {
                        EnZf_PickAction(this, play);
                    }
                }
            }
        }
    }
}

static void EnZf_SetupDamaged(EnZf* this) {
    Animation_Change(&this->skelAnime, &gZfDamagedAnim, 1.5f, 0.0f, Animation_GetLastFrame(&gZfDamagedAnim),
                     ANIMMODE_ONCE, -4.0f);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        ((this->actor.velocity.y == 0.0f) || (this->actor.velocity.y == -4.0f))) {
        this->actor.speed = -4.0f;
        this->actionState = 0;
    } else {
        this->actionState = 1;
    }

    { this->actor.world.rot.y = this->actor.yawTowardsPlayer; }
    Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_DAMAGE);
    this->action = ENZF_ACTION_DAMAGED;
    EnZf_SetupAction(this, EnZf_Damaged);
}

static void EnZf_Damaged(EnZf* this, PlayState* play) {
    s16 absRelWallYaw;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        this->actor.speed = 0.0f;
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.speed < 0.0f) {
            this->actor.speed += 0.05f;
        }
        this->actionState = 0;
    }
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!((0) && 0) && SkelAnime_Update(&this->skelAnime) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        {
            absRelWallYaw = this->actor.wallYaw - this->actor.shape.rot.y;
            absRelWallYaw = ABS(absRelWallYaw);
            if (!EnZf_ReactToProjectile(play, this)) {
                {
                    this->actor.world.rot.y = this->actor.shape.rot.y;
                    if (!EnZf_TestNoFloorAhead(this, play, 135.0f) && (this->actor.xzDistToPlayer < 90.0f)) {
                        EnZf_SetupJumpAttack(this);
                    } else if ((this->actor.xzDistToPlayer <= 100.0f) && ((play->gameplayFrames % 4) == 0)) {
                        EnZf_SetupAttack(this);
                    } else {
                        EnZf_PickAction(this, play);
                    }
                }
            }
        }
    }
}

static void EnZf_SetupJumpAttack(EnZf* this) {
    Animation_Change(&this->skelAnime, &gZfJumpAnim, 1.0f, 0.0f, 3.0f, ANIMMODE_ONCE, 0.0f);
    this->timer = 0;
    this->actionState = 1;
    this->action = ENZF_ACTION_JUMP_ATTACK;
    this->actor.velocity.y = 22.0f;
    this->actor.speed = 7.5f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_JUMP);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnZf_SetupAction(this, EnZf_JumpAttack);
}

static void EnZf_JumpAttack(EnZf* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (this->actor.velocity.y >= 5.0f) {
        func_800355B8(play, &this->leftFootPos);
        func_800355B8(play, &this->rightFootPos);
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer == 0) {
            Animation_Change(&this->skelAnime, &gZfSlashAnim, 3.0f, 0.0f, 13.0f, ANIMMODE_ONCE, -4.0f);
            this->timer = 10;
        } else if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
            this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
            this->actor.velocity.y = 0.0f;
            this->actor.speed = 0.0f;
            this->actor.world.pos.y = this->actor.floorHeight;
            EnZf_SetupAttack(this);
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_ATTACK);
            this->skelAnime.curFrame = 13.0f;
        }
    }
}

static void EnZf_SetupCirclePlayerIfFloor(EnZf* this, PlayState* play) {
    s16 playerYaw;
    Player* player;

    {
        Animation_PlayLoop(&this->skelAnime, &gZfWalkSidewaysAnim);
        player = GET_PLAYER(play);
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
        playerYaw = player->actor.shape.rot.y;
        if (Math_SinS(playerYaw - this->actor.shape.rot.y) >= 0.0f) {
            this->actor.speed = -6.0f;
        } else if (Math_SinS(playerYaw - this->actor.shape.rot.y) < 0.0f) {
            this->actor.speed = 6.0f;
        }
        this->dodgeSpeed = 0.0f;
        this->actionState = 0;
        this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
        this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
        this->action = ENZF_ACTION_CIRCLE_PLAYER;
        EnZf_SetupAction(this, EnZf_CirclePlayer);
    }
}

static void EnZf_CirclePlayer(EnZf* this, PlayState* play) {
    s16 playerYaw;
    s16 yaw;
    Player* player = GET_PLAYER(play);
    s32 prevFrame;
    s32 beforeCurFrame;

    f32 extraRadius = 0.0f;
    f32 absPlaySpeed;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 1);
    playerYaw = player->actor.shape.rot.y;
    {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
            !EnZf_TestFloorHeading(this, play, this->actor.speed, this->actor.shape.rot.y + 0x3FFF)) {
            if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                if (this->actor.speed >= 0.0f) {
                    yaw = this->actor.shape.rot.y + 0x3FFF;
                } else {
                    yaw = this->actor.shape.rot.y - 0x3FFF;
                }
                yaw = this->actor.wallYaw - yaw;
            } else {
                yaw = 0;
                this->actor.speed *= -0.8f;
            }
            if (ABS(yaw) > 0x4000) {
                this->actor.speed *= -0.8f;
                if (this->actor.speed < 0.0f) {
                    this->actor.speed -= 0.5f;
                } else {
                    this->actor.speed += 0.5f;
                }
            }
        }
#if !DEBUG_FEATURES
        {}
#endif
    }
    if (Math_SinS(playerYaw - this->actor.shape.rot.y) >= 0.0f) {
        this->actor.speed += 0.125f;
    } else {
        this->actor.speed -= 0.125f;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x4000;
    if (Actor_OtherIsLockedOn(play, &this->actor)) {
        extraRadius = 100.0f;
    }
    if (this->actor.xzDistToPlayer <= (70.0f + extraRadius)) {
        Math_SmoothStepToF(&this->dodgeSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if ((90.0f + extraRadius) < this->actor.xzDistToPlayer) {
        Math_SmoothStepToF(&this->dodgeSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->dodgeSpeed, 0.0f, 1.0f, 5.65f, 0.0f);
    }
    if ((this->dodgeSpeed != 0.0f) && !EnZf_TestNoFloorAheadShape(this, play, this->dodgeSpeed)) {
        this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->dodgeSpeed;
        this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->dodgeSpeed;
    }
    if (ABS(this->actor.speed) >= ABS(this->dodgeSpeed)) {
        this->skelAnime.playSpeed = -this->actor.speed * 0.75f;
    } else {
        if (this->skelAnime.playSpeed < 0.0f) {
            this->skelAnime.playSpeed = this->dodgeSpeed * -0.75f;
        } else {
            this->skelAnime.playSpeed = this->dodgeSpeed * 0.75f;
        }
    }
    prevFrame = (s32)this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);
    beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
    absPlaySpeed = ((void)0, ABS(this->skelAnime.playSpeed));

    if (this->timer == 0) {
        yaw = player->actor.shape.rot.y - this->actor.shape.rot.y;
        if (yaw < 0) {
            yaw *= -1;
        }
        if (yaw >= 0x3A98) {
            {
                EnZf_SetupIdle(this);
                this->timer = (Rand_ZeroOne() * 5.0f) + 1.0f;
            }
        } else {
            {
                this->actor.world.rot.y = this->actor.shape.rot.y;
                if ((this->actor.xzDistToPlayer <= 100.0f) && ((play->gameplayFrames % 4) == 0) &&
                    EnZf_CanAttack(play, this)) {
                    EnZf_SetupAttack(this);
                } else {
                    if ((this->actor.xzDistToPlayer < 280.0f) && (this->actor.xzDistToPlayer > 240.0f) &&
                        !EnZf_TestNoFloorAhead(this, play, 191.9956f) && ((play->gameplayFrames % 2) == 0)) {
                        EnZf_SetupJumpForwards(this);
                    } else {
                        EnZf_SetupMainWalk(this, play);
                    }
                }
            }
        }

    } else {
        this->timer--;
    }
    if ((prevFrame != (s32)this->skelAnime.curFrame) &&
        (((beforeCurFrame < 14) && (((s32)absPlaySpeed + prevFrame) >= 16)) ||
         ((beforeCurFrame < 27) && (((s32)absPlaySpeed + prevFrame) >= 29)))) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_WALK);
    }
    if ((play->gameplayFrames & 0x5F) == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
    }
}

static void EnZf_SetupDie(EnZf* this) {

    Animation_Change(&this->skelAnime, &gZfDieAnim, 1.5f, 0.0f, Animation_GetLastFrame(&gZfDieAnim), ANIMMODE_ONCE,
                     -4.0f);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        ((this->actor.velocity.y == 0.0f) || (this->actor.velocity.y == -4.0f))) {
        this->actor.speed = 0.0f;
        this->actionState = 0;
    } else {
        this->actionState = 1;
    }
    this->action = ENZF_ACTION_DIE;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_DEAD);
    EnZf_SetupAction(this, EnZf_Die);
}

static void EnZf_Die(EnZf* this, PlayState* play) {
    s32 temp_ft1;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        this->actor.speed = 0.0f;
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.15f, 0.0f);
        this->actionState = 0;
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->actor.category != ACTORCAT_PROP) {
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
        }
        if (this->alpha != 0) {
            this->alpha -= 5;
            this->actor.shape.shadowAlpha = this->alpha;
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        temp_ft1 = (s32)this->skelAnime.curFrame;
        if ((temp_ft1 == 10) || (temp_ft1 == 18)) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_DOWN);
        }
    }
}

static void EnZf_UpdateHeadRot(EnZf* this, PlayState* play) {
    s16 yawTowardsPlayerRelToHead;

    {
        yawTowardsPlayerRelToHead = this->actor.yawTowardsPlayer;
        yawTowardsPlayerRelToHead -= (s16)(this->headRot + this->actor.shape.rot.y);
        this->headRotStep = CLAMP(yawTowardsPlayerRelToHead, -0x7D0, 0x7D0);
        this->headRot += this->headRotStep;
        this->headRot = CLAMP(this->headRot, -0x1CD7, 0x1CD7);
    }
}

static void EnZf_CheckCollide(EnZf* this, PlayState* play) {

    s16 dropParams;

    if ((this->bodyCollider.base.acFlags & AC_HIT) && (this->action <= ENZF_ACTION_STUNNED)) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;
        if (this->actor.colChkInfo.damageEffect != ENZF_DMG_REACT_6) {
            this->lastDamageReaction = this->actor.colChkInfo.damageEffect;
            Actor_SetDropFlag(&this->actor, &this->bodyCollider.elem, false);
            if ((this->actor.colChkInfo.damageEffect == ENZF_DMG_REACT_STUN) ||
                (this->actor.colChkInfo.damageEffect == ENZF_DMG_REACT_ICE)) {
                if (this->action != ENZF_ACTION_STUNNED) {
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);
                    Actor_ApplyDamage(&this->actor);
                    EnZf_SetupStunned(this);
                }
            } else {
                Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_CRY);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                if (Actor_ApplyDamage(&this->actor) == 0) {
                    dropParams = COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false);
                    EnZf_SetupDie(this);

                    Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, dropParams);
                    Enemy_StartFinishingBlow(play, &this->actor);
                } else {

                    EnZf_SetupDamaged(this);
                }
            }
        }
    }
}

static s32 EnZf_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                                 Gfx** gfx) {
    EnZf* this = (EnZf*)thisx;

    switch (limbIndex) {
        case ZF_LIMB_HEAD_ROOT:
            rot->y -= this->headRot;
            break;

        case ZF_LIMB_RIGHT_HAND_AND_SWORD:
            if (this->isSwordSheathed) {
                *dList = gLizalfosRightHandDL;
            }
            break;

        case ZF_LIMB_SHEATH:
            if (this->isSwordSheathed) {
                *dList = gLizalfosSheathedSwordDL;
            }
            break;
    }
    return 0;
}

static void EnZf_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f sp54;
    Vec3f sp48;
    s32 bodyPartIndex = -1;
    EnZf* this = (EnZf*)thisx;

    if (limbIndex == ZF_LIMB_RIGHT_HAND_AND_SWORD) {
        Matrix_MultVec3f(&D_80B4A2BC, &this->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&D_80B4A2C8, &this->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&D_80B4A2D4, &this->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&D_80B4A2E0, &this->swordCollider.dim.quad[2]);
        Collider_SetQuadVertices(&this->swordCollider, &this->swordCollider.dim.quad[0],
                                 &this->swordCollider.dim.quad[1], &this->swordCollider.dim.quad[2],
                                 &this->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&D_80B4A2A4, &sp54);
        Matrix_MultVec3f(&D_80B4A2B0, &sp48);
        if (this->action == ENZF_ACTION_ATTACK) {
            if (this->skelAnime.curFrame < 14.0f) {
                enemyLizBlurSpace(Effect_GetByIndex(this->swordBlureEffectIndex));
            } else if (this->skelAnime.curFrame < 20.0f) {
                enemyLizBlurVertex(Effect_GetByIndex(this->swordBlureEffectIndex), &sp54, &sp48);
            }
        }
    } else {
        Actor_SetFeetPos(&this->actor, limbIndex, ZF_LIMB_LEFT_FOOT_CLAWS, &D_80B4A298, ZF_LIMB_RIGHT_FOOT_CLAWS,
                         &D_80B4A298);
    }
    switch (limbIndex) {
        case ZF_LIMB_LEFT_FOOT_CLAWS:
            Matrix_MultVec3f(&D_80B4A298, &this->leftFootPos);
            break;
        case ZF_LIMB_RIGHT_FOOT_CLAWS:
            Matrix_MultVec3f(&D_80B4A298, &this->rightFootPos);
            break;
    }
    if (this->frozenTimer != 0) {
        switch (limbIndex) {
            case ZF_LIMB_HEAD:
                bodyPartIndex = 0;
                break;
            case ZF_LIMB_NECK:
                bodyPartIndex = 1;
                break;
            case ZF_LIMB_ARMOR:
                bodyPartIndex = 2;
                break;
            case ZF_LIMB_FOREARM:
                bodyPartIndex = 3;
                break;
            case ZF_LIMB_LEFT_FOREARM:
                bodyPartIndex = 4;
                break;
            case ZF_LIMB_BODY:
                bodyPartIndex = 5;
                break;
            case ZF_LIMB_RIGHT_HAND_AND_SWORD_ROOT:
                bodyPartIndex = 6;
                break;
            case ZF_LIMB_RIGHT_SHIN:
                bodyPartIndex = 7;
                break;
            case ZF_LIMB_LEFT_SHIN_ROOT:
                bodyPartIndex = 8;
                break;
        }
        if (bodyPartIndex >= 0) {
            Matrix_MultVec3f(&D_80B4A2EC, &this->bodyPartsPos[bodyPartIndex]);
        }
    }
}

static void EnZf_SetupCirclePlayer(EnZf* this, f32 speed) {
    Animation_MorphToLoop(&this->skelAnime, &gZfWalkSidewaysAnim, -1.0f);
    this->timer = (Rand_ZeroOne() * 10.0f) + 8.0f;
    { this->actor.speed = speed; }
    this->actionState = 0;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
    this->action = ENZF_ACTION_CIRCLE_PLAYER;
    EnZf_SetupAction(this, EnZf_CirclePlayer);
}

static Actor* EnZf_FindIncomingProjectile(PlayState* play, Actor* actor) {
    s32 (*cylinderIntersects)(f32, f32, f32, Vec3f*, Vec3f*, Vec3f*, Vec3f*, Vec3f*) = (void*)0x800e8784;
    for (Actor* item = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; item; item = item->next) {
        if (!item->update || (item->id == ACTOR_EN_ARROW && item->parent) || item->speed <= 0.f ||
            (item->id != ACTOR_ARMS_HOOK && item->id != ACTOR_EN_ARROW))
            continue;
        f32 dx = item->world.pos.x - actor->world.pos.x;
        f32 dy = item->world.pos.y - actor->world.pos.y;
        f32 dz = item->world.pos.z - actor->world.pos.z;
        if (dx * dx + dy * dy + dz * dz > 600.f * 600.f)
            continue;
        if (item->id == ACTOR_ARMS_HOOK && !((Actor_ArmsHook*)item)->timer)
            continue;
        Vec3f end = item->world.pos;
        f32 distance = item->speed * 10.f;
        end.x += distance * Math_SinS(item->world.rot.y);
        end.y += item->velocity.y + item->gravity * 10.f;
        end.z += distance * Math_CosS(item->world.rot.y);
        Vec3f hit1, hit2;
        if (cylinderIntersects(actor->colChkInfo.cylRadius, actor->colChkInfo.cylHeight, 0.f, &actor->world.pos,
                               &item->world.pos, &end, &hit1, &hit2))
            return item;
    }
    return NULL;
}

static s32 EnZf_ReactToProjectile(PlayState* play, EnZf* this) {
    Actor* projectile = EnZf_FindIncomingProjectile(play, &this->actor);
    s16 relYawTowardsProjectile;
    s16 noFloorAheadFlags;
    s16 directionFlags;

    if (projectile != NULL) {
        s16 previousHeading = this->actor.world.rot.y;
        relYawTowardsProjectile =
            Actor_WorldYawTowardActor(&this->actor, projectile) - (s16)(this->actor.shape.rot.y + 0);
        this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
        noFloorAheadFlags = 0;
        if (EnZf_TestNoFloorAhead(this, play, -8.0f)) {
            noFloorAheadFlags |= 1;
        }
        if (EnZf_TestNoFloorAhead(this, play, 8.0f)) {
            noFloorAheadFlags |= 2;
        }
        this->actor.world.rot.y = this->actor.shape.rot.y;
        if ((projectile->id == ACTOR_ARMS_HOOK || this->actor.xzDistToPlayer < 90.0f || noFloorAheadFlags == 3) &&
            !EnZf_TestNoFloorAhead(this, play, 135.0f)) {
            EnZf_SetupJumpAttack(this);
        } else {
            if (noFloorAheadFlags == 3) {
                this->actor.world.rot.y = previousHeading;
                return false;
            }
            this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
            if (noFloorAheadFlags == 0) {
                directionFlags = play->gameplayFrames & 1;
            } else {
                directionFlags = noFloorAheadFlags;
            }
            s32 angle = ABS(relYawTowardsProjectile);
            if (angle == 0x5fff) {
                this->actor.world.rot.y = previousHeading;
                return false;
            }
            f32 dodgeSpeed = (angle < 0x2000 || angle >= 0x6000) ? 8.f : 4.f;
            EnZf_SetupCirclePlayer(this, (directionFlags & 1) ? dodgeSpeed : -dodgeSpeed);
        }
        return true;
    }
    return false;
}

static void EnZf_Update(Actor* thisx, PlayState* play) {
    EnZf* this = (EnZf*)thisx;

    EnZf_CheckCollide(this, play);
    if (this->actor.colChkInfo.damageEffect != ENZF_DMG_REACT_6) {
        this->noFloorAhead = false;
        if (this->actionState != 1) {
            if (this->actor.speed != 0.0f) {
                this->noFloorAhead = EnZf_TestNoFloorAhead(this, play, this->actor.speed * 1.5f);
            }
            if (!this->noFloorAhead) {
                this->noFloorAhead = EnZf_TestNoFloorAhead(this, play, 0.0f);
            }
        }
        if (!this->noFloorAhead) {
            Actor_MoveXZGravity(&this->actor);
        }
        Actor_UpdateBgCheckInfo(play, &this->actor, 25.0f, 30.0f, 60.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                    UPDBGCHECKINFO_FLAG_4);
        if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            this->actionState = 1;
        }
        this->actionFunc(this, play);
    }
    if (this->actor.colChkInfo.health > 0) {
        if ((this->action != ENZF_ACTION_ATTACK) && (this->action != ENZF_ACTION_STUNNED)) {
            EnZf_UpdateHeadRot(this, play);
        }
    }
    if (this->action >= ENZF_ACTION_DIE) {
        Math_SmoothStepToS(&this->headRot, 0, 1, 0x7D0, 0);

        if (this->timer2 >= 2) {
            this->timer2--;
        }
    }
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 40.0f;
    if (this->actor.colChkInfo.health > 0) {
        if (this->alpha == 255) {
            Collider_UpdateCylinder(&this->actor, &this->bodyCollider);
            if ((this->actor.world.pos.y == this->actor.floorHeight) && (this->action <= ENZF_ACTION_DAMAGED)) {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
            }
            if (!((this->actor.colorFilterTimer != 0) && (this->actor.colorFilterParams & 0x4000))) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
            }
        }
    }
    if ((this->action == ENZF_ACTION_ATTACK) && ((this->skelAnime.curFrame >= 14.0f)) &&
        (this->skelAnime.curFrame <= 20.0f)) {
        if (!(this->swordCollider.base.atFlags & AT_BOUNCED) && !(this->swordCollider.base.acFlags & AC_HIT)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordCollider.base);
        } else {
            this->swordCollider.base.atFlags &= ~AT_BOUNCED;
            this->swordCollider.base.acFlags &= ~AC_HIT;
            EnZf_SetupAttackBounced(this);
        }
    }
}

static void EnZf_Draw(Actor* thisx, PlayState* play) {
    EnZf* this = (EnZf*)thisx;
    s32 bodyPartIndex;

    OPEN_DISPS(play->state.gfxCtx);
    if (this->alpha == 255) {
        PreDraw1(&this->actor, play, 1);
        gSPSegment(POLY_OPA_DISP++, 8, D_80B4A2F8);
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_OPA_DISP++, 9, enemyLizalfosOpaque);
        POLY_OPA_DISP = EnemyLizalfosSkelDraw(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                              EnZf_OverrideLimbDraw, EnZf_PostLimbDraw, this, POLY_OPA_DISP);
        if (this->frozenTimer != 0) {
            thisx->colorFilterTimer += 1;
            this->frozenTimer--;
            if ((this->frozenTimer & 3) == 0) {
                bodyPartIndex = this->frozenTimer >> 2;
                EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &this->bodyPartsPos[bodyPartIndex], 150, 150, 150,
                                               250, 235, 245, 255, 1.4f);
            }
        }
    } else {
        PreDraw2(&this->actor, play, 1);
        gSPSegment(POLY_XLU_DISP++, 8, D_80B4A2F8);
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_XLU_DISP++, 9, enemyLizalfosTranslucent);
        POLY_XLU_DISP = EnemyLizalfosSkelDraw(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                              EnZf_OverrideLimbDraw, EnZf_PostLimbDraw, this, POLY_XLU_DISP);
    }
    CLOSE_DISPS();
}

static ActorProfile EnemyLizalfosProfile = {
    0xc25,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x101b,
    sizeof(EnZf),
    EnZf_Init,
    EnZf_Destroy,
    EnZf_Update,
    EnZf_Draw
};
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_PROFILE, EnemyLizalfosProfile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_INIT, EnZf_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_DESTROY, EnZf_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_UPDATE, EnZf_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_LIZALFOS_DRAW, EnZf_Draw);
