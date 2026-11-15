#include <combo/enemy_bodybreak_impl.h>
#include "oot_variant_compat.h"
#include "types/en_skb.h"
#define IS_DAY 0
#undef ACTOR_EN_SKB
#define ACTOR_EN_SKB 0xdb0
#undef OBJECT_SKB
#define OBJECT_SKB 0x1184
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 maxAngle))0x800b73e0)
#undef Actor_Kill
extern void FOREIGN_ENEMY_API_KILL(Actor* actor);
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
extern void FOREIGN_ENEMY_API_SFX_ACTOR(Actor* actor, u16 sfxId);
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
extern void FOREIGN_ENEMY_API_COLOR_FILTER(Actor* actor, s16 colorFlag, s16 colorIntensityMax, s16 bufFlag,
                                           s16 duration);
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetDropFlag
extern void FOREIGN_ENEMY_API_DROP_FLAG(Actor* actor, ColliderElement* elem, s32 freezeFlag);
#define Actor_SetDropFlag FOREIGN_ENEMY_API_DROP_FLAG
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x80134748)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef Collider_DestroyJntSph
extern s32 FOREIGN_ENEMY_API_JOINT_DESTROY(PlayState* play, ColliderJntSph* jntSph);
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x800e7fdc)
#undef CollisionCheck_SetAC
extern s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
extern s32 FOREIGN_ENEMY_API_COLLISION_AT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
extern s32 FOREIGN_ENEMY_API_COLLISION_OC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsEnFire_SpawnVec3f
#define EffectSsEnFire_SpawnVec3f ((void (*)(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart))0x800b2df4)
#undef EffectSsHahen_Spawn
extern void FOREIGN_ENEMY_API_DEFAULT_FRAGMENT(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 unused,
                                               s16 scale, s16 objId, s16 life, Gfx* dList);
#define EffectSsHahen_Spawn FOREIGN_ENEMY_API_DEFAULT_FRAGMENT
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Item_DropCollectible
extern EnItem00* FOREIGN_ENEMY_API_DROP(PlayState* play, Vec3f* spawnPos, s16 params);
#define Item_DropCollectible FOREIGN_ENEMY_API_DROP
#undef Item_DropCollectibleRandom
extern void FOREIGN_ENEMY_API_RANDOM_DROP(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 angle))0x80179540)
#undef Math_SinF
#define Math_SinF ((f32 (*)(f32 angle))0x801794ec)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXZ
#define Math_Vec3f_DistXZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff92c)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(PlayState* play, void** skeleton, Vec3s* jointTable, void* overrideLimbDraw, void* postLimbDraw, void* arg))0x80133b3c)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136a7c)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef NA_SE_EN_STALKID_WALK
#define NA_SE_EN_STALKID_WALK 0x3830
#undef NA_SE_EN_STALKID_ATTACK
#define NA_SE_EN_STALKID_ATTACK 0x3831
#undef NA_SE_EN_STALKID_DAMAGE
#define NA_SE_EN_STALKID_DAMAGE 0x3832
#undef NA_SE_EN_STALKID_DEAD
#define NA_SE_EN_STALKID_DEAD 0x3833
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389E
#undef NA_SE_EN_RIVA_APPEAR
#define NA_SE_EN_RIVA_APPEAR 0x3986
#undef NA_SE_EN_AKINDONUTS_HIDE
#define NA_SE_EN_AKINDONUTS_HIDE 0x3987
#define gStalchildSkel (*(SkeletonHeader*)0x060041f8)
#define gStalchildAttackingAnim (*(AnimationHeader*)0x06000460)
#define gStalchildDyingAnim (*(AnimationHeader*)0x060009dc)
#define gStalchildDamagedAnim (*(AnimationHeader*)0x06000d98)
#define gStalchildUncurlingAnim (*(AnimationHeader*)0x06001854)
#define gStalchildWalkingAnim (*(AnimationHeader*)0x060047e0)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

typedef enum StalchildBehavior {
    SKB_BEHAVIOR_BURIED,
    SKB_BEHAVIOR_DYING,
    SKB_BEHAVIOR_DAMAGED,
    SKB_BEHAVIOR_ATTACKING,
    SKB_BEHAVIOR_WALKING,
    SKB_BEHAVIOR_RECOILING,
    SKB_BEHAVIOR_STUNNED
} StalchildBehavior;

void EnSkb_Init(Actor* thisx, PlayState* play);
void EnSkb_Destroy(Actor* thisx, PlayState* play);
void EnSkb_Update(Actor* thisx, PlayState* play);
void EnSkb_Draw(Actor* thisx, PlayState* play);

void EnSkb_SetupRiseFromGround(EnSkb* this);
void EnSkb_RiseFromGround(EnSkb* this, PlayState* play);
void EnSkb_SetupDespawn(EnSkb* this);
void EnSkb_Despawn(EnSkb* this, PlayState* play);
void EnSkb_SetupWalkForward(EnSkb* this);
void EnSkb_WalkForward(EnSkb* this, PlayState* play);
void EnSkb_SetupAttack(EnSkb* this);
void EnSkb_Attack(EnSkb* this, PlayState* play);
void EnSkb_SetupRecoil(EnSkb* this);
void EnSkb_Recoil(EnSkb* this, PlayState* play);
void EnSkb_Stunned(EnSkb* this, PlayState* play);
void EnSkb_TakeDamage(EnSkb* this, PlayState* play);
void EnSkb_SetupDeath(EnSkb* this, PlayState* play);
void EnSkb_Death(EnSkb* this, PlayState* play);

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x04 },
            { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 15, { { 0, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static DamageTable sDamageTable = {
    DMG_ENTRY(0, 0x1), DMG_ENTRY(2, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x1), DMG_ENTRY(2, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x1), DMG_ENTRY(1, 0xE), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(4, 0x7),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(4, 0x7),
    DMG_ENTRY(0, 0x6), DMG_ENTRY(3, 0xD), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xD), DMG_ENTRY(4, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(8, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
};

ActorProfile En_Skb_Profile = {
    ACTOR_EN_SKB, ACTORCAT_ENEMY, FLAGS, OBJECT_SKB, sizeof(EnSkb), EnSkb_Init, EnSkb_Destroy, EnSkb_Update, EnSkb_Draw,
};

void EnSkb_SetupAction(EnSkb* this, EnSkbActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnSkb_SpawnDebris(PlayState* play, EnSkb* this, Vec3f* spawnPos) {
    Vec3f pos;
    Vec3f vel = { 0.0f, 8.0f, 0.0f };
    Vec3f accel = { 0.0f, -1.5f, 0.0f };
    f32 spreadAngle;
    f32 scale;

    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = this->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * 15.0f) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * 15.0f) + spawnPos->z;
    accel.x = Rand_CenteredFloat(1.0f);
    accel.z = Rand_CenteredFloat(1.0f);
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scale = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(play, &pos, &vel, &accel, 2, scale * 0.8f, -1, 10, NULL);
    func_80033480(play, &pos, 10.0f, 1, 150, 0, 1);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(lockOnArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_STOP),
};

void EnSkb_Init(Actor* thisx, PlayState* play) {
    EnSkb* this = (EnSkb*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 2;
    this->actor.shape.yOffset = -8000.0f;
    SkelAnime_Init(play, &this->skelAnime, &gStalchildSkel, &gStalchildUncurlingAnim, this->jointTable,
                   this->morphTable, 20);
    this->actor.naviEnemyId = TATL_HINT_ID_STALCHILD;

    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    Actor_SetScale(&this->actor, ((this->actor.params * 0.1f) + 1.0f) * 0.01f);

    this->collider.elements[0].dim.modelSphere.radius = this->collider.elements[0].dim.worldSphere.radius =
        10 + this->actor.params;
    this->collider.elements[1].dim.modelSphere.radius = this->collider.elements[1].dim.worldSphere.radius =
        20 + (this->actor.params * 2);

    this->actor.home.pos = this->actor.world.pos;
    this->actor.floorHeight = this->actor.world.pos.y;
    EnSkb_SetupRiseFromGround(this);
}

void EnSkb_Destroy(Actor* thisx, PlayState* play) {
    EnSkb* this = (EnSkb*)thisx;
    if (this->bodyBreak.matrices) {
        free(this->bodyBreak.matrices);
        this->bodyBreak.matrices = NULL;
    }

    Collider_DestroyJntSph(play, &this->collider);
}

void EnSkb_DecideNextAction(EnSkb* this) {
    if (IS_DAY) {
        EnSkb_SetupDespawn(this);
    } else if (Actor_IsFacingPlayer(&this->actor, 0x11C7) &&
               (this->actor.xzDistToPlayer < (60.0f + (this->actor.params * 6.0f)))) {
        EnSkb_SetupAttack(this);
    } else {
        EnSkb_SetupWalkForward(this);
    }
}

void EnSkb_SetupRiseFromGround(EnSkb* this) {
    Animation_PlayOnceSetSpeed(&this->skelAnime, &gStalchildUncurlingAnim, 1.0f);
    this->actionState = SKB_BEHAVIOR_BURIED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Actor_PlaySfx(&this->actor, NA_SE_EN_RIVA_APPEAR);
    EnSkb_SetupAction(this, EnSkb_RiseFromGround);
}

void EnSkb_RiseFromGround(EnSkb* this, PlayState* play) {
    if (this->skelAnime.curFrame < 4.0f) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    } else {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    }
    Math_SmoothStepToF(&this->actor.shape.yOffset, 0.0f, 1.0f, 800.0f, 0.0f);
    Math_SmoothStepToF(&this->actor.shape.shadowScale, 25.0f, 1.0f, 2.5f, 0.0f);
    if ((play->gameplayFrames & 1) != 0) {
        EnSkb_SpawnDebris(play, this, &this->actor.world.pos);
    }
    if (SkelAnime_Update(&this->skelAnime) && (0.0f == this->actor.shape.yOffset)) {
        EnSkb_DecideNextAction(this);
    }
}

void EnSkb_SetupDespawn(EnSkb* this) {
    Animation_Change(&this->skelAnime, &gStalchildUncurlingAnim, -1.0f,
                     Animation_GetLastFrame(&gStalchildUncurlingAnim), 0.0f, ANIMMODE_ONCE, -4.0f);
    this->actionState = SKB_BEHAVIOR_BURIED;
    this->setColliderAT = false;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_AKINDONUTS_HIDE);
    EnSkb_SetupAction(this, EnSkb_Despawn);
}

void EnSkb_Despawn(EnSkb* this, PlayState* play) {
    if ((Math_SmoothStepToF(&this->actor.shape.yOffset, -8000.0f, 1.0f, 500.0f, 0.0f) != 0.0f) &&
        (play->gameplayFrames & 1)) {
        EnSkb_SpawnDebris(play, this, &this->actor.world.pos);
    }
    Math_SmoothStepToF(&this->actor.shape.shadowScale, 0.0f, 1.0f, 2.5f, 0.0f);
    if (SkelAnime_Update(&this->skelAnime)) {
        Actor_Kill(&this->actor);
    }
}

void EnSkb_SetupWalkForward(EnSkb* this) {
    Animation_Change(&this->skelAnime, &gStalchildWalkingAnim, 0.96000004f, 0.0f,
                     Animation_GetLastFrame(&gStalchildWalkingAnim), ANIMMODE_LOOP, -4.0f);
    this->actionState = SKB_BEHAVIOR_WALKING;
    this->headlessYawOffset = 0;
    this->actor.speed = this->actor.scale.y * 160.0f;
    EnSkb_SetupAction(this, EnSkb_WalkForward);
}

void EnSkb_WalkForward(EnSkb* this, PlayState* play) {
    s32 thisKeyFrame;
    s32 prevKeyFrame;
    f32 playSpeed;
    Player* player = GET_PLAYER(play);

    if ((this->breakFlags != 0) && ((play->gameplayFrames & 0xF) == 0)) {
        this->headlessYawOffset = Rand_CenteredFloat(50000.0f);
    }
    Math_SmoothStepToS(&this->actor.shape.rot.y, (this->actor.yawTowardsPlayer + this->headlessYawOffset), 1, 0x2EE, 0);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    thisKeyFrame = this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);
    if (this->skelAnime.playSpeed >= 0.0f) {
        playSpeed = this->skelAnime.playSpeed;
    } else {
        playSpeed = -this->skelAnime.playSpeed;
    }
    prevKeyFrame = (this->skelAnime.curFrame - playSpeed);
    if (this->skelAnime.playSpeed >= 0.0f) {
        playSpeed = this->skelAnime.playSpeed;
    } else {
        playSpeed = -this->skelAnime.playSpeed;
    }
    if (thisKeyFrame != (s32)this->skelAnime.curFrame) {
        if (((prevKeyFrame < 9) && (((s32)playSpeed + thisKeyFrame) >= 8)) ||
            !((prevKeyFrame >= 16) || (((s32)playSpeed + thisKeyFrame) < 15))) {

            Actor_PlaySfx(&this->actor, NA_SE_EN_STALKID_WALK);
        }
    }
    if (IS_DAY) {
        EnSkb_SetupDespawn(this);
    } else if (Actor_IsFacingPlayer(&this->actor, 0x11C7) &&
               (this->actor.xzDistToPlayer < (60.0f + (this->actor.params * 6.0f)))) {
        EnSkb_SetupAttack(this);
    }
}

void EnSkb_SetupAttack(EnSkb* this) {
    Animation_Change(&this->skelAnime, &gStalchildAttackingAnim, 0.6f, 0.0f,
                     Animation_GetLastFrame(&gStalchildAttackingAnim), ANIMMODE_ONCE_INTERP, 4.0f);
    this->collider.base.atFlags &= ~AT_BOUNCED;
    this->actionState = SKB_BEHAVIOR_ATTACKING;
    this->actor.speed = 0.0f;
    EnSkb_SetupAction(this, EnSkb_Attack);
}

void EnSkb_Attack(EnSkb* this, PlayState* play) {
    s32 frameData;

    frameData = this->skelAnime.curFrame;
    if (frameData == 3) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALKID_ATTACK);
        this->setColliderAT = true;
    } else if (frameData == 6) {
        this->setColliderAT = false;
    }
    if (this->collider.base.atFlags & AT_BOUNCED) {
        this->collider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        EnSkb_SetupRecoil(this);
    } else if (SkelAnime_Update(&this->skelAnime)) {
        EnSkb_DecideNextAction(this);
    }
}

void EnSkb_SetupRecoil(EnSkb* this) {
    Animation_Change(&this->skelAnime, &gStalchildAttackingAnim, -0.4f, this->skelAnime.curFrame - 1.0f, 0.0f,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    this->collider.base.atFlags &= ~AT_BOUNCED;
    this->actionState = SKB_BEHAVIOR_RECOILING;
    this->setColliderAT = false;
    EnSkb_SetupAction(this, EnSkb_Recoil);
}

void EnSkb_Recoil(EnSkb* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        EnSkb_DecideNextAction(this);
    }
}

void EnSkb_SetupStunned(EnSkb* this) {
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speed = 0.0f;
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->setColliderAT = false;
    this->actionState = SKB_BEHAVIOR_STUNNED;
    EnSkb_SetupAction(this, EnSkb_Stunned);
}

void EnSkb_Stunned(EnSkb* this, PlayState* play) {
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        this->actor.speed = 0.0f;
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.speed < 0.0f) {
            this->actor.speed += 0.05f;
        }
    }
    if ((this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        if (this->actor.colChkInfo.health == 0) {
            EnSkb_SetupDeath(this, play);
        } else {
            EnSkb_DecideNextAction(this);
        }
    }
}

void EnSkb_SetupTakeDamage(EnSkb* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gStalchildDamagedAnim, -4.0f);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speed = -4.0f;
    }
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Actor_PlaySfx(&this->actor, NA_SE_EN_STALKID_DAMAGE);
    this->actionState = SKB_BEHAVIOR_DAMAGED;
    EnSkb_SetupAction(this, EnSkb_TakeDamage);
}

void EnSkb_TakeDamage(EnSkb* this, PlayState* play) {

    u8* new_var;

    new_var = &this->breakFlags;
    if ((this->breakFlags != 1) || BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, 1)) {
        if ((*new_var) != 0) {
            this->breakFlags = (*new_var) | 2;
        }
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            this->actor.speed = 0;
        }
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
            if (this->actor.speed < 0.0f) {
                this->actor.speed += 0.05f;
            }
        }

        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1194, 0);
        if (SkelAnime_Update(&this->skelAnime) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            EnSkb_DecideNextAction(this);
        }
    }
}

void EnSkb_SetupDeath(EnSkb* this, PlayState* play) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gStalchildDyingAnim, -4.0f);
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speed = -6.0f;
    }
    this->actionState = SKB_BEHAVIOR_DYING;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    BodyBreak_Alloc(&this->bodyBreak, 18, play);
    this->breakFlags |= 4;
    EffectSsDeadSound_SpawnStationary(play, &this->actor.projectedPos, NA_SE_EN_STALKID_DEAD, 1, 1, 0x28);
    EnSkb_SetupAction(this, EnSkb_Death);
}

void EnSkb_Death(EnSkb* this, PlayState* play) {
    if (BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, 1)) {
        if (this->actor.scale.x == 0.01f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_1, false));
        } else if (this->actor.scale.x <= 0.015f) {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_BLUE);
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_RED);
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_RED);
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_RED);
        }

        this->breakFlags |= 8;
        Actor_Kill(&this->actor);
    }
}

void EnSkb_CheckDamage(EnSkb* this, PlayState* play) {
    s16 pad;
    s32 i;
    Vec3f flamePos;
    s16 scale;
    s16 colorFilterDuration;
    Player* player;

    if ((this->actionState != SKB_BEHAVIOR_DYING) &&
        (this->actor.bgCheckFlags & (BGCHECKFLAG_WATER | BGCHECKFLAG_WATER_TOUCH)) &&
        (this->actor.depthInWater >= 40.0f)) {
        this->actor.colChkInfo.health = 0;
        this->setColliderAT = false;
        EnSkb_SetupDeath(this, play);
    } else if (this->actionState >= SKB_BEHAVIOR_ATTACKING) {
        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->actor.colChkInfo.damageEffect != 6) {
                this->lastDamageReaction = this->actor.colChkInfo.damageEffect;
                Actor_SetDropFlag(&this->actor, &this->collider.elements[1].base, true);
                this->setColliderAT = false;
                if (this->actor.colChkInfo.damageEffect == 1) {
                    if (this->actionState != SKB_BEHAVIOR_STUNNED) {
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA,
                                             80);
                        Actor_ApplyDamage(&this->actor);
                        EnSkb_SetupStunned(this);
                    }
                } else {
                    colorFilterDuration = 8;
                    if (this->actor.colChkInfo.damageEffect == 7) {
                        scale = this->actor.scale.y * 7500.0f;
                        for (i = 4; i >= 0; i--) {
                            flamePos = this->actor.world.pos;
                            flamePos.x += Rand_CenteredFloat(20.0f);
                            flamePos.z += Rand_CenteredFloat(20.0f);
                            flamePos.y += (Rand_ZeroOne() * 25.0f);
                            EffectSsEnFire_SpawnVec3f(play, &this->actor, &flamePos, scale, 0, 0, -1);
                        }
                        colorFilterDuration = 25;
                    }
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA,
                                         colorFilterDuration);
                    if (!Actor_ApplyDamage(&this->actor)) {
                        EnSkb_SetupDeath(this, play);
                        return;
                    }
                    player = GET_PLAYER(play);
                    if (this->breakFlags == 0) {
                        if ((this->actor.colChkInfo.damageEffect == 0xD) ||
                            ((this->actor.colChkInfo.damageEffect == 0xE) &&
                             ((player->meleeWeaponAnimation >= PLAYER_MWA_RIGHT_SLASH_1H &&
                               player->meleeWeaponAnimation <= PLAYER_MWA_LEFT_COMBO_2H) ||
                              (player->meleeWeaponAnimation == PLAYER_MWA_BACKSLASH_RIGHT ||
                               player->meleeWeaponAnimation == PLAYER_MWA_BACKSLASH_LEFT)))) {
                            BodyBreak_Alloc(&this->bodyBreak, 2, play);
                            this->breakFlags = 1;
                        }
                    }
                    EnSkb_SetupTakeDamage(this);
                }
            }
        }
    }
}

void EnSkb_Update(Actor* thisx, PlayState* play) {
    EnSkb* this = (EnSkb*)thisx;
    s32 pad;

    EnSkb_CheckDamage(this, play);
    Actor_MoveXZGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 15.0f, 30.0f, 60.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    this->actionFunc(this, play);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += (3000.0f * this->actor.scale.y);
    if (this->setColliderAT) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->actionState >= SKB_BEHAVIOR_ATTACKING) {
        if ((this->actor.colorFilterTimer == 0) || ((this->actor.colorFilterParams & 0x4000) == 0)) {

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

s32 EnSkb_OverrideLimbDraw(PlayState* play2, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSkb* this = (EnSkb*)thisx;
    PlayState* play = (PlayState*)play2;
    s16 color;

    if (limbIndex == 11) {
        if ((this->breakFlags & 2) == 0) {
            OPEN_DISPS(play->state.gfxCtx);
            color = ABS((s16)(Math_SinS(play->gameplayFrames * 0x1770) * 95.0f)) + 160;
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, color, color, color, 255);
            CLOSE_DISPS();
        } else {
            *dList = NULL;
        }
    } else if ((limbIndex == 12) && ((this->breakFlags & 2) != 0)) {
        *dList = NULL;
    }
    return 0;
}

void EnSkb_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnSkb* this = (EnSkb*)thisx;

    Collider_UpdateSpheres(limbIndex, &this->collider);

    if ((this->breakFlags ^ 1) == 0) {
        BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 11, 12, 18, dList, BODYBREAK_OBJECT_SLOT_DEFAULT);
    } else if ((this->breakFlags ^ (this->breakFlags | 4)) == 0) {
        BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 0, 18, 18, dList, BODYBREAK_OBJECT_SLOT_DEFAULT);
    }
}

void EnSkb_Draw(Actor* thisx, PlayState* play) {
    EnSkb* this = (EnSkb*)thisx;
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnSkb_OverrideLimbDraw,
                      EnSkb_PostLimbDraw, &this->actor);
}

OVL_INFO_NATIVE_ENEMY(0x1b0, En_Skb_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_INIT, EnSkb_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_DESTROY, EnSkb_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_UPDATE, EnSkb_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_DRAW, EnSkb_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_PROFILE, En_Skb_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x1b0, 0x184, 0x9d494650 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKB_ORIGIN, kNativePortOrigin);
