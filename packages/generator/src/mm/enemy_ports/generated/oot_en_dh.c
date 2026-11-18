#include "oot_compat.h"
#include "types/en_dh.h"
#undef ACTOR_EN_DH
#define ACTOR_EN_DH 0x00a4
#undef OBJECT_DH
#define OBJECT_DH 0x10a6
#undef NA_SE_EN_RIZA_DOWN
#define NA_SE_EN_RIZA_DOWN 0x387a
#undef NA_SE_EN_DEADHAND_BITE
#define NA_SE_EN_DEADHAND_BITE 0x3900
#undef NA_SE_EN_DEADHAND_WALK
#define NA_SE_EN_DEADHAND_WALK 0x3901
#undef NA_SE_EN_DEADHAND_LAUGH
#define NA_SE_EN_DEADHAND_LAUGH 0x39a7
#undef NA_SE_EN_DEADHAND_HIDE
#define NA_SE_EN_DEADHAND_HIDE 0x39a8
#undef NA_SE_EN_DEADHAND_DAMAGE
#define NA_SE_EN_DEADHAND_DAMAGE 0x39a9
#undef NA_SE_EN_DEADHAND_DEAD
#define NA_SE_EN_DEADHAND_DEAD 0x39ab
#undef DMG_DEFAULT
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)
#define object_dh_Anim_001A3C (*(AnimationHeader*)0x06001a3c)
#define object_dh_Anim_002148 (*(AnimationHeader*)0x06002148)
#define object_dh_Anim_0032BC (*(AnimationHeader*)0x060032bc)
#define object_dh_Anim_00375C (*(AnimationHeader*)0x0600375c)
#define object_dh_Anim_003A8C (*(AnimationHeader*)0x06003a8c)
#define object_dh_Anim_003D6C (*(AnimationHeader*)0x06003d6c)
#define object_dh_Anim_004658 (*(AnimationHeader*)0x06004658)
#define object_dh_Anim_005880 (*(AnimationHeader*)0x06005880)
#define object_dh_Skel_007E88 (*(FlexSkeletonHeader*)0x06007e88)
#define object_dh_DL_007FC0 (*((Gfx (*)[1])0x06007fc0))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_ChangeCategory
#define Actor_ChangeCategory FOREIGN_ENEMY_API_CATEGORY
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 maxAngle))0x800b73e0)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x80134748)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137594)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137430)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x800e7fdc)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsHahen_Spawn
#define EffectSsHahen_Spawn FOREIGN_ENEMY_API_DEFAULT_FRAGMENT
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TwoTexScroll
#define Gfx_TwoTexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2))0x8012cbd0)
#undef Item_DropCollectibleRandom
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
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawFlex
#define SkelAnime_DrawFlex ((Gfx* (*)(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, void* overrideLimbDraw, void* postLimbDraw, void* arg, Gfx* gfx))0x80134dbc)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136b30)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef func_800F5ACC
#define func_800F5ACC FOREIGN_ENEMY_API_OOT_BGM_STORE

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_UPDATE_CULLING_DISABLED)

typedef enum EnDhAction { DH_WAIT, DH_RETREAT, DH_BURROW, DH_WALK, DH_ATTACK, DH_DEATH, DH_DAMAGE } EnDhAction;

Actor* FOREIGN_ENEMY_API_SPAWN_CHILD(ActorContext*, Actor*, PlayState*, s16, f32, f32, f32, s16, s16, s16, s32);
#define DEAD_HAND_SPAWN_ARM FOREIGN_ENEMY_API_SPAWN_CHILD
#include "../../../common/ovl/dead_hand_placement.c"
#undef DEAD_HAND_SPAWN_ARM

void EnDh_Init(Actor* thisx, PlayState* play);
void EnDh_Destroy(Actor* thisx, PlayState* play);
void EnDh_Update(Actor* thisx, PlayState* play);
void EnDh_Draw(Actor* thisx, PlayState* play);

void EnDh_SetupWait(EnDh* this);
void EnDh_SetupWalk(EnDh* this);
void EnDh_SetupAttack(EnDh* this);
void EnDh_SetupBurrow(EnDh* this);

void EnDh_Wait(EnDh* this, PlayState* play);
void EnDh_Walk(EnDh* this, PlayState* play);
void EnDh_Retreat(EnDh* this, PlayState* play);
void EnDh_Attack(EnDh* this, PlayState* play);
void EnDh_Burrow(EnDh* this, PlayState* play);
void EnDh_Damage(EnDh* this, PlayState* play);
void EnDh_Death(EnDh* this, PlayState* play);

ActorProfile En_Dh_Profile = {
    0xca4, ACTORCAT_ENEMY, FLAGS, OBJECT_DH, sizeof(EnDh), EnDh_Init, EnDh_Destroy, EnDh_Update, EnDh_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000008, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 35, 70, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON | OCELEM_UNK3,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static DamageTable D_809EC620 = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(8, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0xff, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3500, ICHAIN_STOP),
};

void EnDh_SetupAction(EnDh* this, EnDhActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnDh_Init(Actor* thisx, PlayState* play) {
    EnDh* this = (EnDh*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.colChkInfo.damageTable = &D_809EC620;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_dh_Skel_007E88, &object_dh_Anim_005880, this->jointTable,
                       this->limbRotTable, 16);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 64.0f);
    this->actor.params = ENDH_WAIT_UNDERGROUND;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 20;
    this->alpha = this->unk_258 = 255;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Collider_InitCylinder(play, &this->collider1);
    Collider_SetCylinder(play, &this->collider1, &this->actor, &sCylinderInit);
    Collider_InitJntSph(play, &this->colliderJntSph);
    Collider_SetJntSph(play, &this->colliderJntSph, &this->actor, &sJntSphInit, this->colliderJntSphElements);
    EnDh_SetupWait(this);
    this->handsPlaced = DeadHand_PlaceHands(thisx, play);
}

void EnDh_Destroy(Actor* thisx, PlayState* play) {
    EnDh* this = (EnDh*)thisx;

    Collider_DestroyCylinder(play, &this->collider1);
    Collider_DestroyJntSph(play, &this->colliderJntSph);
}

void EnDh_SpawnDebris(PlayState* play, EnDh* this, Vec3f* spawnPos, f32 spread, s32 arg4, f32 accelXZ, f32 scale) {
    Vec3f pos;
    Vec3f vel = { 0.0f, 8.0f, 0.0f };
    Vec3f accel = { 0.0f, -1.5f, 0.0f };
    f32 spreadAngle;
    f32 scaleMod;

    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = this->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * spread) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * spread) + spawnPos->z;
    accel.x = (Rand_ZeroOne() - 0.5f) * accelXZ;
    accel.z = (Rand_ZeroOne() - 0.5f) * accelXZ;
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scaleMod = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(play, &pos, &vel, &accel, arg4, scaleMod * scale, -1, 10, NULL);
}

void EnDh_SetupWait(EnDh* this) {
    Animation_PlayLoop(&this->skelAnime, &object_dh_Anim_003A8C);
    this->curAction = DH_WAIT;
    this->actor.world.pos = this->actor.home.pos;
    this->actor.shape.yOffset = -15000.0f;
    this->dirtWaveSpread = this->actor.speed = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.flags |= ACTOR_FLAG_REACT_TO_LENS;
    this->dirtWavePhase = this->actionState = this->actor.params = ENDH_WAIT_UNDERGROUND;
    EnDh_SetupAction(this, EnDh_Wait);
}

void EnDh_Wait(EnDh* this, PlayState* play) {
    const Actor* player = &GET_PLAYER(play)->actor;
    float dy = player->world.pos.y - this->actor.home.pos.y;
    Collider* cylinder = &this->collider1.base;
    if (!this->actionState) {
        if ((cylinder->acFlags & AC_HIT) && cylinder->ac && cylinder->ac->category == ACTORCAT_EXPLOSIVE) {
            cylinder->acFlags &= ~AC_HIT;
            this->actor.params = ENDH_START_ATTACK_BOMB;
        } else if (!this->handsPlaced && this->actor.xzDistToPlayer < 200.f && dy > -80.f && dy < 100.f) {
            Vec3f from = this->actor.home.pos;
            Vec3f to = player->world.pos;
            Vec3f hit;
            CollisionPoly* poly;
            s32 bgId;
            from.y += 30.f;
            to.y += 30.f;
            if (!BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId))
                this->actor.params = ENDH_START_ATTACK_BOMB;
        }
    }
    if (this->actionState || this->actor.params >= ENDH_START_ATTACK_GRAB ||
        (this->handsPlaced && this->actor.params <= -this->handsPlaced)) {
        switch (this->actionState) {
            case 0:
                this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
                this->actor.flags &= ~ACTOR_FLAG_REACT_TO_LENS;
                this->actionState++;
                this->drawDirtWave++;
                Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_HIDE);
                FALLTHROUGH;
            case 1:
                this->dirtWavePhase += 0x3A7;
                Math_SmoothStepToF(&this->dirtWaveSpread, 300.0f, 1.0f, 5.0f, 0.0f);
                this->dirtWaveHeight = Math_SinS(this->dirtWavePhase) * 55.0f;
                this->dirtWaveAlpha = (s16)(Math_SinS(this->dirtWavePhase) * 255.0f);
                EnDh_SpawnDebris(play, this, &this->actor.world.pos, this->dirtWaveSpread, 4, 2.05f, 1.2f);
                if (this->actor.shape.yOffset == 0.0f) {
                    this->drawDirtWave = false;
                    this->actionState++;
                } else if (this->dirtWavePhase > 0x12C0) {
                    this->actor.shape.yOffset += 500.0f;
                }
                break;
            case 2:
                EnDh_SetupWalk(this);
                break;
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x7D0, 0);
        SkelAnime_Update(&this->skelAnime);
        if (this->actor.params != ENDH_START_ATTACK_BOMB) {
            void (*setAutoLockOn)(PlayState*, Actor*) = (void*)0x80123e90;
            setAutoLockOn(play, &this->actor);
        }
    }
}

void EnDh_SetupWalk(EnDh* this) {
    Animation_Change(&this->skelAnime, &object_dh_Anim_003A8C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_dh_Anim_003A8C) - 3.0f, ANIMMODE_LOOP, -6.0f);
    this->curAction = DH_WALK;
    this->timer = 300;
    this->actor.speed = 1.0f;
    EnDh_SetupAction(this, EnDh_Walk);
}

void EnDh_Walk(EnDh* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA, 0);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);
    if (((s32)this->skelAnime.curFrame % 8) == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_WALK);
    }
    if ((play->gameplayFrames & 0x5F) == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_LAUGH);
    }
    if (this->actor.xzDistToPlayer <= 100.0f) {
        this->actor.speed = 0.0f;
        if (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
            EnDh_SetupAttack(this);
        }
    } else if (--this->timer == 0) {
        EnDh_SetupBurrow(this);
    }
}

void EnDh_SetupRetreat(EnDh* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &object_dh_Anim_005880, -4.0f);
    this->curAction = DH_RETREAT;
    this->timer = 70;
    this->actor.speed = 1.0f;
    EnDh_SetupAction(this, EnDh_Retreat);
}

void EnDh_Retreat(EnDh* this, PlayState* play) {
    this->timer--;
    if (this->timer == 0) {
        this->retreat = false;
        EnDh_SetupBurrow(this);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, (s16)(this->actor.yawTowardsPlayer + 0x8000), 1, 0xBB8, 0);
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    SkelAnime_Update(&this->skelAnime);
}

void EnDh_SetupAttack(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_004658, -6.0f);
    this->timer = this->actionState = 0;
    this->curAction = DH_ATTACK;
    this->actor.speed = 0.0f;
    EnDh_SetupAction(this, EnDh_Attack);
}

void EnDh_Attack(EnDh* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        this->actionState++;
    } else if ((this->actor.xzDistToPlayer > 100.0f) || !Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
        Animation_Change(&this->skelAnime, &object_dh_Anim_004658, -1.0f, this->skelAnime.curFrame, 0.0f, ANIMMODE_ONCE,
                         -4.0f);
        this->actionState = 4;
        this->colliderJntSph.base.atFlags = this->colliderJntSph.elements[0].base.atElemFlags = AT_NONE;
        this->colliderJntSph.elements[0].base.atDmgInfo.dmgFlags =
            this->colliderJntSph.elements[0].base.atDmgInfo.damage = 0;
    }
    switch (this->actionState) {
        case 1:
            Animation_PlayOnce(&this->skelAnime, &object_dh_Anim_001A3C);
            this->actionState++;
            Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_BITE);
            FALLTHROUGH;
        case 0:
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            break;
        case 2:
            if (this->skelAnime.curFrame >= 4.0f) {
                this->colliderJntSph.base.atFlags |= AT_ON | AT_TYPE_ENEMY;
                this->colliderJntSph.elements[0].base.atElemFlags = ATELEM_ON | ATELEM_SFX_NORMAL;
                this->colliderJntSph.elements[0].base.atDmgInfo.dmgFlags = DMG_DEFAULT;
                this->colliderJntSph.elements[0].base.atDmgInfo.damage = 8;
            }
            if (this->colliderJntSph.base.atFlags & AT_BOUNCED) {
                this->colliderJntSph.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                this->colliderJntSph.base.atFlags = this->colliderJntSph.elements[0].base.atElemFlags = AT_NONE;
                this->colliderJntSph.elements[0].base.atDmgInfo.dmgFlags =
                    this->colliderJntSph.elements[0].base.atDmgInfo.damage = 0;
                this->actionState++;
            } else if (this->colliderJntSph.base.atFlags & AT_HIT) {
                this->colliderJntSph.base.atFlags &= ~AT_HIT;
                Player* player = GET_PLAYER(play);
                Actor* hand = player->actor.parent;
                if (hand && hand->id == 0xca5 && hand->parent == &this->actor)
                    player->av2.actionVar2 = 100;
            }
            break;
        case 3:
            if ((this->actor.xzDistToPlayer <= 100.0f) && (Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360))) {

                Animation_Change(&this->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), ANIMMODE_ONCE, -6.0f);
                this->actionState = 0;
            } else {
                Animation_Change(&this->skelAnime, &object_dh_Anim_004658, -1.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), 0.0f, ANIMMODE_ONCE, -4.0f);
                this->actionState++;
                this->colliderJntSph.base.atFlags = this->colliderJntSph.elements[0].base.atElemFlags = AT_NONE;
                this->colliderJntSph.elements[0].base.atDmgInfo.dmgFlags =
                    this->colliderJntSph.elements[0].base.atDmgInfo.damage = 0;
            }
            break;
        case 5:
            EnDh_SetupWalk(this);
            break;
        case 4:
            break;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void EnDh_SetupBurrow(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_002148, -6.0f);
    this->curAction = DH_BURROW;
    this->dirtWaveSpread = this->actor.speed = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->dirtWavePhase = 0;
    this->actionState = 0;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_HIDE);
    EnDh_SetupAction(this, EnDh_Burrow);
}

void EnDh_Burrow(EnDh* this, PlayState* play) {
    switch (this->actionState) {
        case 0:
            this->actionState++;
            this->drawDirtWave++;
            this->collider1.base.atFlags = AT_ON | AT_TYPE_ENEMY;
            this->collider1.elem.atElemFlags = ATELEM_ON | ATELEM_SFX_NORMAL;
            this->collider1.elem.atDmgInfo.dmgFlags = DMG_DEFAULT;
            this->collider1.elem.atDmgInfo.damage = 4;
            FALLTHROUGH;
        case 1:
            this->dirtWavePhase += 0x47E;
            Math_SmoothStepToF(&this->dirtWaveSpread, 300.0f, 1.0f, 8.0f, 0.0f);
            this->dirtWaveHeight = Math_SinS(this->dirtWavePhase) * 55.0f;
            this->dirtWaveAlpha = (s16)(Math_SinS(this->dirtWavePhase) * 255.0f);
            EnDh_SpawnDebris(play, this, &this->actor.world.pos, this->dirtWaveSpread, 4, 2.05f, 1.2f);
            this->collider1.dim.radius = this->dirtWaveSpread * 0.6f;
            if (SkelAnime_Update(&this->skelAnime)) {
                this->actionState++;
            }
            break;
        case 2:
            this->drawDirtWave = false;
            this->collider1.dim.radius = 35;
            this->collider1.base.atFlags = this->collider1.elem.atElemFlags = AT_NONE;
            this->collider1.elem.atDmgInfo.dmgFlags = this->collider1.elem.atDmgInfo.damage = 0;
            EnDh_SetupWait(this);
            break;
    }
}

void EnDh_SetupDamage(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_003D6C, -6.0f);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speed = -1.0f;
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_DAMAGE);
    this->curAction = DH_DAMAGE;
    EnDh_SetupAction(this, EnDh_Damage);
}

void EnDh_Damage(EnDh* this, PlayState* play) {
    if (this->actor.speed < 0.0f) {
        this->actor.speed += 0.15f;
    }
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.world.rot.y = this->actor.shape.rot.y;
        if (this->retreat) {
            EnDh_SetupRetreat(this, play);
        } else if ((this->actor.xzDistToPlayer <= 105.0f) && Actor_IsFacingPlayer(&this->actor, 60 * 0x10000 / 360)) {
            f32 frames = Animation_GetLastFrame(&object_dh_Anim_004658);

            EnDh_SetupAttack(this);
            Animation_Change(&this->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f, frames, ANIMMODE_ONCE, -6.0f);
        } else {
            EnDh_SetupWalk(this);
        }
        this->unk_258 = 255;
    }
}

void EnDh_SetupDeath(EnDh* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &object_dh_Anim_0032BC, -1.0f);
    this->curAction = DH_DEATH;
    this->timer = 300;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.speed = 0.0f;
    this->actor.params = ENDH_DEATH;
    Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_DEAD);
    EnDh_SetupAction(this, EnDh_Death);
}

void EnDh_Death(EnDh* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime) || (this->timer != 300)) {
        if (this->timer == 300) {
            Animation_PlayLoop(&this->skelAnime, &object_dh_Anim_00375C);
        }
        this->timer--;
        if (this->timer < 150) {
            if (this->alpha != 0) {
                this->actor.scale.y -= 0.000075f;
                this->actor.shape.shadowAlpha = this->alpha -= 5;
            } else {
                Actor_Kill(&this->actor);
                return;
            }
        }
    } else {
        if (((s32)this->skelAnime.curFrame == 53) || ((s32)this->skelAnime.curFrame == 56) ||
            ((s32)this->skelAnime.curFrame == 61)) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_RIZA_DOWN);
        }
        if ((s32)this->skelAnime.curFrame == 61) {
            Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
        }
    }
}

void EnDh_CollisionCheck(EnDh* this, PlayState* play) {
    s32 lastHealth;

    if ((this->colliderJntSph.base.acFlags & AC_HIT) && !this->retreat) {
        this->colliderJntSph.base.acFlags &= ~AC_HIT;
        if ((this->actor.colChkInfo.damageEffect != 0) && (this->actor.colChkInfo.damageEffect != 6)) {
            this->colliderJntSph.base.atFlags = this->colliderJntSph.elements[0].base.atElemFlags = AT_NONE;
            this->colliderJntSph.elements[0].base.atDmgInfo.dmgFlags =
                this->colliderJntSph.elements[0].base.atDmgInfo.damage = 0;
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
            lastHealth = this->actor.colChkInfo.health;
            if (Actor_ApplyDamage(&this->actor) == 0) {
                EnDh_SetupDeath(this);
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                           COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_9, false));
            } else {
                if (((lastHealth >= 15) && (this->actor.colChkInfo.health < 15)) ||
                    ((lastHealth >= 9) && (this->actor.colChkInfo.health < 9)) ||
                    ((lastHealth >= 3) && (this->actor.colChkInfo.health < 3))) {

                    this->retreat++;
                }
                EnDh_SetupDamage(this);
            }
        }
    }
}

void EnDh_Update(Actor* thisx, PlayState* play) {
    EnDh* this = (EnDh*)thisx;

    EnDh_CollisionCheck(this, play);
    this->actionFunc(this, play);
    Actor_MoveXZGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 24.0f, 45.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3);
    this->actor.focus.pos = this->headPos;
    Collider_UpdateCylinder(&this->actor, &this->collider1);
    if (this->actor.colChkInfo.health > 0) {
        if (this->curAction == DH_WAIT) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
        } else {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
        }
        if (this->curAction != DH_DAMAGE && this->actor.shape.yOffset == 0.f) {

            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderJntSph.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderJntSph.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider1.base);
        }
    } else {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderJntSph.base);
    }
}

void EnDh_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f headOffset = { 2000.0f, 1000.0f, 0.0f };
    EnDh* this = (EnDh*)thisx;

    if (limbIndex == 13) {
        Matrix_MultVec3f(&headOffset, &this->headPos);
        Matrix_Push();
        Matrix_Translate(headOffset.x, headOffset.y, headOffset.z, MTXMODE_APPLY);
        Collider_UpdateSpheres(1, &this->colliderJntSph);
        Matrix_Pop();
    }
}

void EnDh_Draw(Actor* thisx, PlayState* play) {
    EnDh* this = (EnDh*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    if (this->alpha == 255) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_OPA_DISP++, 0x08, ACTOR_SETUP_OPA_DL);
        POLY_OPA_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               NULL, EnDh_PostLimbDraw, &this->actor, POLY_OPA_DISP);
    } else {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
        gSPSegment(POLY_XLU_DISP++, 0x08, gActorSetupXluDL);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }
    if (this->drawDirtWave) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 85, 55, 0, 130);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, (play->state.frameCount * -3) % 0x80, 0, 0x20,
                                    0x40, 1, (play->state.frameCount * -10) % 0x80,
                                    (play->state.frameCount * -20) % 0x100, 0x20, 0x40));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0, 0, 0, this->dirtWaveAlpha);

        Matrix_Translate(0.0f, -this->actor.shape.yOffset, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(this->dirtWaveSpread * 0.01f, this->dirtWaveHeight * 0.01f, this->dirtWaveSpread * 0.01f,
                     MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, object_dh_DL_007FC0);
    }
    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0xa4, En_Dh_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_INIT, EnDh_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_DESTROY, EnDh_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_UPDATE, EnDh_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_DRAW, EnDh_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_PROFILE, En_Dh_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xa4, 0xa6, 0x72389a3f };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DH_ORIGIN, kNativePortOrigin);
