#include "skull_kid.h"
#include <combo/enemy_port.h>
#include <combo/enemy_native.h>

#define gSkullKidBackflipAnim ((AnimationHeader*)0x600051c)
#define gSkullKidShootNeedleAnim ((AnimationHeader*)0x60007a4)
#define gSkullKidPlayFluteAnim ((AnimationHeader*)0x6000e10)
#define gSkullKidDieAnim ((AnimationHeader*)0x6006a98)
#define gSkullKidHitAnim ((AnimationHeader*)0x6006d84)
#define gSkullKidLandAnim ((AnimationHeader*)0x6007128)
#define gSkullKidLookLeftAndRightAnim ((AnimationHeader*)0x6008174)
#define gSkullKidFightingStanceAnim ((AnimationHeader*)0x6008374)
#define gSkullKidWalkToPlayerAnim ((AnimationHeader*)0x6008e14)
#define gSkullKidWaitAnim ((AnimationHeader*)0x6008b9c)
#define gSkullKidSkel ((FlexSkeletonHeader*)0x6005f40)

#define battleExitTimer multiuseTimer
#define func_8002DDE4(play) (GET_PLAYER(play)->stateFlags2 & (1u << 3))

typedef struct {
    Actor actor;
    u8 private0[0x48];
    ColliderCylinder collider;
    s16 unused, killTimer;
} EnSkjneedle;

typedef struct {
    Vec3s position;
    u8 alignment[2];
    s32 numElements;
    u8 elements[0x480];
    float speed, gravity;
    u32 uDiv, vDiv;
    Color_RGBA8 colorStart[4], colorEnd[4];
    s32 timer, duration;
} EffectSparkInit;
_Static_assert(sizeof(EffectSparkInit) == 0x4c4, "native spark initializer");

typedef enum SkullKidAnim {
    SKJ_ANIM_BACKFLIP,
    SKJ_ANIM_SHOOT_NEEDLE,
    SKJ_ANIM_PLAY_FLUTE,
    SKJ_ANIM_DIE,
    SKJ_ANIM_HIT,
    SKJ_ANIM_LAND,
    SKJ_ANIM_LOOK_LEFT_RIGHT,
    SKJ_ANIM_FIGHTING_STANCE,
    SKJ_ANIM_WALK_TO_PLAYER,
    SKJ_ANIM_WAIT
} SkullKidAnim;

enum {
    SKJ_ACTION_FADE,
    SKJ_ACTION_WAIT_TO_SHOOT_NEEDLE,
    SKJ_ACTION_SARIA_SONG_IDLE,
    SKJ_ACTION_WAIT_FOR_DEATH_ANIM,
    SKJ_ACTION_PICK_NEXT_FIHGT_ACTION,
    SKJ_ACTION_WAIT_FOR_LAND_ANIM,
    SKJ_ACTION_RESET_FIGHT,
    SKJ_ACTION_FIGHT,
    SKJ_ACTION_NEEDLE_RECOVER,
    SKJ_ACTION_SPAWN_DEATH_EFFECT
};
static ColliderCylinderInitType1 D_80B01678 = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x08 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 8, 48, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(4, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(8, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
} };

typedef struct {
    AnimationHeader* animation;
    u8 mode;
    float morphFrames;
} EnemySkullKidAnimation;
static const EnemySkullKidAnimation sAnimationInfo[] = {
    { gSkullKidBackflipAnim, ANIMMODE_ONCE, 0 },
    { gSkullKidShootNeedleAnim, ANIMMODE_ONCE, 0 },
    { gSkullKidPlayFluteAnim, ANIMMODE_LOOP, 0 },
    { gSkullKidDieAnim, ANIMMODE_ONCE, 0 },
    { gSkullKidHitAnim, ANIMMODE_ONCE, 0 },
    { gSkullKidLandAnim, ANIMMODE_ONCE, 0 },
    { gSkullKidLookLeftAndRightAnim, ANIMMODE_LOOP, 0 },
    { gSkullKidFightingStanceAnim, ANIMMODE_LOOP, 0 },
    { gSkullKidWalkToPlayerAnim, ANIMMODE_LOOP, 0 },
    { gSkullKidWaitAnim, ANIMMODE_LOOP, 0 },
};
static void EnSkj_ChangeAnim(EnSkj* this, u8 index);
static void EnSkj_CalculateCenter(EnSkj* this);
static s32 EnSkj_ShootNeedle(EnSkj* this, PlayState* play);
static void EnSkj_SpawnBlood(PlayState* play, Vec3f* pos);
static s32 EnSkj_CollisionCheck(EnSkj* this, PlayState* play);
static s32 func_80AFEDF8(EnSkj* this, PlayState* play);
static void EnSkj_Backflip(EnSkj* this);
static void EnSkj_Fade(EnSkj* this, PlayState* play);
static void EnSkj_SetupWaitToShootNeedle(EnSkj* this);
static void EnSkj_WaitToShootNeedle(EnSkj* this, PlayState* play);
static void EnSkj_SetupResetFight(EnSkj* this);
static void EnSkj_SariasSongKidIdle(EnSkj* this, PlayState* play);
static void EnSkj_SetupDie(EnSkj* this);
static void EnSkj_WaitForDeathAnim(EnSkj* this, PlayState* play);
static void func_80AFF1F0(EnSkj* this);
static void EnSkj_PickNextFightAction(EnSkj* this, PlayState* play);
static void func_80AFF2A0(EnSkj* this);
static void EnSkj_WaitForLandAnim(EnSkj* this, PlayState* play);
static void func_80AFF334(EnSkj* this);
static void EnSkj_ResetFight(EnSkj* this, PlayState* play);
static void EnSkj_SetupStand(EnSkj* this);
static void EnSkj_Fight(EnSkj* this, PlayState* play);
static void EnSkj_SetupNeedleRecover(EnSkj* this);
static void EnSkj_NeedleRecover(EnSkj* this, PlayState* play);
static void EnSkj_SetupSpawnDeathEffect(EnSkj* this);
static void EnSkj_SpawnDeathEffect(EnSkj* this, PlayState* play);
static void EnSkj_Destroy(Actor* thisx, PlayState* play);
static void EnSkj_Update(Actor* thisx, PlayState* play);
static void EnSkj_SetupAction(EnSkj* this, u8 action) {
    static EnSkjActionFunc actions[] = { EnSkj_Fade,
                                         EnSkj_WaitToShootNeedle,
                                         EnSkj_SariasSongKidIdle,
                                         EnSkj_WaitForDeathAnim,
                                         EnSkj_PickNextFightAction,
                                         EnSkj_WaitForLandAnim,
                                         EnSkj_ResetFight,
                                         EnSkj_Fight,
                                         EnSkj_NeedleRecover,
                                         EnSkj_SpawnDeathEffect };
    this->action = action;
    this->actionFunc = actions[action];
    this->unk_2D3 = action != 0 && action != 3 && action != 4 && action != 9;
}

static void EnSkj_Init(Actor* thisx, PlayState* play) {
    EnSkj* this = (EnSkj*)thisx;
    thisx->params = 3;
    thisx->attentionRangeType = 2;
    thisx->targetArrowOffset = 30;
    thisx->hintId = 0xff;
    SkelAnime_InitFlex(play, &this->skelAnime, gSkullKidSkel, gSkullKidPlayFluteAnim, this->jointTable,
                       this->morphTable, 19);
    this->alpha = 255;
    EnSkj_SetupResetFight(this);
    thisx->colChkInfo.damageTable = &sDamageTable;
    thisx->colChkInfo.health = 10;
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinderType1(play, &this->collider, thisx, &D_80B01678);
    ActorShape_Init(&thisx->shape, 0, ActorShadow_DrawCircle, 40);
    Actor_SetScale(thisx, 0.01f);
    this->needlesToShoot = 3;
    this->hitsUntilDodge = 3;
    thisx->gravity = -1;
    EnSkj_CalculateCenter(this);
}

static void EnSkj_ChangeAnim(EnSkj* this, u8 index) {
    f32 endFrame = Animation_GetLastFrame(sAnimationInfo[index].animation);

    this->animIndex = index;
    Animation_Change(&this->skelAnime, sAnimationInfo[index].animation, 1.0f, 0.0f, endFrame,
                     sAnimationInfo[index].mode, sAnimationInfo[index].morphFrames);
}

static void EnSkj_CalculateCenter(EnSkj* this) {
    Vec3f mult;

    mult.x = 0.0f;
    mult.y = 0.0f;
    mult.z = 120.0f;

    Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y), MTXMODE_NEW);
    Matrix_MultVec3f(&mult, &this->center);

    this->center.x += this->actor.world.pos.x;
    this->center.z += this->actor.world.pos.z;
}

static s32 EnSkj_ShootNeedle(EnSkj* this, PlayState* play) {
    Vec3f pos;
    Vec3f pos2;
    EnSkjneedle* needle;

    pos.x = 1.5f;
    pos.y = 0.0f;
    pos.z = 40.0f;

    Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y), MTXMODE_NEW);
    Matrix_MultVec3f(&pos, &pos2);

    pos2.x += this->actor.world.pos.x;
    pos2.z += this->actor.world.pos.z;
    pos2.y = this->actor.world.pos.y + 27.0f;

    needle = (EnSkjneedle*)comboEnemyActorSpawn(&play->actorCtx, play, 0x116, pos2.x, pos2.y, pos2.z,
                                                this->actor.shape.rot.x, this->actor.shape.rot.y,
                                                this->actor.shape.rot.z, 0);
    if (needle != NULL) {
        needle->killTimer = 100;
        needle->actor.speed = 24.0f;
        return 1;
    }
    return 0;
}

static void EnSkj_SpawnBlood(PlayState* play, Vec3f* pos) {
    EffectSparkInit effect = { 0 };
    s32 sp20;

    effect.position.x = pos->x;
    effect.position.y = pos->y;
    effect.position.z = pos->z;
    effect.uDiv = 5;
    effect.vDiv = 5;

    effect.colorStart[0].r = 0;
    effect.colorStart[0].g = 0;
    effect.colorStart[0].b = 128;
    effect.colorStart[0].a = 255;

    effect.colorStart[1].r = 0;
    effect.colorStart[1].g = 0;
    effect.colorStart[1].b = 128;
    effect.colorStart[1].a = 255;

    effect.colorStart[2].r = 0;
    effect.colorStart[2].g = 0;
    effect.colorStart[2].b = 128;
    effect.colorStart[2].a = 255;

    effect.colorStart[3].r = 0;
    effect.colorStart[3].g = 0;
    effect.colorStart[3].b = 128;
    effect.colorStart[3].a = 255;

    effect.colorEnd[0].r = 0;
    effect.colorEnd[0].g = 0;
    effect.colorEnd[0].b = 32;
    effect.colorEnd[0].a = 0;

    effect.colorEnd[1].r = 0;
    effect.colorEnd[1].g = 0;
    effect.colorEnd[1].b = 32;
    effect.colorEnd[1].a = 0;

    effect.colorEnd[2].r = 0;
    effect.colorEnd[2].g = 0;
    effect.colorEnd[2].b = 64;
    effect.colorEnd[2].a = 0;

    effect.colorEnd[3].r = 0;
    effect.colorEnd[3].g = 0;
    effect.colorEnd[3].b = 64;
    effect.colorEnd[3].a = 0;

    effect.speed = 8.0f;
    effect.gravity = -1.0f;

    effect.timer = 0;
    effect.duration = 8;

    EnemyNativeSegments scope = { 0 };
    if (comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN)) {
        Effect_Add(play, &sp20, EFFECT_SPARK, 0, 1, &effect);
        comboEnemyNativeLeave(&scope);
    }
}

static s32 EnSkj_CollisionCheck(EnSkj* this, PlayState* play) {
    s16 yawDiff;
    Vec3f effectPos;

    if (!((this->unk_2D3 == 0) || !(this->collider.base.acFlags & AC_HIT))) {
        this->collider.base.acFlags &= ~AC_HIT;
        switch (this->actor.colChkInfo.damageEffect) {
            case 0xF:
                effectPos.x = this->collider.elem.acDmgInfo.hitPos.x;
                effectPos.y = this->collider.elem.acDmgInfo.hitPos.y;
                effectPos.z = this->collider.elem.acDmgInfo.hitPos.z;

                EnSkj_SpawnBlood(play, &effectPos);
                EffectSsHitMark_SpawnFixedScale(play, 1, &effectPos);

                yawDiff = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
                if ((this->action == 2) || (this->action == 6)) {
                    if ((yawDiff > 0x6000) || (yawDiff < -0x6000)) {
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                        EnSkj_SetupDie(this);
                        return 1;
                    }
                }

                Actor_ApplyDamage(&this->actor);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);

                if (this->actor.colChkInfo.health != 0) {
                    if (this->hitsUntilDodge != 0) {
                        this->hitsUntilDodge--;
                    }
                    if (this->dodgeResetTimer == 0) {
                        this->dodgeResetTimer = 60;
                    }
                    func_80AFF1F0(this);
                    return 1;
                }
                EnSkj_SetupDie(this);
                return 1;

            case 0:
                this->backflipFlag = 1;
                EnSkj_Backflip(this);
                return 1;

            default:
                break;
        }
    }
    return 0;
}

static s32 func_80AFEDF8(EnSkj* this, PlayState* play) {
    s16 yawDiff;

    if (this->actor.xzDistToPlayer < this->unk_2EC) {
        if (func_8002DDE4(play) != 0) {
            return 1;
        }
    }

    yawDiff = this->actor.yawTowardsPlayer - this->actor.world.rot.y;

    if ((yawDiff < this->unk_2C8) && (-this->unk_2C8 < yawDiff)) {
        return 1;
    }

    return 0;
}

static void EnSkj_Backflip(EnSkj* this) {
    this->actor.velocity.y = 8.0f;
    this->actor.speed = -8.0f;

    EnSkj_ChangeAnim(this, SKJ_ANIM_BACKFLIP);
    EnSkj_SetupAction(this, SKJ_ACTION_FADE);
}

static void EnSkj_Fade(EnSkj* this, PlayState* play) {
    u32 alpha = this->alpha;

    alpha -= 20;

    if (this->backflipFlag != 0) {
        if (alpha > 255) {
            alpha = 0;
        }

        this->alpha = alpha;
        this->actor.shape.shadowAlpha = alpha;
    }

    if (this->actor.velocity.y <= 0.0f) {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
            func_80AFF2A0(this);
        }
    }
}

static void EnSkj_SetupWaitToShootNeedle(EnSkj* this) {
    this->needlesToShoot = 3;
    this->needleShootTimer = 0;
    EnSkj_ChangeAnim(this, SKJ_ANIM_SHOOT_NEEDLE);
    EnSkj_SetupAction(this, SKJ_ACTION_WAIT_TO_SHOOT_NEEDLE);
}

static void EnSkj_WaitToShootNeedle(EnSkj* this, PlayState* play) {
    u8 val;
    s16 lastFrame = Animation_GetLastFrame(gSkullKidShootNeedleAnim);

    if ((this->skelAnime.curFrame == lastFrame) && (this->needleShootTimer == 0)) {
        val = this->needlesToShoot;
        if (this->needlesToShoot != 0) {
            EnSkj_ShootNeedle(this, play);
            this->needleShootTimer = 4;
            val--;
            this->needlesToShoot = val;

        } else {
            EnSkj_SetupNeedleRecover(this);
        }
    }
}

static void EnSkj_SetupResetFight(EnSkj* this) {
    this->unk_2C8 = 0xAAA;
    this->unk_2EC = 200.0f;
    EnSkj_ChangeAnim(this, SKJ_ANIM_PLAY_FLUTE);
    EnSkj_SetupAction(this, SKJ_ACTION_SARIA_SONG_IDLE);
}

static void EnSkj_SariasSongKidIdle(EnSkj* this, PlayState* play) {
    if (func_80AFEDF8(this, play))
        func_80AFF334(this);
}

static void EnSkj_SetupDie(EnSkj* this) {
    EnSkj_ChangeAnim(this, SKJ_ANIM_DIE);
    EnSkj_SetupAction(this, SKJ_ACTION_WAIT_FOR_DEATH_ANIM);
}

static void EnSkj_WaitForDeathAnim(EnSkj* this, PlayState* play) {
    s16 lastFrame = Animation_GetLastFrame(gSkullKidDieAnim);

    if (this->skelAnime.curFrame == lastFrame) {
        EnSkj_SetupSpawnDeathEffect(this);
    }
}

static void func_80AFF1F0(EnSkj* this) {
    EnSkj_ChangeAnim(this, SKJ_ANIM_HIT);
    EnSkj_SetupAction(this, SKJ_ACTION_PICK_NEXT_FIHGT_ACTION);
}

static void EnSkj_PickNextFightAction(EnSkj* this, PlayState* play) {
    s16 lastFrame = Animation_GetLastFrame(gSkullKidHitAnim);

    if (this->skelAnime.curFrame == lastFrame) {
        if (this->hitsUntilDodge == 0) {
            this->hitsUntilDodge = 3;
            EnSkj_Backflip(this);
        } else {
            EnSkj_SetupStand(this);
        }
    }
}

static void func_80AFF2A0(EnSkj* this) {
    EnSkj_CalculateCenter(this);
    this->actor.speed = 0.0f;
    EnSkj_ChangeAnim(this, SKJ_ANIM_LAND);
    EnSkj_SetupAction(this, SKJ_ACTION_WAIT_FOR_LAND_ANIM);
}

static void EnSkj_WaitForLandAnim(EnSkj* this, PlayState* play) {
    s16 lastFrame = Animation_GetLastFrame(gSkullKidLandAnim);

    if (this->skelAnime.curFrame == lastFrame) {
        EnSkj_SetupStand(this);
    }
}

static void func_80AFF334(EnSkj* this) {
    this->unk_2C8 = 0x2000;
    this->battleExitTimer = 400;
    this->unk_2EC = 600.0f;
    EnSkj_ChangeAnim(this, SKJ_ANIM_LOOK_LEFT_RIGHT);
    EnSkj_SetupAction(this, SKJ_ACTION_RESET_FIGHT);
}

static void EnSkj_ResetFight(EnSkj* this, PlayState* play) {
    if (this->battleExitTimer == 0) {
        EnSkj_SetupResetFight(this);
    } else if (func_80AFEDF8(this, play) != 0) {
        this->battleExitTimer = 600;
        EnSkj_SetupStand(this);
    }
}

static void EnSkj_SetupStand(EnSkj* this) {
    this->needleShootTimer = 60;
    this->unk_2C8 = 0x2000;
    this->unk_2F0 = 0.0f;
    this->unk_2EC = 600.0f;
    EnSkj_ChangeAnim(this, SKJ_ANIM_FIGHTING_STANCE);
    EnSkj_SetupAction(this, SKJ_ACTION_FIGHT);
}

static void EnSkj_Fight(EnSkj* this, PlayState* play) {
    Vec3f pos1;
    Vec3f pos2;
    f32 prevPosX;
    f32 prevPosZ;
    f32 phi_f14;
    s16 yawDistToPlayer;

    if (this->needleShootTimer == 0) {
        EnSkj_SetupWaitToShootNeedle(this);
    } else if (this->battleExitTimer != 0) {
        yawDistToPlayer =
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, this->unk_2F0, 0);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        Math_ApproachF(&this->unk_2F0, 2000.0f, 1.0f, 200.0f);

        pos1.x = 0.0f;
        pos1.y = 0.0f;
        pos1.z = -120.0f;

        Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y), MTXMODE_NEW);
        Matrix_MultVec3f(&pos1, &pos2);
        prevPosX = this->actor.world.pos.x;
        prevPosZ = this->actor.world.pos.z;
        if (1) {
        }
        this->actor.world.pos.x = this->center.x + pos2.x;
        this->actor.world.pos.z = this->center.z + pos2.z;

        phi_f14 = sqrtf(SQ(this->actor.world.pos.x - prevPosX) + SQ(this->actor.world.pos.z - prevPosZ));
        phi_f14 = CLAMP_MAX(phi_f14, 10.0f);
        phi_f14 /= 10.0f;

        this->skelAnime.playSpeed = (yawDistToPlayer < 0) ? -(1.0f + phi_f14) : (1.0f + phi_f14);

    } else if (func_80AFEDF8(this, play) != 0) {
        this->backflipFlag = 1;
        EnSkj_Backflip(this);
    } else {
        EnSkj_SetupResetFight(this);
    }
}

static void EnSkj_SetupNeedleRecover(EnSkj* this) {
    Animation_Reverse(&this->skelAnime);
    EnSkj_SetupAction(this, SKJ_ACTION_NEEDLE_RECOVER);
}

static void EnSkj_NeedleRecover(EnSkj* this, PlayState* play) {
    if (this->skelAnime.curFrame == 0.0f) {
        EnSkj_SetupStand(this);
    }
}

static void EnSkj_SetupSpawnDeathEffect(EnSkj* this) {
    this->backflipFlag = 1;
    EnSkj_SetupAction(this, SKJ_ACTION_SPAWN_DEATH_EFFECT);
}

static void EnSkj_SpawnDeathEffect(EnSkj* this, PlayState* play) {
    Vec3f effectPos;
    Vec3f effectVel;
    Vec3f effectAccel;
    u32 phi_v0;

    phi_v0 = this->alpha - 4;

    if (phi_v0 > 255) {
        phi_v0 = 0;
    }
    this->alpha = phi_v0;
    this->actor.shape.shadowAlpha = phi_v0;

    effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.world.pos.x;
    effectPos.y = Rand_CenteredFloat(30.0f) + this->actor.world.pos.y;
    effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.world.pos.z;

    effectAccel.z = 0.0f;
    effectAccel.y = 0.0f;
    effectAccel.x = 0.0f;

    effectVel.z = 0.0f;
    effectVel.y = 0.0f;
    effectVel.x = 0.0f;

    EffectSsDeadDb_Spawn(play, &effectPos, &effectVel, &effectAccel, 100, 10, 255, 255, 255, 255, 0, 0, 255, 1, 9, 1);
}

static void EnSkj_Destroy(Actor* thisx, PlayState* play) {
    EnSkj* this = (EnSkj*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

static void EnSkj_Update(Actor* thisx, PlayState* play) {
    Vec3f dropPos;
    EnSkj* this = (EnSkj*)thisx;

    this->timer++;

    if (this->multiuseTimer != 0) {
        this->multiuseTimer--;
    }

    if (this->needleShootTimer != 0) {
        this->needleShootTimer--;
    }

    if (this->dodgeResetTimer != 0) {
        this->dodgeResetTimer--;
    }

    if (this->dodgeResetTimer == 0) {
        this->hitsUntilDodge = 3;
    }

    if ((this->backflipFlag != 0) && (this->alpha == 0)) {
        if (this->action == 9) {
            dropPos.x = this->actor.world.pos.x;
            dropPos.y = this->actor.world.pos.y;
            dropPos.z = this->actor.world.pos.z;

            Item_DropCollectible(play, &dropPos, ITEM00_RUPEE_ORANGE);
        }
        Actor_Kill(&this->actor);
        return;
    }

    Actor_SetFocus(&this->actor, 30.0f);
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc(this, play);
    this->actor.messageId = this->textId;
    EnSkj_CollisionCheck(this, play);
    Collider_UpdateCylinder(&this->actor, &this->collider);

    if ((this->unk_2D3 != 0)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);

        if (this->actor.colorFilterTimer == 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    SkelAnime_Update(&this->skelAnime);
    Actor_MoveXZGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_2 | UPDBGCHECKINFO_FLAG_MM_4);
}

static Gfx* enemySkullKidAlphaList(GraphicsContext* gfx, u32 alpha) {
    Gfx* list = GRAPH_ALLOC(gfx, 3 * sizeof(Gfx));
    Gfx* next = list;
    if (alpha < 255)
        gDPSetRenderMode(next++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(next++, 0, 0, 0, alpha);
    gSPEndDisplayList(next++);
    return list;
}

static void EnSkj_Draw(Actor* actor, PlayState* play) {
    EnSkj* this = (EnSkj*)actor;
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x0c, enemySkullKidAlphaList(play->state.gfxCtx, this->alpha));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->jointTable, this->skelAnime.dListCount, NULL, NULL,
                          this);
    CLOSE_DISPS();
}

static ActorProfile EnemySkullKidProfile = {
    0xd15,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x110a,
    sizeof(EnSkj),
    EnSkj_Init,
    EnSkj_Destroy,
    EnSkj_Update,
    EnSkj_Draw,
};
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_SKULL_KID, EnemySkullKidProfile, EnSkj_Init, EnSkj_Destroy, EnSkj_Update,
                     EnSkj_Draw);
