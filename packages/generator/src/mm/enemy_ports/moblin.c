#include "moblin.h"
#include <combo/enemy_bodybreak.h>
#include <combo/enemy_port.h>
#include <assets/oot/objects/object_mb.h>

#define gEnMbClubBeatenKneelingAnim ((u8*)0x0600d5d4)
#define gEnMbClubDamagedKneelAnim ((u8*)0x0600c44c)
#define gEnMbClubFallOnItsBackAnim ((u8*)0x0600be58)
#define gEnMbClubLiftClubAnim ((u8*)0x0600b4bc)
#define gEnMbClubSkel ((u8*)0x06014190)
#define gEnMbClubStandStillClubDownAnim ((u8*)0x0600ebe4)
#define gEnMbClubStandUpAnim ((u8*)0x0600e18c)
#define gEnMbClubStrikeDownAnim ((u8*)0x0600abe0)
#define gEnMbSpearChargeAnim ((u8*)0x06002f10)
#define gEnMbSpearDamagedFromBehindAnim ((u8*)0x0600095c)
#define gEnMbSpearDamagedFromFrontAnim ((u8*)0x06001950)
#define gEnMbSpearFallOnItsBackAnim ((u8*)0x060016b4)
#define gEnMbSpearLookLeftAndRightAnim ((u8*)0x060041a8)
#define gEnMbSpearPrepareChargeAnim ((u8*)0x06002c10)
#define gEnMbSpearSkel ((u8*)0x06008f38)
#define gEnMbSpearSlowDownAnim ((u8*)0x06009280)
#define gEnMbSpearStandStillAnim ((u8*)0x060028e0)
#define gEnMbSpearWalkAnim ((u8*)0x06009fc0)
#include <combo/enemy_native.h>

extern void comboEnemyKnockbackLargeNoDamage(PlayState*, Actor*, float, s16, float);
extern void comboEnemySfxWorld(PlayState*, Vec3f*, s32, u16);
extern Actor* comboEnemyActorSpawnAsChild(ActorContext*, Actor*, PlayState*, s16, float, float, float, s16, s16, s16,
                                          s32);

static void moblinShadowFeet(Actor* actor, Lights* lights, PlayState* play) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(Actor*, Lights*, PlayState*))0x800b42f8)(actor, lights, play);
    comboEnemyNativeLeave(&scope);
}

static void moblinFeetPos(Actor* actor, s32 limb, s32 left, Vec3f* leftPos, s32 right, Vec3f* rightPos) {
    ((void (*)(Actor*, s32, s32, Vec3f*, s32, Vec3f*))0x800b4a98)(actor, limb, left, leftPos, right, rightPos);
}

static void moblinFloorDust(PlayState* play, Actor* actor, Vec3f* pos, float radius, s32 count, float acceleration,
                            s16 scale, s16 step, u8 lighting) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    ((void (*)(PlayState*, Actor*, Vec3f*, float, s32, float, s16, s16, u8))0x800bbdac)(
        play, actor, pos, radius, count, acceleration, scale, step, lighting);
    comboEnemyNativeLeave(&scope);
}

static void moblinDust(PlayState* play, Vec3f* origin, float spread, s32 count, s16 scale, s16 step, u8 lighting) {
    Vec3f velocity = { 0, 0, 0 }, acceleration = { 0, 0.3f, 0 };
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    for (s32 i = count; i >= 0; --i) {
        Vec3f pos;
        pos.x = origin->x + (Rand_ZeroOne() - 0.5f) * spread;
        pos.y = origin->y + (Rand_ZeroOne() - 0.5f) * spread;
        pos.z = origin->z + (Rand_ZeroOne() - 0.5f) * spread;
        s16 size = (s16)(scale * Rand_ZeroOne() * 0.2f) + scale;
        ((void (*)(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16))(lighting ? 0x800b1210 : 0x800b11a0))(
            play, &pos, &velocity, &acceleration, size, step);
    }
    comboEnemyNativeLeave(&scope);
}

static void moblinWhiteShockwave(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* acceleration) {
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return;
    EffectSsBlast_SpawnWhiteShockwave(play, pos, velocity, acceleration);
    comboEnemyNativeLeave(&scope);
}

static int moblinOwnsGrab(EnMb* moblin, Player* player) {
    return player->actor.parent == &moblin->actor && (player->stateFlags2 & PLAYER_STATE2_MM_80);
}

static void moblinReleasePlayer(EnMb* moblin, PlayState* play, f32 speed) {
    Player* player = GET_PLAYER(play);
    if (!player || player->actor.parent != &moblin->actor)
        return;
    player->actor.parent = NULL;
    if (!(player->stateFlags2 & PLAYER_STATE2_MM_80))
        return;
    player->av2.actionVar2 = 100;
    if (speed > 0.f) {
        player->stateFlags2 &= ~PLAYER_STATE2_MM_80;
        comboEnemyKnockbackLargeNoDamage(play, &moblin->actor, speed, moblin->actor.world.rot.y, speed);
    }
}

static int moblinCarryPlayer(EnMb* moblin, PlayState* play, Player* player) {
    Actor* actor = &moblin->actor;
    f32 sine = Math_SinS(actor->shape.rot.y);
    f32 cosine = Math_CosS(actor->shape.rot.y);
    Vec3f carried = { actor->world.pos.x + cosine * 10.f + sine * 89.f, player->actor.world.pos.y,
                      actor->world.pos.z + sine * 10.f + cosine * 89.f };
    Vec3f from = player->actor.world.pos;
    Vec3f to = carried;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    from.y += 24.f;
    to.y += 24.f;
    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId))
        return 0;
    f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, &player->actor, &to);
    if (!poly || fabsf(floor - carried.y) > 30.f)
        return 0;

    player->actor.world.pos.x = carried.x;
    player->actor.world.pos.z = carried.z;
    player->av2.actionVar2 = 0;
    player->actor.speed = player->linearVelocity = player->actor.velocity.y = 0.f;
    return 1;
}

static void moblinStartWave(EnMb* this, PlayState* play, Vec3f* position);

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

typedef enum EnMbType { ENMB_TYPE_SPEAR_GUARD = -1, ENMB_TYPE_CLUB, ENMB_TYPE_SPEAR_PATROL } EnMbType;

#define ENMB_ATTACK_NONE 0
#define ENMB_ATTACK_SPEAR 1
#define ENMB_ATTACK_CLUB_RIGHT 1
#define ENMB_ATTACK_CLUB_MIDDLE 2
#define ENMB_ATTACK_CLUB_LEFT 3

typedef enum EnMbLimb {
    ENMB_LIMB_ROOT = 1,
    ENMB_LIMB_WAIST = 3,
    ENMB_LIMB_CHEST = 6,
    ENMB_LIMB_HEAD,
    ENMB_LIMB_LSHOULDER = 9,
    ENMB_LIMB_LFOREARM = 11,
    ENMB_LIMB_LHAND,
    ENMB_LIMB_RSHOULDER = 14,
    ENMB_LIMB_RFOREARM = 16,
    ENMB_LIMB_RHAND,
    ENMB_LIMB_LTHIGH = 20,
    ENMB_LIMB_LSHIN,
    ENMB_LIMB_LFOOT,
    ENMB_LIMB_RTHIGH = 25,
    ENMB_LIMB_RSHIN,
    ENMB_LIMB_RFOOT
} EnMbLimb;

void EnMb_Init(Actor* thisx, PlayState* play);
void EnMb_Destroy(Actor* thisx, PlayState* play);
void EnMb_Update(Actor* thisx, PlayState* play);
void EnMb_Draw(Actor* thisx, PlayState* play);

void EnMb_SetupClubWaitPlayerNear(EnMb* this);
void EnMb_SpearGuardLookAround(EnMb* this, PlayState* play);
void EnMb_SetupSpearGuardLookAround(EnMb* this);
void EnMb_SetupSpearDamaged(EnMb* this);
void EnMb_SpearGuardWalk(EnMb* this, PlayState* play);
void EnMb_SpearGuardPrepareAndCharge(EnMb* this, PlayState* play);
void EnMb_SpearEndChargeQuick(EnMb* this, PlayState* play);
void EnMb_Stunned(EnMb* this, PlayState* play);
void EnMb_ClubDead(EnMb* this, PlayState* play);
void EnMb_ClubDamagedWhileKneeling(EnMb* this, PlayState* play);
void EnMb_ClubWaitPlayerNear(EnMb* this, PlayState* play);
void EnMb_ClubAttack(EnMb* this, PlayState* play);
void EnMb_SpearDead(EnMb* this, PlayState* play);
void EnMb_SpearDamaged(EnMb* this, PlayState* play);
void EnMb_SetupSpearDead(EnMb* this);
void EnMb_ClubWaitAfterAttack(EnMb* this, PlayState* play);
void EnMb_ClubDamaged(EnMb* this, PlayState* play);

static ColliderCylinderInit sBodyColliderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
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

static ColliderTrisElementInit sFrontShieldingTrisElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, 14.0f, 2.0f }, { -10.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, -6.0f, 2.0f }, { 9.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
};

static ColliderTrisInit sFrontShieldingTrisInit = {
    {
        COL_MATERIAL_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    ARRAY_COUNT(sFrontShieldingTrisElementsInit),
    sFrontShieldingTrisElementsInit,
};

static ColliderQuadInit sAttackColliderQuadInit = {
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
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x08 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum EnMbDamageReaction {
    ENMB_DMG_REACT_IGNORE,
    ENMB_DMG_REACT_STUN,
    ENMB_DMG_REACT_FREEZE = 0x5,
    ENMB_DMG_REACT_STUN_ICE,
    ENMB_DMG_REACT_DEFAULT = 0xF
} EnMbDamageReaction;

static DamageTable sSpearMoblinDamageTable = { {
    DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),   DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(1, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_STUN),    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(1, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(4, ENMB_DMG_REACT_STUN_ICE), DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),
    DMG_ENTRY(3, ENMB_DMG_REACT_STUN_ICE), DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
    DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),   DMG_ENTRY(1, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(8, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
    DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
} };

static DamageTable sClubMoblinDamageTable = { {
    DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),   DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_STUN),    DMG_ENTRY(1, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(4, ENMB_DMG_REACT_STUN_ICE), DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),
    DMG_ENTRY(3, ENMB_DMG_REACT_STUN_ICE), DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
    DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),   DMG_ENTRY(1, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(2, ENMB_DMG_REACT_DEFAULT), DMG_ENTRY(8, ENMB_DMG_REACT_DEFAULT),
    DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_FREEZE),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
    DMG_ENTRY(4, ENMB_DMG_REACT_DEFAULT),  DMG_ENTRY(0, ENMB_DMG_REACT_IGNORE),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, 0x4a, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5300, ICHAIN_STOP),
};

void EnMb_SetupAction(EnMb* this, EnMbActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnMb_Init(Actor* thisx, PlayState* play) {
    EnMb* this = (EnMb*)thisx;

    Player* player = GET_PLAYER(play);
    s16 relYawFromPlayer;

    if (thisx->params != -1 && thisx->params != 2) {
        Actor_Kill(thisx);
        return;
    }
    if (thisx->params == 2)
        thisx->params = ENMB_TYPE_CLUB;
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 46.0f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.colChkInfo.damageTable = &sSpearMoblinDamageTable;
    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, &this->actor, &sBodyColliderInit);
    Collider_InitTris(play, &this->frontShieldingCollider);
    Collider_SetTris(play, &this->frontShieldingCollider, &this->actor, &sFrontShieldingTrisInit,
                     this->frontShieldingColliderElements);
    Collider_InitQuad(play, &this->attackCollider);
    Collider_SetQuad(play, &this->attackCollider, &this->actor, &sAttackColliderQuadInit);

    switch (this->actor.params) {
        case ENMB_TYPE_SPEAR_GUARD:
            SkelAnime_InitFlex(play, &this->skelAnime, (FlexSkeletonHeader*)gEnMbSpearSkel,
                               (AnimationHeader*)gEnMbSpearStandStillAnim, this->jointTable, this->morphTable, 28);
            this->actor.colChkInfo.health = 2;
            this->actor.colChkInfo.mass = MASS_HEAVY;
            this->maxHomeDist = 1000.0f;
            this->playerDetectionRange = 1750.0f;
            EnMb_SetupSpearGuardLookAround(this);
            break;
        case ENMB_TYPE_CLUB:
            SkelAnime_InitFlex(play, &this->skelAnime, (FlexSkeletonHeader*)gEnMbClubSkel,
                               (AnimationHeader*)gEnMbClubStandStillClubDownAnim, this->jointTable, this->morphTable,
                               28);

            this->actor.colChkInfo.health = 6;
            this->actor.colChkInfo.mass = MASS_IMMOVABLE;
            this->actor.colChkInfo.damageTable = &sClubMoblinDamageTable;
            Actor_SetScale(&this->actor, 0.02f);
            this->bodyCollider.dim.height = 170;
            this->bodyCollider.dim.radius = 45;
            this->actor.cullingVolumeDistance = 4000.0f;
            this->actor.cullingVolumeScale = 800.0f;
            this->actor.cullingVolumeDownward = 1800.0f;
            this->playerDetectionRange = 710.0f;
            this->attackCollider.elem.atDmgInfo.dmgFlags = DMG_UNBLOCKABLE;

            relYawFromPlayer =
                this->actor.world.rot.y - Math_Vec3f_Yaw(&this->actor.world.pos, &player->actor.world.pos);
            if (ABS(relYawFromPlayer) > 0x4000) {
                this->actor.world.rot.y = thisx->world.rot.y + 0x8000;
                this->actor.shape.rot.y = thisx->world.rot.y;
            }

            ActorShape_Init(&this->actor.shape, 0.0f, moblinShadowFeet, 90.0f);
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->actor.hintId += 0x4b - 0x4a;
            EnMb_SetupClubWaitPlayerNear(this);
            break;
        default:
            break;
    }
}

void EnMb_Destroy(Actor* thisx, PlayState* play) {
    EnMb* this = (EnMb*)thisx;
    moblinReleasePlayer(this, play, 0.f);

    Collider_DestroyTris(play, &this->frontShieldingCollider);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->attackCollider);
}

void EnMb_SetupSpearGuardLookAround(EnMb* this) {
    Animation_MorphToLoop(&this->skelAnime, (AnimationHeader*)gEnMbSpearLookLeftAndRightAnim, -4.0f);
    this->actor.speed = 0.0f;
    this->timer1 = Rand_S16Offset(30, 50);
    this->state = ENMB_STATE_IDLE;
    EnMb_SetupAction(this, EnMb_SpearGuardLookAround);
}

void EnMb_SetupClubWaitPlayerNear(EnMb* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)gEnMbClubStandStillClubDownAnim);
    this->actor.speed = 0.0f;
    this->timer1 = Rand_S16Offset(30, 50);
    this->state = ENMB_STATE_IDLE;
    EnMb_SetupAction(this, EnMb_ClubWaitPlayerNear);
}

void EnMb_SetupSpearGuardWalk(EnMb* this) {
    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbSpearWalkAnim, 0.0f, 0.0f,
                     Animation_GetLastFrame((AnimationHeader*)gEnMbSpearWalkAnim), ANIMMODE_LOOP, -4.0f);
    this->actor.speed = 0.59999996f;
    this->timer1 = Rand_S16Offset(50, 70);
    this->state = ENMB_STATE_WALK;
    EnMb_SetupAction(this, EnMb_SpearGuardWalk);
}

void EnMb_SetupSpearPrepareAndCharge(EnMb* this) {
    f32 frameCount = Animation_GetLastFrame((AnimationHeader*)gEnMbSpearPrepareChargeAnim);

    Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearPrepareChargeAnim, -4.0f);
    this->state = ENMB_STATE_ATTACK;
    this->actor.speed = 0.0f;
    this->timer3 = (s16)frameCount + 6;

    this->timer1 = MAX(60, MIN(200, (s16)(this->actor.xzDistToPlayer / 10.f) + 30));
    this->chargeHit = 0;
    Actor_PlaySfx(&this->actor, 0x38bc);
    EnMb_SetupAction(this, EnMb_SpearGuardPrepareAndCharge);
}

void EnMb_SetupClubAttack(EnMb* this) {
    f32 frames = Animation_GetLastFrame((AnimationHeader*)gEnMbClubLiftClubAnim);
    s16 relYawFromPlayer;

    this->state = ENMB_STATE_ATTACK;
    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubLiftClubAnim, 3.0f, 0.0f, frames,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    this->timer3 = 1;
    relYawFromPlayer = this->actor.world.rot.y - this->actor.yawTowardsPlayer;

    if (ABS(relYawFromPlayer) <= 0x258) {
        this->attack = ENMB_ATTACK_CLUB_MIDDLE;
    } else if (relYawFromPlayer >= 0) {
        this->attack = ENMB_ATTACK_CLUB_RIGHT;
    } else {
        this->attack = ENMB_ATTACK_CLUB_LEFT;
    }

    EnMb_SetupAction(this, EnMb_ClubAttack);
}

void EnMb_SetupSpearEndChargeQuick(EnMb* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearSlowDownAnim);
    this->state = ENMB_STATE_ATTACK_END;
    this->timer1 = 0;
    this->timer3 = 5;
    Actor_PlaySfx(&this->actor, 0x38b9);
    EnMb_SetupAction(this, EnMb_SpearEndChargeQuick);
}

void EnMb_SetupClubWaitAfterAttack(EnMb* this) {
    f32 frameCount = Animation_GetLastFrame((AnimationHeader*)gEnMbClubStandStillClubDownAnim);

    this->state = ENMB_STATE_ATTACK_END;
    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubStandStillClubDownAnim, 5.0f, 0.0f, frameCount,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    EnMb_SetupAction(this, EnMb_ClubWaitAfterAttack);
}

void EnMb_SetupClubDamaged(EnMb* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbClubDamagedKneelAnim);
    this->state = ENMB_STATE_CLUB_KNEELING;
    this->timer1 = 0;
    this->timer3 = 20;
    Actor_PlaySfx(&this->actor, 0x38be);
    EnMb_SetupAction(this, EnMb_ClubDamaged);
}

void EnMb_SetupClubDamagedWhileKneeling(EnMb* this) {
    f32 frames = Animation_GetLastFrame((AnimationHeader*)gEnMbClubBeatenKneelingAnim);

    this->state = ENMB_STATE_CLUB_KNEELING_DAMAGED;
    this->timer1 = 0;
    this->timer3 = 6;
    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubBeatenKneelingAnim, 1.0f, 4.0f, frames,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    EnMb_SetupAction(this, EnMb_ClubDamagedWhileKneeling);
}

void EnMb_SetupClubDead(EnMb* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbClubFallOnItsBackAnim, -4.0f);
    this->state = ENMB_STATE_CLUB_DEAD;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->bodyCollider.dim.height = 80;
    this->bodyCollider.dim.radius = 95;
    this->timer1 = 30;
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, 0x38be);
    EnMb_SetupAction(this, EnMb_ClubDead);
}

void EnMb_SetupStunned(EnMb* this) {
    this->state = ENMB_STATE_STUNNED;
    this->actor.speed = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);
    if (this->damageEffect == ENMB_DMG_REACT_STUN_ICE) {
        this->iceEffectTimer = 40;
    } else {
        if (this->actor.params != ENMB_TYPE_CLUB) {
            Animation_PlayOnceSetSpeed(&this->skelAnime, (AnimationHeader*)gEnMbSpearDamagedFromFrontAnim, 0.0f);
        }
        Actor_PlaySfx(&this->actor, 0x389e);
    }
    EnMb_SetupAction(this, EnMb_Stunned);
}

void EnMb_Stunned(EnMb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (moblinOwnsGrab(this, player)) {
        moblinReleasePlayer(this, play, 4.f);
        this->attack = ENMB_ATTACK_NONE;
    }

    if (this->actor.colorFilterTimer == 0) {
        if (this->actor.params == ENMB_TYPE_CLUB) {
            if (this->actor.colChkInfo.health == 0) {
                EnMb_SetupClubDead(this);
            } else if (this->state == ENMB_STATE_CLUB_KNEELING) {
                EnMb_SetupClubDamagedWhileKneeling(this);
            } else {
                EnMb_SetupClubWaitPlayerNear(this);
            }
        } else {
            if (this->actor.colChkInfo.health == 0) {
                EnMb_SetupSpearDead(this);
            } else {
                EnMb_SetupSpearDamaged(this);
            }
        }
    }
}

void EnMb_SpearGuardLookAround(EnMb* this, PlayState* play) {
    s16 timer1;

    SkelAnime_Update(&this->skelAnime);
    if (this->timer1 == 0) {
        timer1 = 0;
    } else {
        this->timer1--;
        timer1 = this->timer1;
    }
    if (timer1 == 0 && Animation_OnFrame(&this->skelAnime, 0.0f)) {
        EnMb_SetupSpearGuardWalk(this);
    }
}

void EnMb_SpearEndChargeQuick(EnMb* this, PlayState* play) {

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 0.5f, 1.0f, 0.0f);
    if (this->actor.speed > 1.0f) {
        moblinFloorDust(play, &this->actor, &this->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer1 == 0) {
            this->timer3--;
            if (this->timer3 == 0) {
                Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbSpearPrepareChargeAnim, -1.0f,
                                 Animation_GetLastFrame((AnimationHeader*)gEnMbSpearPrepareChargeAnim), 0.0f,
                                 ANIMMODE_ONCE, 0.0f);
                this->timer1 = 1;
                this->actor.speed = 0.0f;
                Actor_PlaySfx(&this->actor, 0x38bd);
            }
        } else {
            EnMb_SetupSpearGuardWalk(this);
            this->timer1 = this->timer2 = this->timer3 = 80;
        }
    }
}

void EnMb_ClubWaitAfterAttack(EnMb* this, PlayState* play) {
    this->attack = ENMB_ATTACK_NONE;
    if (SkelAnime_Update(&this->skelAnime)) {
        EnMb_SetupClubWaitPlayerNear(this);
    }
}

void EnMb_SpearGuardPrepareAndCharge(EnMb* this, PlayState* play) {
    s32 prevFrame;
    Player* player = GET_PLAYER(play);
    Collider* attack = &this->attackCollider.base;
    if ((attack->atFlags & AT_HIT) && attack->at == &player->actor)
        this->chargeHit = 1;

    prevFrame = this->skelAnime.curFrame;

    if (SkelAnime_Update(&this->skelAnime)) {
        Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)gEnMbSpearChargeAnim);
        Actor_PlaySfx(&this->actor, 0x38ba);
    }

    if (this->timer3 != 0) {
        this->timer3--;
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    } else {

        s16 yaw = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
        if (!this->chargeHit && !moblinOwnsGrab(this, player) && yaw > -0x4000 && yaw < 0x4000)
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 8, 0x80, 0);
        this->actor.speed = 10.0f;
        this->attack = ENMB_ATTACK_SPEAR;
        moblinFloorDust(play, &this->actor, &this->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
        if (prevFrame != (s32)this->skelAnime.curFrame &&
            ((s32)this->skelAnime.curFrame == 2 || (s32)this->skelAnime.curFrame == 6)) {
            Actor_PlaySfx(&this->actor, 0x38bf);
        }
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
    int endCharge = 0;
    if (this->attack == ENMB_ATTACK_SPEAR) {
        s16 (*testFloor)(Actor*, PlayState*, f32, s16) = (void*)0x800bc4ec;
        if (!testFloor(&this->actor, play, this->actor.speed, this->actor.world.rot.y)) {
            this->actor.speed = 0.f;
            endCharge = 1;
        }
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || !this->timer1 || !--this->timer1)
            endCharge = 1;
    }
    if (attack->atFlags & AT_HIT) {
        if (attack->at == &player->actor && !endCharge && !player->actor.parent &&
            !(player->stateFlags2 & PLAYER_STATE2_MM_80)) {
            if (player->invincibilityTimer < 0) {
                s8 invincibility = player->invincibilityTimer;
                player->invincibilityTimer = 0;
                if (invincibility > -40)
                    play->damagePlayer(play, -8);
            }
            if (!(attack->atFlags & AT_BOUNCED))
                Actor_PlaySfx(&player->actor, 0x083e);
            if (play->grabPlayer(play, player))
                player->actor.parent = &this->actor;
        }
        attack->atFlags &= ~(AT_HIT | AT_BOUNCED);
    }
    if (moblinOwnsGrab(this, player) && !moblinCarryPlayer(this, play, player))
        endCharge = 1;
    if (endCharge) {
        moblinReleasePlayer(this, play, 4.f);
        this->attack = ENMB_ATTACK_NONE;
        EnMb_SetupSpearEndChargeQuick(this);
    }
}

void EnMb_ClubAttack(EnMb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Vec3f effSpawnPos;
    Vec3f effWhiteShockwaveDynamics = { 0.0f, 0.0f, 0.0f };
    s16 relYawTarget[] = { -0x9C4, 0, 0xDAC };

    Math_SmoothStepToS(&this->actor.shape.rot.y, relYawTarget[this->attack - 1] + this->actor.world.rot.y, 1, 0x2EE, 0);

    if (this->attackCollider.base.atFlags & AT_HIT) {
        this->attackCollider.base.atFlags &= ~AT_HIT;
        if (this->attackCollider.base.at == &player->actor) {
            u8 prevPlayerInvincibilityTimer = player->invincibilityTimer;

            if (player->invincibilityTimer < 0) {
                if (player->invincibilityTimer <= -40) {
                    player->invincibilityTimer = 0;
                } else {
                    player->invincibilityTimer = 0;
                    play->damagePlayer(play, -8);
                }
            }

            comboEnemyKnockbackLargeNoDamage(play, &this->actor, (650.0f - this->actor.xzDistToPlayer) * 0.04f + 4.0f,
                                             this->actor.world.rot.y, 8.0f);

            player->invincibilityTimer = prevPlayerInvincibilityTimer;
        }
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer3 != 0) {
            this->timer3--;
            if (this->timer3 == 0) {
                f32 lastAnimFrame = Animation_GetLastFrame((AnimationHeader*)gEnMbClubStrikeDownAnim);
                Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubStrikeDownAnim, 1.5f, 0.0f, lastAnimFrame,
                                 ANIMMODE_ONCE_INTERP, 0.0f);
            }
        } else {
            effSpawnPos = this->effSpawnPos;
            effSpawnPos.y = this->actor.floorHeight;
            Actor_PlaySfx(&this->actor, 0x39ef);
            Rumble_Request(this->actor.xzDistToPlayer, 255, 20, 150);
            moblinWhiteShockwave(play, &effSpawnPos, &effWhiteShockwaveDynamics, &effWhiteShockwaveDynamics);
            moblinDust(play, &effSpawnPos, 2.0f, 3, 0x12C, 0xB4, 1);
            Camera_RequestQuake(&play->mainCamera, 2, 25, 5);
            moblinStartWave(this, play, &effSpawnPos);
            EnMb_SetupClubWaitAfterAttack(this);
        }
    } else {
        if (this->timer3 != 0 && this->skelAnime.curFrame == 6.0f) {
            Actor_PlaySfx(&this->actor, 0x38e2);

        } else if (this->timer3 == 0 && this->skelAnime.curFrame == 3.0f) {
            Actor_PlaySfx(&this->actor, 0x38e1);
        }
    }
}

void EnMb_ClubDamaged(EnMb* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer3 != 0) {
            Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbClubStandUpAnim);
            this->timer3 = 0;
            Rumble_Request(this->actor.xzDistToPlayer, 255, 20, 150);
            Camera_RequestQuake(&play->mainCamera, 2, 25, 5);
        } else {
            EnMb_SetupClubWaitPlayerNear(this);
        }
    }
}

void EnMb_ClubDamagedWhileKneeling(EnMb* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer3 != 0) {
            this->timer3--;
            if (this->timer3 == 0) {
                if (this->timer1 == 0) {

                    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubStandUpAnim, 3.0f, 0.0f,
                                     Animation_GetLastFrame((AnimationHeader*)gEnMbClubStandUpAnim),
                                     ANIMMODE_ONCE_INTERP, 0.0f);
                    this->timer1 = 1;
                    this->timer3 = 6;
                } else {
                    Animation_Change(&this->skelAnime, (AnimationHeader*)gEnMbClubStandUpAnim, 3.0f, 0.0f,
                                     Animation_GetLastFrame((AnimationHeader*)gEnMbClubStandUpAnim),
                                     ANIMMODE_ONCE_INTERP, 0.0f);
                }
            }
        } else {
            EnMb_SetupClubWaitPlayerNear(this);
        }
    }
}

void EnMb_ClubDead(EnMb* this, PlayState* play) {
    Vec3f effPos;
    Vec3f effPosBase;

    effPos = this->actor.world.pos;
    effPos.x += Math_SinS(this->actor.shape.rot.y) * -70.0f;
    effPos.z += Math_CosS(this->actor.shape.rot.y) * -70.0f;
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    effPosBase = effPos;

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer1 > 0) {
            Vec3f effZeroVec = { 0.0f, 0.0f, 0.0f };
            s32 i;

            this->timer1--;
            for (i = 4; i >= 0; i--) {
                effPos.x = Rand_CenteredFloat(240.0f) + effPosBase.x;
                effPos.y = Rand_CenteredFloat(15.0f) + (effPosBase.y + 20.0f);
                effPos.z = Rand_CenteredFloat(240.0f) + effPosBase.z;
                EffectSsDeadDb_Spawn(play, &effPos, &effZeroVec, &effZeroVec, 230, 7, 255, 255, 255, 255, 0, 255, 0, 1,
                                     9, true);
            }
        } else {
            Item_DropCollectibleRandom(play, &this->actor, &effPos, 0xc0);
            Actor_Kill(&this->actor);
        }
    } else if ((s32)this->skelAnime.curFrame == 15 || (s32)this->skelAnime.curFrame == 22) {
        Rumble_Request(this->actor.xzDistToPlayer, 255, 20, 150);
        moblinFloorDust(play, &this->actor, &effPos, 50.0f, 10, 3.0f, 400, 60, false);
        Actor_PlaySfx(&this->actor, 0x387a);
        Camera_RequestQuake(&play->mainCamera, 2, 25, 5);
    }
}

void EnMb_SpearGuardWalk(EnMb* this, PlayState* play) {
    s32 prevFrame;
    s32 beforeCurFrame;

    Player* player = GET_PLAYER(play);
    s16 relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    s16 yawTowardsHome;
    f32 playSpeedAbs;

    relYawTowardsPlayer = ABS(relYawTowardsPlayer);
    Math_SmoothStepToF(&this->actor.speed, 0.59999996f, 0.1f, 1.0f, 0.0f);
    this->skelAnime.playSpeed = this->actor.speed;
    prevFrame = this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);

    playSpeedAbs = ABS(this->skelAnime.playSpeed);
    beforeCurFrame = this->skelAnime.curFrame - playSpeedAbs;
    playSpeedAbs = ABS(this->skelAnime.playSpeed);
    if (this->timer3 == 0 &&
        Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < this->playerDetectionRange) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->actor.xzDistToPlayer < 500.0f && relYawTowardsPlayer < 0x1388) {
            EnMb_SetupSpearPrepareAndCharge(this);
        }
    } else {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        if (Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos) > this->maxHomeDist || this->timer2 != 0) {
            yawTowardsHome = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
            Math_SmoothStepToS(&this->actor.world.rot.y, yawTowardsHome, 1, 0x2EE, 0);
        }
        if (this->timer2 != 0) {
            this->timer2--;
        }
        if (this->timer3 != 0) {
            this->timer3--;
        }
        if (this->timer2 == 0) {
            Actor_PlaySfx(&this->actor, 0x38bb);
        }
        this->timer1--;
        if (this->timer1 == 0) {
            if (Rand_ZeroOne() > 0.7f) {
                this->timer1 = Rand_S16Offset(50, 70);
                this->timer2 = Rand_S16Offset(15, 40);
            } else {
                EnMb_SetupSpearGuardLookAround(this);
            }
        }
    }

    if (prevFrame != (s32)this->skelAnime.curFrame) {
        if ((beforeCurFrame <= 1 && prevFrame + (s32)playSpeedAbs >= 1) ||
            (beforeCurFrame <= 20 && prevFrame + (s32)playSpeedAbs >= 20)) {
            Actor_PlaySfx(&this->actor, 0x38b8);
        }
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void EnMb_ClubWaitPlayerNear(EnMb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    if (Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < this->playerDetectionRange &&
        !(player->stateFlags1 & PLAYER_STATE1_MM_4000000)) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x600, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
        s16 angle = this->actor.world.rot.y - this->actor.yawTowardsPlayer;
        if (ABS(angle) < 0x3e80)
            EnMb_SetupClubAttack(this);
    }
}

void EnMb_SetupSpearDamaged(EnMb* this) {
    s16 relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if (ABS(relYawTowardsPlayer) <= 0x4000) {
        Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearDamagedFromFrontAnim, -4.0f);
        this->actor.speed = -8.0f;
    } else {
        Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearDamagedFromBehindAnim, -4.0f);
        this->actor.speed = 8.0f;
    }

    this->timer1 = 30;
    this->state = ENMB_STATE_SPEAR_SPEARPATH_DAMAGED;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    Actor_PlaySfx(&this->actor, 0x38be);
    EnMb_SetupAction(this, EnMb_SpearDamaged);
}

void EnMb_SpearDamaged(EnMb* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    if (SkelAnime_Update(&this->skelAnime)) {
        EnMb_SetupSpearGuardLookAround(this);
    }
}

void EnMb_SetupSpearDead(EnMb* this) {
    s16 relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if (ABS(relYawTowardsPlayer) <= 0x4000) {
        Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearFallOnItsBackAnim, -4.0f);
        this->actor.speed = -8.0f;
    } else {
        Animation_MorphToPlayOnce(&this->skelAnime, (AnimationHeader*)gEnMbSpearFallOnItsBackAnim, -4.0f);
        this->actor.speed = 8.0f;
    }

    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->timer1 = 30;
    this->state = ENMB_STATE_SPEAR_SPEARPATH_DAMAGED;
    Actor_PlaySfx(&this->actor, 0x38be);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    EnMb_SetupAction(this, EnMb_SpearDead);
}

void EnMb_SpearDead(EnMb* this, PlayState* play) {

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer1 > 0) {
            Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
            s32 i;
            Vec3f effPos;

            this->actor.shape.shadowScale = 0.0f;
            this->timer1--;
            for (i = 4; i >= 0; i--) {
                effPos.x = Rand_CenteredFloat(110.0f) + this->actor.world.pos.x;
                effPos.y = Rand_CenteredFloat(15.0f) + (this->actor.world.pos.y + 20.0f);
                effPos.z = Rand_CenteredFloat(110.0f) + this->actor.world.pos.z;

                EffectSsDeadDb_Spawn(play, &effPos, &zeroVec, &zeroVec, 100, 7, 255, 255, 255, 255, 0, 255, 0, 1, 9,
                                     true);
            }
        } else {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xe0);
            Actor_Kill(&this->actor);
        }
    }
}

void EnMb_SpearUpdateAttackCollider(Actor* thisx, PlayState* play) {
    Vec3f quadModel0 = { 1000.0f, 1500.0f, 0.0f };
    Vec3f quadModel1 = { -1000.0f, 1500.0f, 0.0f };
    Vec3f quadModel2 = { 1000.0f, 1500.0f, 4500.0f };
    Vec3f quadModel3 = { -1000.0f, 1500.0f, 4500.0f };
    EnMb* this = (EnMb*)thisx;

    if (this->actor.params >= ENMB_TYPE_SPEAR_PATROL) {
        quadModel0.x += 2000.0f;
        quadModel0.z = -4000.0f;
        quadModel1.z = -4000.0f;
        quadModel2.x += 2000.0f;
        quadModel1.x -= 2000.0f;
        quadModel3.x -= 2000.0f;
        quadModel2.z += 4000.0f;
        quadModel3.z += 4000.0f;
    }
    Matrix_MultVec3f(&quadModel0, &this->attackCollider.dim.quad[1]);
    Matrix_MultVec3f(&quadModel1, &this->attackCollider.dim.quad[0]);
    Matrix_MultVec3f(&quadModel2, &this->attackCollider.dim.quad[3]);
    Matrix_MultVec3f(&quadModel3, &this->attackCollider.dim.quad[2]);
    Collider_SetQuadVertices(&this->attackCollider, &this->attackCollider.dim.quad[0],
                             &this->attackCollider.dim.quad[1], &this->attackCollider.dim.quad[2],
                             &this->attackCollider.dim.quad[3]);
}

void EnMb_ClubUpdateAttackCollider(Actor* thisx, PlayState* play) {
    static Vec3f quadModel[] = { { 1000.0f, 0.0f, 0.0f },
                                 { 1000.0f, 0.0f, 0.0f },
                                 { 1000.0f, -8000.0f, -1500.0f },
                                 { 1000.0f, -9000.0f, 2000.0f } };
    EnMb* this = (EnMb*)thisx;

    Matrix_MultVec3f(&quadModel[0], &this->attackCollider.dim.quad[1]);
    Matrix_MultVec3f(&quadModel[1], &this->attackCollider.dim.quad[0]);
    Matrix_MultVec3f(&quadModel[2], &this->attackCollider.dim.quad[3]);
    Matrix_MultVec3f(&quadModel[3], &this->attackCollider.dim.quad[2]);
    Collider_SetQuadVertices(&this->attackCollider, &this->attackCollider.dim.quad[0],
                             &this->attackCollider.dim.quad[1], &this->attackCollider.dim.quad[2],
                             &this->attackCollider.dim.quad[3]);
}

void EnMb_CheckColliding(EnMb* this, PlayState* play) {
    if (this->frontShieldingCollider.base.acFlags & AC_HIT) {
        this->frontShieldingCollider.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
        this->bodyCollider.base.acFlags &= ~AC_HIT;
    } else if ((this->bodyCollider.base.acFlags & AC_HIT) && this->state >= ENMB_STATE_STUNNED) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;
        if (this->actor.colChkInfo.damageEffect != ENMB_DMG_REACT_IGNORE &&
            this->actor.colChkInfo.damageEffect != ENMB_DMG_REACT_FREEZE) {
            moblinReleasePlayer(this, play, 6.f);
            this->damageEffect = this->actor.colChkInfo.damageEffect;
            this->attack = ENMB_ATTACK_NONE;
            Actor_SetDropFlag(&this->actor, &this->bodyCollider.elem, false);
            if (this->actor.colChkInfo.damageEffect == ENMB_DMG_REACT_STUN ||
                this->actor.colChkInfo.damageEffect == ENMB_DMG_REACT_STUN_ICE) {
                if (this->state != ENMB_STATE_STUNNED) {
                    Actor_ApplyDamage(&this->actor);
                    EnMb_SetupStunned(this);
                }
            } else {
                Actor_ApplyDamage(&this->actor);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 250, COLORFILTER_BUFFLAG_OPA, 12);
                if (this->actor.params == ENMB_TYPE_CLUB) {
                    if (this->actor.colChkInfo.health == 0) {
                        EnMb_SetupClubDead(this);
                    } else if (this->state != ENMB_STATE_CLUB_KNEELING) {
                        EnMb_SetupClubDamaged(this);
                    }
                } else {
                    if (this->actor.colChkInfo.health == 0) {
                        EnMb_SetupSpearDead(this);
                    } else {
                        EnMb_SetupSpearDamaged(this);
                    }
                }
            }
        }
    }
}

void EnMb_Update(Actor* thisx, PlayState* play) {
    EnMb* this = (EnMb*)thisx;

    EnMb_CheckColliding(this, play);
    if (thisx->colChkInfo.damageEffect != ENMB_DMG_REACT_FREEZE) {
        this->actionFunc(this, play);
        Actor_MoveWithGravity(thisx);
        Actor_UpdateBgCheckInfo(play, thisx, 40.0f, 40.0f, 70.0f,
                                UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_8 |
                                    UPDBGCHECKINFO_FLAG_MM_10);
        Actor_SetFocus(thisx, thisx->scale.x * 4500.0f);
        Collider_UpdateCylinder(thisx, &this->bodyCollider);
        if (thisx->colChkInfo.health <= 0) {
            this->bodyCollider.dim.pos.x += Math_SinS(thisx->shape.rot.y) * (-4400.0f * thisx->scale.y);
            this->bodyCollider.dim.pos.z += Math_CosS(thisx->shape.rot.y) * (-4400.0f * thisx->scale.y);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
        if (this->state >= ENMB_STATE_STUNNED &&
            (thisx->params == ENMB_TYPE_CLUB || this->state != ENMB_STATE_ATTACK)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }
        if (thisx->params != ENMB_TYPE_CLUB && this->state >= ENMB_STATE_IDLE) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->frontShieldingCollider.base);
        }
        if (this->attack > ENMB_ATTACK_NONE) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->attackCollider.base);
        }
    }
}

void EnMb_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {

    static Vec3f feetPos = { 0.0f, 0.0f, 0.0f };
    static Vec3f effSpawnOffsetFromLeftHand = { 0.0f, -8000.0f, 0.0f };
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s32 bodyPart = -1;
    EnMb* this = (EnMb*)thisx;
    Vec3f bodyPartPos;

    if (this->actor.params == ENMB_TYPE_CLUB) {
        if (limbIndex == ENMB_LIMB_LHAND) {
            Matrix_MultVec3f(&effSpawnOffsetFromLeftHand, &this->effSpawnPos);
            if (this->attack > ENMB_ATTACK_NONE) {
                EnMb_ClubUpdateAttackCollider(&this->actor, play);
            }
        }
        moblinFeetPos(&this->actor, limbIndex, ENMB_LIMB_LFOOT, &feetPos, ENMB_LIMB_RFOOT, &feetPos);
    }

    if (this->iceEffectTimer != 0) {
        switch (limbIndex) {
            case ENMB_LIMB_HEAD:
                bodyPart = 0;
                break;
            case ENMB_LIMB_LHAND:
                bodyPart = 1;
                break;
            case ENMB_LIMB_RHAND:
                bodyPart = 2;
                break;
            case ENMB_LIMB_LSHOULDER:
                bodyPart = 3;
                break;
            case ENMB_LIMB_RSHOULDER:
                bodyPart = 4;
                break;
            case ENMB_LIMB_CHEST:
                bodyPart = 5;
                break;
            case ENMB_LIMB_LTHIGH:
                bodyPart = 6;
                break;
            case ENMB_LIMB_RTHIGH:
                bodyPart = 7;
                break;
            case ENMB_LIMB_LFOOT:
                bodyPart = 8;
                break;
            case ENMB_LIMB_RFOOT:
                bodyPart = 9;
                break;
        }
        if (bodyPart >= 0) {
            Matrix_MultVec3f(&zeroVec, &bodyPartPos);
            this->bodyPartsPos[bodyPart].x = bodyPartPos.x;
            this->bodyPartsPos[bodyPart].y = bodyPartPos.y;
            this->bodyPartsPos[bodyPart].z = bodyPartPos.z;
        }
    }
}

void EnMb_Draw(Actor* thisx, PlayState* play) {
    static Vec3f frontShieldingTriModel0[] = {
        { 4000.0f, 7000.0f, 3500.0f },
        { 4000.0f, 0.0f, 3500.0f },
        { -4000.0f, 7000.0f, 3500.0f },
    };
    static Vec3f frontShieldingTriModel1[] = {
        { -4000.0f, 7000.0f, 3500.0f },
        { -4000.0f, 0.0f, 3500.0f },
        { 4000.0f, 0.0f, 3500.0f },
    };
    s32 i;
    f32 scale;
    Vec3f frontShieldingTri0[3];
    Vec3f frontShieldingTri1[3];
    s32 bodyPartIdx;
    EnMb* this = (EnMb*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          EnMb_PostLimbDraw, thisx);

    if (thisx->params != ENMB_TYPE_CLUB) {
        if (this->attack > ENMB_ATTACK_NONE) {
            EnMb_SpearUpdateAttackCollider(thisx, play);
        }
        for (i = 0; i < 3; i++) {
            Matrix_MultVec3f(&frontShieldingTriModel0[i], &frontShieldingTri0[i]);
            Matrix_MultVec3f(&frontShieldingTriModel1[i], &frontShieldingTri1[i]);
        }
        Collider_SetTrisVertices(&this->frontShieldingCollider, 0, &frontShieldingTri0[0], &frontShieldingTri0[1],
                                 &frontShieldingTri0[2]);
        Collider_SetTrisVertices(&this->frontShieldingCollider, 1, &frontShieldingTri1[0], &frontShieldingTri1[1],
                                 &frontShieldingTri1[2]);
    }

    if (this->iceEffectTimer != 0) {
        thisx->colorFilterTimer++;
        if (this->iceEffectTimer >= 0) {
            this->iceEffectTimer--;
        }
        if ((this->iceEffectTimer % 4) == 0) {
            scale = 2.5f;
            if (thisx->params == ENMB_TYPE_CLUB) {
                scale = 4.0f;
            }
            bodyPartIdx = this->iceEffectTimer >> 2;
            EffectSsEnIce_SpawnFlyingVec3s(play, thisx, &this->bodyPartsPos[bodyPartIdx], 150, 150, 150, 250, 235, 245,
                                           255, scale);
        }
    }
}

static ActorProfile sMoblinPortProfile = {
    0xc4b, ACTORCAT_ENEMY, FLAGS, 0x1030, sizeof(EnMb), EnMb_Init, EnMb_Destroy, EnMb_Update, EnMb_Draw,
};
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MOBLIN, sMoblinPortProfile, EnMb_Init, EnMb_Destroy, EnMb_Update, EnMb_Draw);

typedef struct {
    Actor actor;
    Vec3f origin;
    s16 timer;
    u8 active;
} MoblinWave;

static void moblinStartWave(EnMb* this, PlayState* play, Vec3f* position) {
    Actor* actor = comboEnemyActorSpawnAsChild(&play->actorCtx, &this->actor, play, 7, position->x, position->y,
                                               position->z, this->actor.world.rot.x, this->actor.world.rot.y, 0, -1);
    if (actor) {
        ((MoblinWave*)actor)->origin = this->actor.world.pos;
        if (this->actor.child == actor)
            this->actor.child = NULL;
        actor->parent = NULL;
    }
}
