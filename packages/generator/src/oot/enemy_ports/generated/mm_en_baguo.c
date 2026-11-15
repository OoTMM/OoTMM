#include "mm_native_compat.h"
#define gBoulderFragmentsDL (*((Gfx (*)[1])0x0401fa40))
#define gNejironEyeOpenTex (*((u64 (*)[1])0x060014c8))
#define gNejironEyeHalfTex (*((u64 (*)[1])0x060018c8))
#define gNejironEyeClosedTex (*((u64 (*)[1])0x06001cc8))
#define gNejironSkel (*(SkeletonHeader*)0x060020e8)

#include "types/en_baguo.h"
#define TATL_HINT_ID_NEJIRON 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_BAGUO
#define ACTOR_EN_BAGUO 0x155
#undef OBJECT_GMO
#define OBJECT_GMO 0x1171
#undef NA_SE_IT_BOMB_EXPLOSION
#define NA_SE_IT_BOMB_EXPLOSION 0x180e
#undef NA_SE_EN_BAKUO_ROLL
#define NA_SE_EN_BAKUO_ROLL 0x3805
#undef NA_SE_EN_BAKUO_VOICE
#define NA_SE_EN_BAKUO_VOICE 0x38fb
#undef NA_SE_EN_BAKUO_DEAD
#define NA_SE_EN_BAKUO_DEAD 0x39c8
#undef NA_SE_EN_BAKUO_APPEAR
#define NA_SE_EN_BAKUO_APPEAR 0x39c9
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(struct PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c684u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(struct PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 countMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800260e8u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x8008993cu)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnBaguo_Init(Actor* thisx, PlayState* play);
void EnBaguo_Destroy(Actor* thisx, PlayState* play);
void EnBaguo_Update(Actor* thisx, PlayState* play);

void EnBaguo_UndergroundIdle(EnBaguo* this, PlayState* play);
void EnBaguo_EmergeFromUnderground(EnBaguo* this, PlayState* play);
void EnBaguo_Idle(EnBaguo* this, PlayState* play);
void EnBaguo_Roll(EnBaguo* this, PlayState* play);
void EnBaguo_SetupRetreatUnderground(EnBaguo* this);
void EnBaguo_RetreatUnderground(EnBaguo* this, PlayState* play);
void EnBaguo_DrawBody(Actor* thisx, PlayState* play);
void EnBaguo_InitializeEffect(EnBaguo* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 timer);
void EnBaguo_UpdateEffects(EnBaguo* this, PlayState* play);
void EnBaguo_DrawEffects(EnBaguo* this, PlayState* play);

typedef enum {
    NEJIRON_ACTION_INACTIVE,
    NEJIRON_ACTION_ACTIVE,
    NEJIRON_ACTION_RETREATING,
    NEJIRON_ACTION_EXPLODING
} NejironAction;

typedef enum { NEJIRON_DIRECTION_RIGHT, NEJIRON_DIRECTION_LEFT } NejironRollDirection;

ActorProfile En_Baguo_Profile = {
    0xd55, ACTORCAT_ENEMY, FLAGS, 0x1171, sizeof(EnBaguo), EnBaguo_Init, EnBaguo_Destroy, EnBaguo_Update, NULL,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 0 }, 1 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

typedef enum { NEJIRON_DMGEFF_NONE, NEJIRON_DMGEFF_KILL = 14, NEJIRON_DMGEFF_RECOIL } NejironDamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(3, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(3, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL),
    DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(2, NEJIRON_DMGEFF_KILL),
    DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    DMG_ENTRY(0, NEJIRON_DMGEFF_RECOIL), DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
    DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),
    DMG_ENTRY(0, NEJIRON_DMGEFF_NONE),   DMG_ENTRY(1, NEJIRON_DMGEFF_KILL),
} };

void EnBaguo_Init(Actor* thisx, PlayState* play) {
    EnBaguo* this = (EnBaguo*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    SkelAnime_Init(play, &this->skelAnime, &gNejironSkel, NULL, this->jointTable, this->morphTable, NEJIRON_LIMB_MAX);
    this->actor.hintId = TATL_HINT_ID_NEJIRON;
    this->maxDistanceFromHome = 240.0f;
    this->maxDistanceFromHome += this->actor.world.rot.z * 40.0f;
    this->actor.world.rot.z = 0;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.attentionRangeType = ATTENTION_RANGE_2;

    Collider_InitAndSetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    this->collider.elements[0].dim.modelSphere.radius = 30;
    this->collider.elements[0].dim.scale = 1.0f;
    this->collider.elements[0].dim.modelSphere.center.x = 80;
    this->collider.elements[0].dim.modelSphere.center.y = 80;
    this->collider.elements[0].dim.modelSphere.center.z = 0;

    this->actor.shape.yOffset = -3000.0f;
    this->actor.gravity = -3.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->collider.base.acFlags |= AC_HARD;
    this->actionFunc = EnBaguo_UndergroundIdle;
}

void EnBaguo_Destroy(Actor* thisx, PlayState* play) {
    EnBaguo* this = (EnBaguo*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void EnBaguo_UndergroundIdle(EnBaguo* this, PlayState* play) {
    this->action = NEJIRON_ACTION_INACTIVE;
    if ((this->actor.xzDistToPlayer < 200.0f) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
        this->actor.draw = EnBaguo_DrawBody;
        Actor_PlaySfx(&this->actor, NA_SE_EN_BAKUO_APPEAR);
        this->actor.world.rot.z = 0;
        this->actor.world.rot.x = this->actor.world.rot.z;
        this->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actionFunc = EnBaguo_EmergeFromUnderground;
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void EnBaguo_EmergeFromUnderground(EnBaguo* this, PlayState* play) {
    this->actor.world.rot.y += 0x1518;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((play->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale - 20.0f, 10,
                                 8.0f, 500, 10, true);
    }
    Math_ApproachF(&this->actor.shape.shadowScale, 50.0f, 0.3f, 5.0f);
    Math_ApproachF(&this->actor.shape.yOffset, 2700.0f, 100.0f, 500.0f);
    if (this->actor.shape.yOffset > 2650.0f) {
        this->action = NEJIRON_ACTION_ACTIVE;
        this->actor.shape.yOffset = 2700.0f;
        this->timer = 60;
        this->actionFunc = EnBaguo_Idle;
    }
}

void EnBaguo_Idle(EnBaguo* this, PlayState* play) {
    s16 absoluteYaw;
    s16 yaw;

    if (this->timer != 0) {
        Math_SmoothStepToS(&this->actor.world.rot.x, 0, 10, 0x64, 0x3E8);
        Math_SmoothStepToS(&this->actor.world.rot.z, 0, 10, 0x64, 0x3E8);

        if ((this->timer & 8) != 0) {
            if (fabsf(this->actor.world.rot.y - this->actor.yawTowardsPlayer) > 200.0f) {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 30, 0x12C, 0x3E8);
                if ((play->gameplayFrames % 8) == 0) {
                    Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos,
                                             this->actor.shape.shadowScale - 20.0f, 10, 8.0f, 500, 10, true);
                    Actor_PlaySfx(&this->actor, NA_SE_EN_BAKUO_VOICE);
                }
            }
        }
        this->actor.shape.rot.y = this->actor.world.rot.y;
    } else {
        yaw = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
        absoluteYaw = ABS_ALT(yaw);
        Math_Vec3f_Copy(&this->targetRotation, &gZeroVec3f);
        Math_Vec3f_Copy(&this->currentRotation, &gZeroVec3f);

        if (absoluteYaw < 0x2000) {
            this->targetRotation.x = 2000.0f;
        } else {
            this->zRollDirection = NEJIRON_DIRECTION_RIGHT;
            this->targetRotation.z = 2000.0f;
            if ((s16)(this->actor.yawTowardsPlayer - this->actor.world.rot.y) > 0) {
                this->zRollDirection = NEJIRON_DIRECTION_LEFT;
            }
        }

        this->timer = 38;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->bouncedFlag = 0;
        this->actionFunc = EnBaguo_Roll;
    }
}

void EnBaguo_Roll(EnBaguo* this, PlayState* play) {
    f32 xDistanceFromHome = this->actor.home.pos.x - this->actor.world.pos.x;
    f32 zDistanceFromHome = this->actor.home.pos.z - this->actor.world.pos.z;

    if ((sqrtf(SQ(xDistanceFromHome) + SQ(zDistanceFromHome)) > this->maxDistanceFromHome) ||
        (Player_GetMask(play) == PLAYER_MASK_STONE)) {
        EnBaguo_SetupRetreatUnderground(this);
    } else if (this->timer == 0) {
        this->timer = 100;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->actionFunc = EnBaguo_Idle;
        this->actor.speed = 0.0f;
    } else {
        if (!this->bouncedFlag && (this->collider.base.atFlags & AT_BOUNCED)) {
            this->zRollDirection ^= 1;
            this->bouncedFlag = 1;
            this->actor.speed = -7.0f;
        }

        Math_ApproachF(&this->currentRotation.x, this->targetRotation.x, 0.2f, 1000.0f);
        Math_ApproachF(&this->currentRotation.z, this->targetRotation.z, 0.2f, 1000.0f);
        Math_ApproachF(&this->actor.speed, 5.0f, 0.3f, 0.5f);
        this->actor.world.rot.x += TRUNCF_BINANG(this->currentRotation.x);

        if (this->currentRotation.z != 0.0f) {
            if (this->zRollDirection == NEJIRON_DIRECTION_RIGHT) {
                this->actor.world.rot.z += TRUNCF_BINANG(this->currentRotation.z);
            } else {
                this->actor.world.rot.z -= TRUNCF_BINANG(this->currentRotation.z);
            }
        }

        Actor_PlaySfx(&this->actor, NA_SE_EN_BAKUO_ROLL - SFX_FLAG);
    }
}

void EnBaguo_SetupRetreatUnderground(EnBaguo* this) {
    this->action = NEJIRON_ACTION_RETREATING;
    this->actionFunc = EnBaguo_RetreatUnderground;
    this->actor.speed = 0.0f;
}

void EnBaguo_RetreatUnderground(EnBaguo* this, PlayState* play) {
    this->actor.world.rot.y -= 0x1518;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((play->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale - 20.0f, 10,
                                 8.0f, 500, 10, true);
    }

    Math_ApproachF(&this->actor.shape.yOffset, -3000.0f, 100.0f, 500.0f);
    Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.3f, 5.0f);

    if (this->actor.shape.yOffset < -2970.0f) {
        this->actor.shape.yOffset = -3000.0f;
        this->actor.draw = EnBaguo_DrawBody;
        Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
        Actor_PlaySfx(&this->actor, NA_SE_EN_BAKUO_APPEAR);
        this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        this->actionFunc = EnBaguo_UndergroundIdle;
    }
}

void EnBaguo_PostDetonation(EnBaguo* this, PlayState* play) {
    if (this->timer == 0) {
        Actor_Kill(&this->actor);
    }

    if (this->timer >= 26) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnBaguo_CheckForDetonation(EnBaguo* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    s32 i;

    i = false;
    if (this->action != NEJIRON_ACTION_EXPLODING && this->action != NEJIRON_ACTION_RETREATING) {
        if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
            (this->actor.world.pos.y < (this->actor.home.pos.y - 100.0f))) {
            i = true;
        }
        if ((this->actor.bgCheckFlags & (BGCHECKFLAG_WATER | BGCHECKFLAG_WATER_TOUCH)) &&
            (this->actor.depthInWater >= 40.0f)) {
            i = true;
        }
        if ((this->collider.base.acFlags & AC_HIT) || i) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (i || (this->actor.colChkInfo.damageEffect == NEJIRON_DMGEFF_KILL)) {
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                this->action = NEJIRON_ACTION_EXPLODING;
                this->actor.speed = 0.0f;
                this->actor.shape.shadowScale = 0.0f;

                for (i = 0; i < ARRAY_COUNT(this->effects); i++) {
                    accel.x = (Rand_ZeroOne() - 0.5f) * 8.0f;
                    accel.y = -1.0f;
                    accel.z = (Rand_ZeroOne() - 0.5f) * 8.0f;
                    velocity.x = (Rand_ZeroOne() - 0.5f) * 14.0f;
                    velocity.y = Rand_ZeroOne() * 30.0f;
                    velocity.z = (Rand_ZeroOne() - 0.5f) * 14.0f;
                    EnBaguo_InitializeEffect(this, &this->actor.focus.pos, &velocity, &accel,
                                             (Rand_ZeroFloat(1.0f) * 0.01f) + 0.003f, 90);
                }

                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, 0, 0, CLEAR_TAG_PARAMS(CLEAR_TAG_POP));
                Actor_PlaySfx(&this->actor, NA_SE_IT_BOMB_EXPLOSION);
                Actor_PlaySfx(&this->actor, NA_SE_EN_BAKUO_DEAD);

                this->timer = 30;
                this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                Actor_SetScale(&this->actor, 0.0f);
                this->collider.elements[0].dim.scale = 3.0f;
                this->collider.elements[0].base.atDmgInfo.damage = 8;
                Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0xB0);
                this->actionFunc = EnBaguo_PostDetonation;
            }
        }
    }
}

void EnBaguo_Update(Actor* thisx, PlayState* play) {
    EnBaguo* this = (EnBaguo*)thisx;

    Actor_SetFocus(&this->actor, 30.0f);
    EnBaguo_UpdateEffects(this, play);
    EnBaguo_CheckForDetonation(this, play);
    this->actionFunc(this, play);

    DECR(this->blinkTimer);
    DECR(this->timer);

    if ((this->action != NEJIRON_ACTION_EXPLODING) && (this->action != NEJIRON_ACTION_INACTIVE)) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->action != NEJIRON_ACTION_EXPLODING) {
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.z = this->actor.world.rot.z;

        if (this->blinkTimer == 0) {
            this->eyeIndex++;
            if (this->eyeIndex >= 3) {
                this->eyeIndex = 0;
                this->blinkTimer = Rand_ZeroFloat(60.0f) + 20.0f;
            }
        }

        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 60.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                    UPDBGCHECKINFO_FLAG_10);
        if (this->action != NEJIRON_ACTION_INACTIVE) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
        if (this->action != NEJIRON_ACTION_EXPLODING) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void EnBaguo_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnBaguo* this = (EnBaguo*)thisx;

    Collider_UpdateSpheres(limbIndex, &this->collider);
}

void EnBaguo_DrawBody(Actor* thisx, PlayState* play) {
    static TexturePtr sEyeTextures[] = { &gNejironEyeOpenTex, &gNejironEyeHalfTex, &gNejironEyeClosedTex };
    EnBaguo* this = (EnBaguo*)thisx;
    Gfx* gfx;
    s32 eyeIndex;
    void* virtualAddress;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gfx = POLY_OPA_DISP;

    eyeIndex = this->eyeIndex;
    virtualAddress = Lib_SegmentedToVirtual(sEyeTextures[eyeIndex]);
    gSPSegment(&gfx[0], 0x08, virtualAddress);

    POLY_OPA_DISP = &gfx[1];

    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, EnBaguo_PostLimbDraw,
                      &this->actor);

    CLOSE_DISPS();

    EnBaguo_DrawEffects(this, play);
}

void EnBaguo_InitializeEffect(EnBaguo* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 timer) {
    s16 i;
    NejironEffect* effect = this->effects;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isEnabled) {
            effect->isEnabled = true;
            effect->pos = *pos;
            effect->velocity = *velocity;
            effect->accel = *accel;
            effect->scale = scale;
            effect->timer = timer;
            effect->rot.x = TRUNCF_BINANG(Rand_CenteredFloat(0x7530));
            effect->rot.y = TRUNCF_BINANG(Rand_CenteredFloat(0x7530));
            effect->rot.z = TRUNCF_BINANG(Rand_CenteredFloat(0x7530));
            return;
        }
    }
}

void EnBaguo_UpdateEffects(EnBaguo* this, PlayState* play) {
    s32 i;
    NejironEffect* effect = this->effects;

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

            if (effect->pos.y < (this->actor.world.pos.y - 10.0f)) {
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

void EnBaguo_DrawEffects(EnBaguo* this, PlayState* play) {
    s16 i;
    NejironEffect* effect = this->effects;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isEnabled) {
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 1, 255, 255, 255, 255);
            gSPDisplayList(POLY_OPA_DISP++, gBoulderFragmentsDL);
        }
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x155, En_Baguo_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAGUO_PROFILE, En_Baguo_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAGUO_INIT, EnBaguo_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAGUO_DESTROY, EnBaguo_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAGUO_UPDATE, EnBaguo_Update);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x155u, 0x171u, 0xdb43c397u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BAGUO_ORIGIN, generatedNativeOrigin);
