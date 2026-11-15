#include "oot_compat.h"
#include "types/en_anubice.h"
#undef ACTOR_BG_HIDAN_CURTAIN
#define ACTOR_BG_HIDAN_CURTAIN 0x0049
#undef ACTOR_EN_ANUBICE
#define ACTOR_EN_ANUBICE 0x00e0
#undef ACTOR_EN_ANUBICE_FIRE
#define ACTOR_EN_ANUBICE_FIRE 0x00e1
#undef OBJECT_ANUBICE
#define OBJECT_ANUBICE 0x10d6
#undef NA_SE_EN_NUTS_CUTBODY
#define NA_SE_EN_NUTS_CUTBODY 0x393b
#undef NA_SE_EN_ANUBIS_DEAD
#define NA_SE_EN_ANUBIS_DEAD 0x399f
#define gAnubiceSkel (*(SkeletonHeader*)0x06003990)
#define gAnubiceFallDownAnim (*(AnimationHeader*)0x06000348)
#define gAnubiceAttackingAnim (*(AnimationHeader*)0x0600078c)
#define gAnubiceIdleAnim (*(AnimationHeader*)0x06000f74)
#define gAnubiceEyesDL (*((Gfx (*)[1])0x06003468))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ChangeCategory
#define Actor_ChangeCategory FOREIGN_ENEMY_API_CATEGORY
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 yOffset))0x800b675c)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_Spawn
#define Actor_Spawn FOREIGN_ENEMY_API_SPAWN
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_UpdatePos
#define Actor_UpdatePos ((void (*)(Actor* actor))0x800b69ac)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x80134748)
#undef BgCheck_SphVsFirstPoly
#define BgCheck_SphVsFirstPoly ((s32 (*)(CollisionContext* colCtx, Vec3f* center, f32 radius))0x800c5a20)
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
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsEnFire_SpawnVec3f
#define EffectSsEnFire_SpawnVec3f ((void (*)(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart))0x800b2df4)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 fraction, f32 step))0x800ffe68)
#undef Math_FAtan2F
#define Math_FAtan2F ((f32 (*)(f32 y, f32 x))0x80086b30)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_RotateX
#define Matrix_RotateX FOREIGN_ENEMY_API_MATRIX_ROTATE_XF
#undef Matrix_RotateY
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(PlayState* play, void** skeleton, Vec3s* jointTable, void* overrideLimbDraw, void* postLimbDraw, void* arg))0x80133b3c)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136a7c)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)

#include "types/en_anubice_tag.h"
#include "types/bg_hidan_curtain.h"
void comboAnubisUpdateBg(PlayState*, Actor*, float, float, float, s32);

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnAnubice_Init(Actor* thisx, PlayState* play);
void EnAnubice_Destroy(Actor* thisx, PlayState* play);
void EnAnubice_Update(Actor* thisx, PlayState* play);
void EnAnubice_Draw(Actor* thisx, PlayState* play);

void EnAnubice_FindFlameCircles(EnAnubice* this, PlayState* play);
void EnAnubice_SetupIdle(EnAnubice* this, PlayState* play);
void EnAnubice_Idle(EnAnubice* this, PlayState* play);
void EnAnubice_GoToHome(EnAnubice* this, PlayState* play);
void EnAnubice_SetupShootFireball(EnAnubice* this, PlayState* play);
void EnAnubice_ShootFireball(EnAnubice* this, PlayState* play);
void EnAnubice_Die(EnAnubice* this, PlayState* play);

ActorProfile En_Anubice_Profile = {
    0xce0,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubice),
    EnAnubice_Init,
    EnAnubice_Destroy,
    EnAnubice_Update,
    EnAnubice_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 29, 103, 0, { 0, 0, 0 } },
};

typedef enum AnubiceDamageReaction {
    ANUBICE_DMG_REACT_NONE,
    ANUBICE_DMG_REACT_FIRE = 2,
    ANUBICE_DMG_REACT_0xF = 0xF
} AnubiceDamageReaction;

static DamageTable sDamageTable[] = { { {
    DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(1, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(2, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(2, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(6, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(2, ANUBICE_DMG_REACT_FIRE),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(3, ANUBICE_DMG_REACT_FIRE),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(6, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(2, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(12, ANUBICE_DMG_REACT_0xF),
    DMG_ENTRY(4, ANUBICE_DMG_REACT_0xF),  DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE),
    DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE), DMG_ENTRY(0, ANUBICE_DMG_REACT_NONE),
} } };

void EnAnubice_Hover(EnAnubice* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->hoverVelocityTimer += 1500.0f;
    this->targetHeight = player->actor.world.pos.y + this->playerHeightOffset;
    Math_ApproachF(&this->actor.world.pos.y, this->targetHeight, 0.1f, 10.0f);
    Math_ApproachF(&this->playerHeightOffset, 10.0f, 0.1f, 0.5f);
    this->actor.velocity.y = Math_SinS(this->hoverVelocityTimer);
}

void EnAnubice_AimFireball(EnAnubice* this, PlayState* play) {
    f32 xzDist;
    f32 x;
    f32 y;
    f32 z;
    Player* player = GET_PLAYER(play);

    x = player->actor.world.pos.x - this->headPos.x;
    y = player->actor.world.pos.y + 10.0f - this->headPos.y;
    z = player->actor.world.pos.z - this->headPos.z;
    xzDist = sqrtf(SQ(x) + SQ(z));

    this->fireballRot.x = -RAD_TO_BINANG(Math_FAtan2F(y, xzDist));
    this->fireballRot.y = RAD_TO_BINANG(Math_FAtan2F(x, z));
}

void EnAnubice_Init(Actor* thisx, PlayState* play) {
    EnAnubice* this = (EnAnubice*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    SkelAnime_Init(play, &this->skelAnime, &gAnubiceSkel, &gAnubiceIdleAnim, this->jointTable, this->morphTable,
                   ANUBICE_LIMB_MAX);

    this->actor.hintId = 0xff;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);

    Actor_SetScale(&this->actor, 0.015f);

    this->actor.colChkInfo.damageTable = sDamageTable;
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.shape.yOffset = -4230.0f;
    this->focusHeightOffset = 0.0f;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->home = this->actor.world.pos;
    this->actor.attentionRangeType = ATTENTION_RANGE_3;
    this->actionFunc = EnAnubice_FindFlameCircles;
}

void EnAnubice_Destroy(Actor* thisx, PlayState* play) {
    EnAnubice* this = (EnAnubice*)thisx;
    Collider_DestroyCylinder(play, &this->collider);

    Actor* parent = play->actorCtx.actors[ACTORCAT_ENEMY].first;
    while (parent && parent != this->actor.parent) {
        parent = parent->next;
    }
    if (parent && parent->id == 0xcf6 && parent->child == &this->actor) {
        ((EnAnubiceTag*)parent)->anubis = NULL;
        parent->child = NULL;
    }
}

void EnAnubice_FindFlameCircles(EnAnubice* this, PlayState* play) {
    Actor* currentProp;
    s32 flameCirclesFound;

    if (this->isMirroringPlayer) {
        if (!this->hasSearchedForFlameCircles) {
            flameCirclesFound = 0;
            currentProp = play->actorCtx.actors[ACTORCAT_PROP].first;
            while (currentProp != NULL) {
                if (currentProp->id != 0xc49) {
                    currentProp = currentProp->next;
                } else {
                    this->flameCircles[flameCirclesFound] = (BgHidanCurtain*)currentProp;
                    if (flameCirclesFound < ARRAY_COUNT(this->flameCircles) - 1) {
                        flameCirclesFound++;
                    }
                    currentProp = currentProp->next;
                }
            }
            this->hasSearchedForFlameCircles = true;
        }
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actionFunc = EnAnubice_SetupIdle;
    }
}

void EnAnubice_SetupIdle(EnAnubice* this, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceIdleAnim);

    Animation_Change(&this->skelAnime, &gAnubiceIdleAnim, 1.0f, 0.0f, (s16)lastFrame, ANIMMODE_LOOP, -10.0f);

    this->actionFunc = EnAnubice_Idle;
    this->actor.velocity.x = this->actor.velocity.z = this->actor.gravity = 0.0f;
}

void EnAnubice_Idle(EnAnubice* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.shape.yOffset, 0.5f, 300.0f);
    Math_ApproachF(&this->focusHeightOffset, 70.0f, 0.5f, 5.0f);

    if (!this->isKnockedback) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 3000, 0);
    }

    if (this->actor.shape.yOffset > -2.0f) {
        this->actor.shape.yOffset = 0.0f;

        if (player->meleeWeaponState != 0) {
            this->actionFunc = EnAnubice_SetupShootFireball;
        } else if (this->isPlayerOutOfRange) {
            this->actor.velocity.y = 0.0f;
            this->actor.gravity = -1.0f;
            this->actionFunc = EnAnubice_GoToHome;
        }
    }
}

void EnAnubice_GoToHome(EnAnubice* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_ApproachF(&this->actor.shape.yOffset, -4230.0f, 0.5f, 300.0f);
    Math_ApproachZeroF(&this->focusHeightOffset, 0.5f, 5.0f);

    Actor* actor = &this->actor;
    if (!this->isKnockedback)
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 5, 3000, 0);

    f32 dx = this->home.x - actor->world.pos.x;
    f32 dz = this->home.z - actor->world.pos.z;
    if (fabsf(dx) > 3.f && fabsf(dz) > 3.f) {
        f32 step = 8.f / sqrtf(dx * dx + dz * dz);
        Vec3f from = actor->world.pos;
        from.y += 10.f;
        Vec3f to = { from.x + dx * step, from.y, from.z + dz * step };
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (!BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId)) {
            Vec3f probe = { to.x, to.y + 30.f, to.z };
            f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
            if (poly && fabsf(floor - actor->floorHeight) <= 30.f) {
                actor->world.pos.x = to.x;
                actor->world.pos.z = to.z;
                return;
            }
        }
    }

    if (this->actor.shape.yOffset < -4220.0f) {
        this->actor.world.pos = this->actor.prevPos = this->home;
        this->actor.velocity.y = 0.0f;
        this->actor.shape.yOffset = -4230.0f;
        this->isMirroringPlayer = this->isPlayerOutOfRange = false;
        this->actionFunc = EnAnubice_FindFlameCircles;
        this->actor.gravity = 0.0f;
    }
}

void EnAnubice_SetupShootFireball(EnAnubice* this, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceAttackingAnim);

    this->animLastFrame = lastFrame;
    Animation_Change(&this->skelAnime, &gAnubiceAttackingAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -10.0f);
    this->actionFunc = EnAnubice_ShootFireball;
    this->actor.velocity.x = this->actor.velocity.z = 0.0f;
}

void EnAnubice_ShootFireball(EnAnubice* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    SkelAnime_Update(&this->skelAnime);

    if (!this->isKnockedback) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 3000, 0);
    }

    EnAnubice_AimFireball(this, play);

    if (curFrame == 12.0f) {
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ANUBICE_FIRE, this->headPos.x, this->headPos.y + 15.0f,
                    this->headPos.z, this->fireballRot.x, this->fireballRot.y, 0, 0);
    }

    if (this->animLastFrame <= curFrame) {
        this->actionFunc = EnAnubice_SetupIdle;
    }
}

void EnAnubice_SetupDie(EnAnubice* this, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceFallDownAnim);

    this->animLastFrame = lastFrame;
    Animation_Change(&this->skelAnime, &gAnubiceFallDownAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -20.0f);

    this->isNearWall = false;
    this->fallTargetYaw = 0;
    this->deathTimer = 20;
    this->actor.velocity.x = this->actor.velocity.z = 0.0f;
    this->actor.gravity = -1.0f;

    if (BgCheck_SphVsFirstPoly(&play->colCtx, &this->headPos, 70.0f)) {
        this->isNearWall = true;
        this->fallTargetYaw = this->actor.shape.rot.x - 0x7F00;
    }

    this->actionFunc = EnAnubice_Die;
}

void EnAnubice_Die(EnAnubice* this, PlayState* play) {
    f32 curFrame;
    f32 rotX;
    Vec3f baseFireEffectPos = { 0.0f, 0.0f, 0.0f };
    Vec3f rotatedFireEffectPos = { 0.0f, 0.0f, 0.0f };

    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.shape.shadowScale, 0.4f, 0.25f);

    if (this->isNearWall) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->fallTargetYaw, 1, 10000, 0);
        if (fabsf(this->actor.shape.rot.y - this->fallTargetYaw) < 100.0f) {
            this->isNearWall = false;
        }
    }

    curFrame = this->skelAnime.curFrame;
    rotX = curFrame * -3000.0f;
    rotX = CLAMP_MIN(rotX, -11000.0f);

    Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y), MTXMODE_NEW);
    Matrix_RotateX(BINANG_TO_RAD_ALT(rotX), MTXMODE_APPLY);
    baseFireEffectPos.y = Rand_CenteredFloat(10.0f) + 30.0f;
    Matrix_MultVec3f(&baseFireEffectPos, &rotatedFireEffectPos);
    rotatedFireEffectPos.x += this->actor.world.pos.x + Rand_CenteredFloat(40.0f);
    rotatedFireEffectPos.y += this->actor.world.pos.y + Rand_CenteredFloat(40.0f);
    rotatedFireEffectPos.z += this->actor.world.pos.z + Rand_CenteredFloat(30.0f);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 128, COLORFILTER_BUFFLAG_OPA, 8);
    EffectSsEnFire_SpawnVec3f(play, &this->actor, &rotatedFireEffectPos, 100, 0, 0, -1);

    if ((this->animLastFrame <= curFrame) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        Math_ApproachF(&this->actor.shape.yOffset, -4230.0f, 0.5f, 300.0f);
        if (this->actor.shape.yOffset < -2000.0f) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_12, false));
            Actor_Kill(&this->actor);
        }
    }
}

void EnAnubice_Update(Actor* thisx, PlayState* play) {
    f32 zero;
    BgHidanCurtain* flameCircle;
    s32 i;
    Vec3f baseKnockbackVelocity;
    Vec3f rotatedKnockbackVelocity;
    EnAnubice* this = (EnAnubice*)thisx;

    if ((this->actionFunc != EnAnubice_SetupDie) && (this->actionFunc != EnAnubice_Die) &&
        (this->actor.shape.yOffset == 0.0f)) {
        EnAnubice_Hover(this, play);
        for (i = 0; i < ARRAY_COUNT(this->flameCircles); i++) {
            flameCircle = this->flameCircles[i];

            if ((flameCircle != NULL) && (fabsf(flameCircle->actor.world.pos.x - this->actor.world.pos.x) < 60.0f) &&
                (fabsf(this->flameCircles[i]->actor.world.pos.z - this->actor.world.pos.z) < 60.0f) &&
                (flameCircle->timer != 0)) {
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                Enemy_StartFinishingBlow(play, &this->actor);
                Actor_PlaySfx(&this->actor, NA_SE_EN_ANUBIS_DEAD);
                this->actionFunc = EnAnubice_SetupDie;
                return;
            }
        }

        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->actor.colChkInfo.damageEffect == ANUBICE_DMG_REACT_FIRE) {
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                Enemy_StartFinishingBlow(play, &this->actor);
                Actor_PlaySfx(&this->actor, NA_SE_EN_ANUBIS_DEAD);
                this->actionFunc = EnAnubice_SetupDie;
                return;
            }

            if (!this->isKnockedback) {
                this->knockbackTimer = 10;
                this->isKnockedback = true;

                baseKnockbackVelocity.x = 0.0f;
                baseKnockbackVelocity.y = 0.0f;
                baseKnockbackVelocity.z = -10.0f;
                rotatedKnockbackVelocity.x = 0.0f;
                rotatedKnockbackVelocity.y = 0.0f;
                rotatedKnockbackVelocity.z = 0.0f;

                Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.shape.rot.y), MTXMODE_NEW);
                Matrix_MultVec3f(&baseKnockbackVelocity, &rotatedKnockbackVelocity);

                this->actor.velocity.x = rotatedKnockbackVelocity.x;
                this->actor.velocity.z = rotatedKnockbackVelocity.z;
                this->knockbackRecoveryVelocity.x = -rotatedKnockbackVelocity.x;
                this->knockbackRecoveryVelocity.z = -rotatedKnockbackVelocity.z;

                Actor_PlaySfx(&this->actor, NA_SE_EN_NUTS_CUTBODY);
            }
        }

        if (this->isKnockedback) {
            this->actor.shape.rot.y += 6500;
            Math_ApproachF(&this->actor.velocity.x, this->knockbackRecoveryVelocity.x, 0.3f, 1.0f);
            Math_ApproachF(&this->actor.velocity.z, this->knockbackRecoveryVelocity.z, 0.3f, 1.0f);

            zero = 0.0f;
            if (zero) {
            }

            if (this->knockbackTimer == 0) {
                this->actor.velocity.x = this->actor.velocity.z = 0.0f;
                this->knockbackRecoveryVelocity.x = this->knockbackRecoveryVelocity.z = 0.0f;
                this->isKnockedback = false;
            }
        }
    }

    this->timeAlive++;

    if (this->knockbackTimer != 0) {
        this->knockbackTimer--;
    }

    if (this->deathTimer != 0) {
        this->deathTimer--;
    }

    this->actionFunc(this, play);

    this->actor.velocity.y += this->actor.gravity;
    Actor_UpdatePos(&this->actor);

    if (!this->isPlayerOutOfRange) {
        comboAnubisUpdateBg(play, &this->actor, 5.0f, 5.0f, 10.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    } else {
        comboAnubisUpdateBg(play, &this->actor, 5.0f, 5.0f, 10.0f,
                            UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 | UPDBGCHECKINFO_FLAG_4);
    }

    if ((this->actionFunc != EnAnubice_SetupDie) && (this->actionFunc != EnAnubice_Die)) {
        Actor_SetFocus(&this->actor, this->focusHeightOffset);
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

        if (!this->isKnockedback && (this->actor.shape.yOffset == 0.0f)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

s32 EnAnubice_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnAnubice* this = (EnAnubice*)thisx;

    if (limbIndex == ANUBICE_LIMB_HEAD) {
        rot->z += this->headPitch;
    }

    return false;
}

void EnAnubice_PostLimbDraw(struct PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnAnubice* this = (EnAnubice*)thisx;
    Vec3f pos = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == ANUBICE_LIMB_HEAD) {
        OPEN_DISPS(play->state.gfxCtx);

        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gAnubiceEyesDL);
        Matrix_MultVec3f(&pos, &this->headPos);

        CLOSE_DISPS();
    }
}

void EnAnubice_Draw(Actor* thisx, PlayState* play) {
    EnAnubice* this = (EnAnubice*)thisx;

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnAnubice_OverrideLimbDraw,
                      EnAnubice_PostLimbDraw, this);
}

OVL_INFO_NATIVE_ENEMY(0xe0, En_Anubice_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_INIT, EnAnubice_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_DESTROY, EnAnubice_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_UPDATE, EnAnubice_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_DRAW, EnAnubice_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_PROFILE, En_Anubice_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xe0, 0xd6, 0x8c91de7f };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_ORIGIN, kNativePortOrigin);
