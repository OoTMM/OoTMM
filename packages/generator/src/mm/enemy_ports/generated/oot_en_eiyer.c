#include "oot_compat.h"
#include "types/en_eiyer.h"
#undef ACTOR_EN_EIYER
#define ACTOR_EN_EIYER 0x003a
#undef OBJECT_EI
#define OBJECT_EI 0x1026
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387b
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389e
#undef NA_SE_EN_OCTAROCK_JUMP
#define NA_SE_EN_OCTAROCK_JUMP 0x38c2
#undef NA_SE_EN_OCTAROCK_SINK
#define NA_SE_EN_OCTAROCK_SINK 0x38c4
#undef NA_SE_EN_EIER_DAMAGE
#define NA_SE_EN_EIER_DAMAGE 0x394c
#undef NA_SE_EN_EIER_DEAD
#define NA_SE_EN_EIER_DEAD 0x394d
#undef NA_SE_EN_EIER_FLUTTER
#define NA_SE_EN_EIER_FLUTTER 0x394e
#undef NA_SE_EN_EIER_FLY
#define NA_SE_EN_EIER_FLY 0x394f
#undef DMG_DEKU_NUT
#define DMG_DEKU_NUT (1 << 0)
#undef DMG_EXPLOSIVE
#define DMG_EXPLOSIVE (1 << 3)
#undef DMG_BOOMERANG
#define DMG_BOOMERANG (1 << 4)
#undef DMG_DEFAULT
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)
#define gStingerSkel (*(SkeletonHeader*)0x06003410)
#define gStingerPopOutAnim (*(AnimationHeader*)0x06000288)
#define gStingerDiveAnim (*(AnimationHeader*)0x060004c4)
#define gStingerBackflipAnim (*(AnimationHeader*)0x06000704)
#define gStingerHitAnim (*(AnimationHeader*)0x06000fc0)
#define gStingerIdleAnim (*(AnimationHeader*)0x060012ac)

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXYZ
#define Actor_MoveXYZ ((void (*)(Actor* actor))0x800b6b24)
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged FOREIGN_ENEMY_API_SFX_FLAGGED
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetDropFlag
#define Actor_SetDropFlag FOREIGN_ENEMY_API_DROP_FLAG
#undef Actor_SpawnAsChild
extern Actor* FOREIGN_ENEMY_API_SPAWN_CHILD(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId,
                                            f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_SpawnAsChild FOREIGN_ENEMY_API_SPAWN_CHILD
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x800b6e48)
#undef Actor_WorldPitchTowardPoint
#define Actor_WorldPitchTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x800b6da8)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* origin, Vec3f* point))0x800b6d2c)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137594)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x801378b8)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137430)
#undef BgCheck_EntityRaycastDown4
#define BgCheck_EntityRaycastDown4 ((f32 (*)(CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Actor* actor, Vec3f* pos))0x800c411c)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x800e7530)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x800b1f4c)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 step))0x800fffd8)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x800fee70)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x800ff03c)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x800ff450)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SfxSource_PlaySfxAtFixedWorldPos
#define SfxSource_PlaySfxAtFixedWorldPos FOREIGN_ENEMY_API_SFX_WORLD
#undef SkelAnime_Draw
#define SkelAnime_Draw ((Gfx* (*)(PlayState* play, void** skeleton, Vec3s* jointTable, void* overrideLimbDraw, void* postLimbDraw, void* arg, Gfx* gfx))0x80134990)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136a7c)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef cosf
#define cosf ((f32 (*)(f32 x))0x80091f40)
#undef sinf
#define sinf ((f32 (*)(f32 x))0x80088350)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnEiyer_Init(Actor* thisx, PlayState* play);
void EnEiyer_Destroy(Actor* thisx, PlayState* play);
void EnEiyer_Update(Actor* thisx, PlayState* play);
void EnEiyer_Draw(Actor* thisx, PlayState* play);

void EnEiyer_SetupAppearFromGround(EnEiyer* this);
void EnEiyer_SetupUnderground(EnEiyer* this);
void EnEiyer_SetupInactive(EnEiyer* this);
void EnEiyer_SetupAmbush(EnEiyer* this, PlayState* play);
void EnEiyer_SetupGlide(EnEiyer* this);
void EnEiyer_SetupStartAttack(EnEiyer* this);
void EnEiyer_SetupDiveAttack(EnEiyer* this, PlayState* play);
void EnEiyer_SetupLand(EnEiyer* this);
void EnEiyer_SetupHurt(EnEiyer* this);
void EnEiyer_SetupDie(EnEiyer* this);
void EnEiyer_SetupDead(EnEiyer* this);
void EnEiyer_SetupStunned(EnEiyer* this);

void EnEiyer_AppearFromGround(EnEiyer* this, PlayState* play);
void EnEiyer_WanderUnderground(EnEiyer* this, PlayState* play);
void EnEiyer_CircleUnderground(EnEiyer* this, PlayState* play);
void EnEiyer_Inactive(EnEiyer* this, PlayState* play);
void EnEiyer_Ambush(EnEiyer* this, PlayState* play);
void EnEiyer_Glide(EnEiyer* this, PlayState* play);
void EnEiyer_StartAttack(EnEiyer* this, PlayState* play);
void EnEiyer_DiveAttack(EnEiyer* this, PlayState* play);
void EnEiyer_Land(EnEiyer* this, PlayState* play);
void EnEiyer_Hurt(EnEiyer* this, PlayState* play);
void EnEiyer_Die(EnEiyer* this, PlayState* play);
void EnEiyer_Dead(EnEiyer* this, PlayState* play);
void EnEiyer_Stunned(EnEiyer* this, PlayState* play);

ActorProfile En_Eiyer_Profile = {
    0xc3a,        ACTORCAT_ENEMY,  FLAGS,          OBJECT_EI,    sizeof(EnEiyer),
    EnEiyer_Init, EnEiyer_Destroy, EnEiyer_Update, EnEiyer_Draw,
};

static ColliderCylinderInit sColliderCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_KNOCKBACK, 0x08 },
        { 0x00000019, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_HARD,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 27, 17, -10, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 45, 15, 100 };

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x1), DMG_ENTRY(2, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(0, 0x1), DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(2, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(4, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(8, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x0),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0xff, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 5, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 2500, ICHAIN_STOP),
};

void EnEiyer_Init(Actor* thisx, PlayState* play) {
    EnEiyer* this = (EnEiyer*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 600.0f, ActorShadow_DrawCircle, 65.0f);
    SkelAnime_Init(play, &this->skelanime, &gStingerSkel, &gStingerIdleAnim, this->jointTable, this->morphTable, 19);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sColliderCylinderInit);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    if (this->actor.params < 3) {
        if (Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_EIYER, this->actor.home.pos.x,
                               this->actor.home.pos.y, this->actor.home.pos.z, 0, this->actor.shape.rot.y + 0x4000, 0,
                               this->actor.params + 1) == NULL) {
            Actor_Kill(&this->actor);
            return;
        }

        if (this->actor.params == 0) {
            Actor* child = this->actor.child;
            s32 clonesSpawned;

            for (clonesSpawned = 0; clonesSpawned != 3; clonesSpawned++) {
                if (child == NULL) {
                    break;
                }
                child = child->child;
            }

            if (clonesSpawned != 3) {
                for (child = &this->actor; child != NULL; child = child->child) {
                    Actor_Kill(child);
                }
                return;
            } else {
                this->actor.child->parent = &this->actor;
                this->actor.child->child->parent = &this->actor;
                this->actor.child->child->child->parent = &this->actor;
            }
        }
    }

    if (this->actor.params == 0 || this->actor.params == 10) {
        EnEiyer_SetupAppearFromGround(this);
    } else {
        EnEiyer_SetupInactive(this);
    }
}

void EnEiyer_Destroy(Actor* thisx, PlayState* play) {
    EnEiyer* this = (EnEiyer*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
}

void EnEiyer_RotateAroundHome(EnEiyer* this) {
    this->actor.world.pos.x = Math_SinS(this->actor.world.rot.y) * 80.0f + this->actor.home.pos.x;
    this->actor.world.pos.z = Math_CosS(this->actor.world.rot.y) * 80.0f + this->actor.home.pos.z;
    this->actor.shape.rot.y = this->actor.world.rot.y + 0x4000;
}

void EnEiyer_SetupAppearFromGround(EnEiyer* this) {
    this->collider.elem.acDmgInfo.dmgFlags = DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT;
    Animation_PlayLoop(&this->skelanime, &gStingerIdleAnim);

    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.y = this->actor.home.pos.y - 40.0f;
    this->actor.world.pos.z = this->actor.home.pos.z;
    this->actor.velocity.y = 0.0f;
    this->actor.speed = 0.0f;

    if (this->actor.params != 0xA) {
        if (this->actor.params == 0) {
            this->actor.world.rot.y = Rand_ZeroOne() * 0x10000;
        } else {
            this->actor.world.rot.y = this->actor.parent->world.rot.y + this->actor.params * 0x4000;
        }
        EnEiyer_RotateAroundHome(this);
    } else {
        this->actor.world.rot.y = this->actor.shape.rot.y = Rand_ZeroOne() * 0x10000;
    }

    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_IGNORE_QUAKE);
    this->actor.shape.shadowScale = 0.0f;
    this->actor.shape.yOffset = 0.0f;
    this->actionFunc = EnEiyer_AppearFromGround;
}

void EnEiyer_SetupUnderground(EnEiyer* this) {
    if (this->actor.params == 0xA) {
        this->actor.speed = -0.5f;
        this->actionFunc = EnEiyer_WanderUnderground;
    } else {
        this->actionFunc = EnEiyer_CircleUnderground;
    }

    this->collider.base.acFlags |= AC_ON;
    this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
}

void EnEiyer_SetupInactive(EnEiyer* this) {
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = EnEiyer_Inactive;
}

void EnEiyer_SetupAmbush(EnEiyer* this, PlayState* play) {
    this->actor.speed = 0.0f;
    Animation_PlayOnce(&this->skelanime, &gStingerBackflipAnim);
    this->collider.elem.acDmgInfo.dmgFlags = DMG_DEFAULT;
    this->basePos = this->actor.world.pos;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.flags |= ACTOR_FLAG_IGNORE_QUAKE;
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.shape.shadowScale = 65.0f;
    this->actor.shape.yOffset = 600.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_OCTAROCK_JUMP);
    EffectSsGSplash_Spawn(play, &this->actor.world.pos, NULL, NULL, 1, 700);
    this->actionFunc = EnEiyer_Ambush;
}

void EnEiyer_SetupGlide(EnEiyer* this) {
    this->targetYaw = this->actor.shape.rot.y;
    this->basePos.y = (cosf(-M_PI / 8) * 5.0f) + this->actor.world.pos.y;
    Animation_MorphToLoop(&this->skelanime, &gStingerHitAnim, -5.0f);
    this->timer = 60;
    this->actionFunc = EnEiyer_Glide;
}

void EnEiyer_SetupStartAttack(EnEiyer* this) {
    this->actionFunc = EnEiyer_StartAttack;
}

void EnEiyer_SetupDiveAttack(EnEiyer* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->actor.velocity.y = 0.0f;
    this->basePos.y = player->actor.world.pos.y + 15.0f;
    this->collider.base.atFlags |= AT_ON;
    this->collider.base.atFlags &= ~AT_HIT;
    this->actionFunc = EnEiyer_DiveAttack;
}

void EnEiyer_SetupLand(EnEiyer* this) {
    Animation_MorphToPlayOnce(&this->skelanime, &gStingerDiveAnim, -3.0f);
    this->collider.base.atFlags &= ~AT_ON;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;

    this->timer = -1;
    this->actor.gravity = 0.0f;
    this->collider.dim.height = sColliderCylinderInit.dim.height;
    this->actionFunc = EnEiyer_Land;
}

void EnEiyer_SetupHurt(EnEiyer* this) {
    this->basePos.y = this->actor.world.pos.y;
    Animation_Change(&this->skelanime, &gStingerHitAnim, 2.0f, 0.0f, 0.0f, 0, -3.0f);
    this->timer = 40;
    this->actor.gravity = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.speed = 5.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 200, COLORFILTER_BUFFLAG_OPA, 40);
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnEiyer_Hurt;
}

void EnEiyer_SetupDie(EnEiyer* this) {
    this->timer = 20;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 200, COLORFILTER_BUFFLAG_OPA, 40);

    if (this->collider.elem.acDmgInfo.dmgFlags != (DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT)) {
        this->actor.speed = 6.0f;
        Animation_MorphToLoop(&this->skelanime, &gStingerHitAnim, -3.0f);
    } else {
        this->actor.speed -= 6.0f;
    }

    this->collider.elem.acDmgInfo.dmgFlags = DMG_DEFAULT;
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnEiyer_Die;
}

void EnEiyer_SetupDead(EnEiyer* this) {
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.colorFilterParams |= 0x2000;
    this->actionFunc = EnEiyer_Dead;
}

void EnEiyer_SetupStunned(EnEiyer* this) {
    Animation_Change(&this->skelanime, &gStingerPopOutAnim, 2.0f, 0.0f, 0.0f, 0, -8.0f);
    this->timer = 80;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = -1.0f;
    this->collider.dim.height = sColliderCylinderInit.dim.height + 8;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 200, COLORFILTER_BUFFLAG_OPA, 80);
    this->collider.base.atFlags &= ~AT_ON;
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->actionFunc = EnEiyer_Stunned;
}

void EnEiyer_AppearFromGround(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);

    if (Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.75f)) {
        EnEiyer_SetupUnderground(this);
    }
}

void EnEiyer_CheckPlayerCollision(EnEiyer* this, PlayState* play) {
    if (this->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        this->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        EnEiyer_SetupAmbush(this, play);
    }
}

void EnEiyer_CircleUnderground(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    this->actor.world.rot.y += -0x60;
    EnEiyer_RotateAroundHome(this);
    EnEiyer_CheckPlayerCollision(this, play);

    if (this->actor.params != 0 && ((EnEiyer*)this->actor.parent)->actionFunc != EnEiyer_CircleUnderground) {
        EnEiyer_SetupInactive(this);
    }
}

void EnEiyer_WanderUnderground(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);

    if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 100.0f) {
        this->targetYaw = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) + 0x8000;
    } else if (this->targetYaw == this->actor.world.rot.y && Rand_ZeroOne() > 0.99f) {
        this->targetYaw =
            this->actor.world.rot.y + (Rand_ZeroOne() < 0.5f ? -1 : 1) * (Rand_ZeroOne() * 0x2000 + 0x2000);
    }

    Math_ScaledStepToS(&this->actor.world.rot.y, this->targetYaw, 0xB6);
    EnEiyer_CheckPlayerCollision(this, play);
}

void EnEiyer_Inactive(EnEiyer* this, PlayState* play) {
    EnEiyer* parent;

    if (this->actor.home.pos.y - 50.0f < this->actor.world.pos.y) {
        this->actor.world.pos.y -= 0.5f;
    }

    parent = (EnEiyer*)this->actor.parent;
    if (parent->actionFunc == EnEiyer_Dead) {
        Actor_Kill(&this->actor);
    } else if (parent->actionFunc == EnEiyer_AppearFromGround) {
        EnEiyer_SetupAppearFromGround(this);
    }
}

void EnEiyer_Ambush(EnEiyer* this, PlayState* play) {
    s32 animFinished;
    f32 curFrame;
    f32 xzOffset;
    s32 bgId;

    animFinished = SkelAnime_Update(&this->skelanime);
    curFrame = this->skelanime.curFrame;

    if (this->skelanime.curFrame < 12.0f) {
        this->actor.world.pos.y = ((1.0f - cosf((0.996f * M_PI / 12.0f) * curFrame)) * 40.0f) + this->actor.home.pos.y;
        xzOffset = sinf((0.996f * M_PI / 12.0f) * curFrame) * -40.0f;
        this->actor.world.pos.x = (Math_SinS(this->actor.shape.rot.y) * xzOffset) + this->basePos.x;
        this->actor.world.pos.z = (Math_CosS(this->actor.shape.rot.y) * xzOffset) + this->basePos.z;
    } else {
        Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 80.0f, 0.5f);
        this->actor.speed = 0.8f;
    }

    if (animFinished) {
        this->collider.base.acFlags |= AC_ON;
        EnEiyer_SetupGlide(this);
    } else {
        this->actor.floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &this->actor.floorPoly, &bgId, &this->actor,
                                                             &this->actor.world.pos);
    }
}

void EnEiyer_Glide(EnEiyer* this, PlayState* play) {
    f32 curFrame;
    s16 yawChange;

    SkelAnime_Update(&this->skelanime);

    if (this->timer != 0) {
        this->timer--;
    }

    curFrame = this->skelanime.curFrame;
    Math_ApproachF(&this->basePos.y, this->actor.floorHeight + 80.0f + 5.0f, 0.3f, this->actor.speed);
    this->actor.world.pos.y = this->basePos.y - cosf((curFrame - 5.0f) * (M_PI / 40)) * 5.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&this->actor.speed, 1.0f, 0.03f);
    } else {
        Math_StepToF(&this->actor.speed, 1.5f, 0.03f);
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->targetYaw = this->actor.wallYaw;
    }

    if (Math_ScaledStepToS(&this->actor.world.rot.y, this->targetYaw, 0xB6)) {
        if (this->timer != 0 || Rand_ZeroOne() > 0.05f) {
            this->actor.world.rot.y += 0x100;
        } else {
            yawChange = Rand_S16Offset(0x2000, 0x2000);
            this->targetYaw = (Rand_ZeroOne() < 0.5f ? -1 : 1) * yawChange + this->actor.world.rot.y;
        }
    }

    if (this->timer == 0 && this->actor.yDistanceFromLink < 0.0f && this->actor.xzDistToPlayer < 120.0f) {
        EnEiyer_SetupStartAttack(this);
    }

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_StartAttack(EnEiyer* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f focus;

    SkelAnime_Update(&this->skelanime);

    if (this->actor.shape.rot.x > 0 && this->actor.shape.rot.x < 0x8000) {
        focus.x = player->actor.world.pos.x;
        focus.y = player->actor.world.pos.y + 20.0f;
        focus.z = player->actor.world.pos.z;

        if (Math_ScaledStepToS(&this->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&this->actor, &focus), 0x1000)) {
            EnEiyer_SetupDiveAttack(this, play);
        }
    } else {
        this->actor.shape.rot.x -= 0x1000;
    }

    this->actor.world.rot.x = -this->actor.shape.rot.x;
    Math_StepToF(&this->actor.speed, 5.0f, 0.3f);
    Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 2, 0x71C);
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_DiveAttack(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    this->actor.speed *= 1.1f;

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        EnEiyer_SetupLand(this);
    }

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~(AT_ON | AT_HIT);
    }

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_Land(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);
    Math_ScaledStepToS(&this->actor.world.rot.x, -0x4000, 0x450);
    Math_StepToF(&this->actor.speed, 7.0f, 1.0f);

    if (this->timer == -1) {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            this->timer = 10;
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_OCTAROCK_SINK);

            if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                EffectSsGSplash_Spawn(play, &this->actor.world.pos, NULL, NULL, 1, 700);
            }
        }
    } else {
        if (this->timer != 0) {
            this->timer--;
        }

        if (this->timer == 0) {
            this->actor.shape.rot.x = 0;
            this->actor.world.rot.x = 0;
            EnEiyer_SetupAppearFromGround(this);
        }
    }
}

void EnEiyer_Hurt(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);

    if (this->timer != 0) {
        this->timer--;
    }

    Math_ApproachF(&this->basePos.y, this->actor.floorHeight + 80.0f + 5.0f, 0.5f, this->actor.speed);
    this->actor.world.pos.y = this->basePos.y - 5.0f;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->targetYaw = this->actor.wallYaw;
    } else {
        this->targetYaw = this->actor.yawTowardsPlayer + 0x8000;
    }

    Math_ScaledStepToS(&this->actor.world.rot.y, this->targetYaw, 0x38E);
    Math_ScaledStepToS(&this->actor.shape.rot.x, 0, 0x200);
    this->actor.shape.rot.z = sinf(this->timer * (M_PI / 5)) * 5120.0f;

    if (this->timer == 0) {
        this->actor.shape.rot.x = 0;
        this->actor.shape.rot.z = 0;
        this->collider.base.acFlags |= AC_ON;
        EnEiyer_SetupGlide(this);
    }
    this->actor.world.rot.x = -this->actor.shape.rot.x;
}

void EnEiyer_Die(EnEiyer* this, PlayState* play) {
    SkelAnime_Update(&this->skelanime);

    if (this->actor.speed > 0.0f) {
        Math_ScaledStepToS(&this->actor.shape.rot.x, -0x4000, 0x400);
    } else {
        Math_ScaledStepToS(&this->actor.shape.rot.x, 0x4000, 0x400);
    }

    this->actor.shape.rot.z += 0x1000;

    if (this->timer != 0) {
        this->timer--;
    }

    this->actor.world.rot.x = -this->actor.shape.rot.x;

    if (this->timer == 0 || this->actor.bgCheckFlags & BGCHECKFLAG_CEILING) {
        EnEiyer_SetupDead(this);
    }
}

void EnEiyer_Dead(EnEiyer* this, PlayState* play) {
    this->actor.shape.shadowAlpha = CLAMP_MIN((s16)(this->actor.shape.shadowAlpha - 5), 0);
    this->actor.world.pos.y -= 2.0f;

    if (this->actor.shape.shadowAlpha == 0) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_5, false));
        Actor_Kill(&this->actor);
    }
}

void EnEiyer_Stunned(EnEiyer* this, PlayState* play) {
    if (this->timer != 0) {
        this->timer--;
    }

    Math_ScaledStepToS(&this->actor.shape.rot.x, 0, 0x200);
    SkelAnime_Update(&this->skelanime);

    if (Animation_OnFrame(&this->skelanime, 0.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_EIER_FLUTTER);
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
    }

    if (this->timer == 0) {
        this->actor.gravity = 0.0f;
        this->actor.velocity.y = 0.0f;
        this->collider.dim.height = sColliderCylinderInit.dim.height;
        EnEiyer_SetupGlide(this);
    }
}

void EnEiyer_UpdateDamage(EnEiyer* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.elem, true);

        if (this->actor.colChkInfo.damageEffect != 0 || this->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&this->actor) == 0) {
                Enemy_StartFinishingBlow(play, &this->actor);
                Actor_PlaySfx(&this->actor, NA_SE_EN_EIER_DEAD);
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            }

            if (this->collider.elem.acDmgInfo.dmgFlags == (DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT)) {
                if (this->actor.colChkInfo.damage == 0) {
                    EnEiyer_SetupAmbush(this, play);
                } else {
                    EnEiyer_SetupDie(this);
                }
            } else if (this->actor.colChkInfo.damageEffect == 1) {
                if (this->actionFunc != EnEiyer_Stunned) {
                    EnEiyer_SetupStunned(this);
                }
            } else if (this->actor.colChkInfo.health != 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_EIER_DAMAGE);
                EnEiyer_SetupHurt(this);
            } else {
                this->collider.dim.height = sColliderCylinderInit.dim.height;
                EnEiyer_SetupDie(this);
            }
        }
    }
}

void EnEiyer_Update(Actor* thisx, PlayState* play) {
    EnEiyer* this = (EnEiyer*)thisx;

    EnEiyer_UpdateDamage(this, play);
    this->actionFunc(this, play);

    if (this->actor.world.rot.x == 0 || this->actionFunc == EnEiyer_Stunned) {
        Actor_MoveXZGravity(&this->actor);
    } else {
        Actor_MoveXYZ(&this->actor);
    }

    if (this->actionFunc == EnEiyer_Glide || this->actionFunc == EnEiyer_DiveAttack ||
        this->actionFunc == EnEiyer_Stunned || this->actionFunc == EnEiyer_Die || this->actionFunc == EnEiyer_Hurt ||
        (this->actionFunc == EnEiyer_Land && this->timer == -1)) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 27.0f, 30.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
    }

    if (this->actor.params == 0xA ||
        (this->actionFunc != EnEiyer_AppearFromGround && this->actionFunc != EnEiyer_CircleUnderground)) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    if (this->actor.params == 0 || this->actor.params == 0xA) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        if (this->collider.base.atFlags & AT_ON) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
        if (this->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
        if (this->actionFunc != EnEiyer_Ambush) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }
    }

    if (this->actor.flags & ACTOR_FLAG_ATTENTION_ENABLED) {
        this->actor.focus.pos.x = this->actor.world.pos.x + Math_SinS(this->actor.shape.rot.y) * 12.5f;
        this->actor.focus.pos.z = this->actor.world.pos.z + Math_CosS(this->actor.shape.rot.y) * 12.5f;
        this->actor.focus.pos.y = this->actor.world.pos.y;
    }
}

s32 EnEiyer_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                             Gfx** gfx) {
    EnEiyer* this = (EnEiyer*)thisx;

    if (limbIndex == 1) {
        pos->z += 2500.0f;
    }

    if (this->collider.elem.acDmgInfo.dmgFlags == (DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT) && limbIndex != 9 &&
        limbIndex != 10) {
        *dList = NULL;
    }
    return 0;
}

void EnEiyer_Draw(Actor* thisx, PlayState* play) {
    EnEiyer* this = (EnEiyer*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    if (this->actionFunc != EnEiyer_Dead) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, ACTOR_SETUP_OPA_DL);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);

        POLY_OPA_DISP = SkelAnime_Draw(play, this->skelanime.skeleton, this->skelanime.jointTable,
                                       EnEiyer_OverrideLimbDraw, NULL, this, POLY_OPA_DISP);
    } else {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, gActorSetupXluDL);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, this->actor.shape.shadowAlpha);

        POLY_XLU_DISP = SkelAnime_Draw(play, this->skelanime.skeleton, this->skelanime.jointTable,
                                       EnEiyer_OverrideLimbDraw, NULL, this, POLY_XLU_DISP);
    }
    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x3a, En_Eiyer_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_INIT, EnEiyer_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_DESTROY, EnEiyer_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_UPDATE, EnEiyer_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_DRAW, EnEiyer_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_PROFILE, En_Eiyer_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x3a, 0x26, 0x3c860de1 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_EIYER_ORIGIN, kNativePortOrigin);
