#include "oot_compat.h"
#include "types/en_vali.h"
#undef ACTOR_EN_BILI
#define ACTOR_EN_BILI 0x0034
#undef ACTOR_EN_VALI
#define ACTOR_EN_VALI 0x0063
#undef OBJECT_VALI
#define OBJECT_VALI 0x1056
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387b
#undef NA_SE_EN_BARI_SPLIT
#define NA_SE_EN_BARI_SPLIT 0x3887
#undef NA_SE_EN_BIRI_SPARK
#define NA_SE_EN_BIRI_SPARK 0x389a
#undef NA_SE_EN_BARI_ROLL
#define NA_SE_EN_BARI_ROLL 0x389d
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389e
#undef NA_SE_EN_BARI_DEAD
#define NA_SE_EN_BARI_DEAD 0x389f
#undef NA_SE_EN_BARI_DAMAGE
#define NA_SE_EN_BARI_DAMAGE 0x38dc
#define gBariAttackAnim (*(AnimationHeader*)0x06000710)
#define gBariDamagedAnim (*(AnimationHeader*)0x06000854)
#define gBariRecoverAnim (*(AnimationHeader*)0x06000b34)
#define gBariIdleAnim (*(AnimationHeader*)0x060014ac)
#define gBariCoreDL (*((Gfx (*)[1])0x06002610))
#define gBariNucleusDL (*((Gfx (*)[1])0x06002740))
#define gBariHoodDL (*((Gfx (*)[1])0x060027d8))
#define gBariSkel (*(SkeletonHeader*)0x06004848)

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
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
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 yOffset))0x800b675c)
#undef Actor_Spawn
extern Actor* FOREIGN_ENEMY_API_BARI_SPLIT(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                           f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_Spawn FOREIGN_ENEMY_API_BARI_SPLIT
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137594)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137430)
#undef BgCheck_EntityRaycastDown4
#define BgCheck_EntityRaycastDown4 ((f32 (*)(CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Actor* actor, Vec3f* pos))0x800c411c)
#undef Camera_GetCamDirYaw
#define Camera_GetCamDirYaw ((s16 (*)(Camera* camera))0x800dfcdc)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_DestroyQuad
extern s32 FOREIGN_ENEMY_API_QUAD_DESTROY(PlayState* play, ColliderQuad* quad);
#define Collider_DestroyQuad FOREIGN_ENEMY_API_QUAD_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_InitQuad
#define Collider_InitQuad ((s32 (*)(PlayState* play, ColliderQuad* quad))0x800e1e18)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_SetQuad
#define Collider_SetQuad ((s32 (*)(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* src))0x800e1f20)
#undef Collider_SetQuadVertices
#define Collider_SetQuadVertices ((void (*)(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d))0x800e7e5c)
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
#undef EffectSsDtBubble_SpawnColorProfile
#define EffectSsDtBubble_SpawnColorProfile ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 life, s16 colorProfile, s16 randXZ))0x800b2090)
#undef EffectSsEnIce_SpawnFlyingVec3f
#define EffectSsEnIce_SpawnFlyingVec3f FOREIGN_ENEMY_API_FLYING_ICE_F
#undef EffectSsLightning_Spawn
#define EffectSsLightning_Spawn ((void (*)(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 yaw, s16 life, s16 numBolts))0x800b2018)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TexScroll
#define Gfx_TexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, u32 x, u32 y, s32 width, s32 height))0x8012cb4c)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x800ff03c)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_Get
#define Matrix_Get ((void (*)(MtxF* dest))0x801801e8)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Put
#define Matrix_Put ((void (*)(MtxF* src))0x8018020c)
#undef Matrix_RotateY
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
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

#include "types/en_bili.h"

#define FLAGS \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_IGNORE_QUAKE)

void EnVali_Init(Actor* thisx, PlayState* play);
void EnVali_Destroy(Actor* thisx, PlayState* play);
void EnVali_Update(Actor* thisx, PlayState* play);
void EnVali_Draw(Actor* thisx, PlayState* play);

void EnVali_SetupWaitPlayerNear(EnVali* this);
void EnVali_WaitPlayerNear(EnVali* this, PlayState* play);
void EnVali_Appear(EnVali* this, PlayState* play);
void EnVali_Spin(EnVali* this, PlayState* play);
void EnVali_Electrify(EnVali* this, PlayState* play);
void EnVali_Damaged(EnVali* this, PlayState* play);
void EnVali_Recover(EnVali* this, PlayState* play);
void EnVali_Burnt(EnVali* this, PlayState* play);
void EnVali_Die(EnVali* this, PlayState* play);
void EnVali_Stunned(EnVali* this, PlayState* play);
void EnVali_Frozen(EnVali* this, PlayState* play);
void EnVali_Bury(EnVali* this, PlayState* play);

ActorProfile En_Vali_Profile = {
    0xc63, ACTORCAT_ENEMY, FLAGS, OBJECT_VALI, sizeof(EnVali), EnVali_Init, EnVali_Destroy, EnVali_Update, EnVali_Draw,
};

static ColliderQuadInit sColliderQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_7, 0x08 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT8,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_7, 0x08 },
        { 0xFFCFFFFF, HIT_BACKLASH_ELECTRIC, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 17, 35, -15, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 18, 32, MASS_HEAVY };

typedef enum EnValiDamageReaction {
    EN_VALI_DMG_REACT_NONE,
    EN_VALI_DMG_REACT_STUN,
    EN_VALI_DMG_REACT_FIRE,
    EN_VALI_DMG_REACT_ICE,
    EN_VALI_DMG_REACT_SLINGSHOT = 0xE,
    EN_VALI_DMG_REACT_SWORD_SLASH
} EnValiDamageReaction;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_VALI_DMG_REACT_STUN),        DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(0, EN_VALI_DMG_REACT_SLINGSHOT),   DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(0, EN_VALI_DMG_REACT_STUN),        DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(1, EN_VALI_DMG_REACT_SWORD_SLASH), DMG_ENTRY(2, EN_VALI_DMG_REACT_SWORD_SLASH),
    DMG_ENTRY(4, EN_VALI_DMG_REACT_SWORD_SLASH), DMG_ENTRY(4, EN_VALI_DMG_REACT_FIRE),
    DMG_ENTRY(4, EN_VALI_DMG_REACT_ICE),         DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(4, EN_VALI_DMG_REACT_FIRE),
    DMG_ENTRY(4, EN_VALI_DMG_REACT_ICE),         DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(1, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(4, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(2, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(8, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(4, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),
    DMG_ENTRY(4, EN_VALI_DMG_REACT_NONE),        DMG_ENTRY(0, EN_VALI_DMG_REACT_NONE),
} };

static InitChainEntry sInitChain[3] = {
    ICHAIN_S8(naviEnemyId, 0xff, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 5000, ICHAIN_STOP),
};

void EnVali_Init(Actor* thisx, PlayState* play) {
    EnVali* this = (EnVali*)thisx;
    s32 bgId;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 27.0f);
    this->actor.shape.shadowAlpha = 155;
    SkelAnime_Init(play, &this->skelAnime, &gBariSkel, &gBariIdleAnim, this->jointTable, this->morphTable,
                   BARI_LIMB_MAX);
    Collider_InitQuad(play, &this->leftTentacleCollider);
    Collider_SetQuad(play, &this->leftTentacleCollider, &this->actor, &sColliderQuadInit);
    Collider_InitQuad(play, &this->rightTentacleCollider);
    Collider_SetQuad(play, &this->rightTentacleCollider, &this->actor, &sColliderQuadInit);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    EnVali_SetupWaitPlayerNear(this);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.floorHeight =
        BgCheck_EntityRaycastDown4(&play->colCtx, &this->actor.floorPoly, &bgId, &this->actor, &this->actor.world.pos);
    this->actor.params = 0;
    if (this->actor.floorHeight == BGCHECK_Y_MIN) {
        Actor_Kill(&this->actor);
    }
}

void EnVali_Destroy(Actor* thisx, PlayState* play) {
    EnVali* this = (EnVali*)thisx;

    Collider_DestroyQuad(play, &this->leftTentacleCollider);
    Collider_DestroyQuad(play, &this->rightTentacleCollider);
    Collider_DestroyCylinder(play, &this->collider);
}

void EnVali_SetupWaitPlayerNear(EnVali* this) {
    Animation_PlayLoop(&this->skelAnime, &gBariIdleAnim);
    this->actor.draw = NULL;
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnVali_WaitPlayerNear;
}

void EnVali_SetupAppear(EnVali* this) {
    this->actor.draw = EnVali_Draw;
    this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    this->actionFunc = EnVali_Appear;
    this->actor.velocity.y = 1.0f;
}

void EnVali_SetupSpin(EnVali* this) {
    Animation_MorphToLoop(&this->skelAnime, &gBariAttackAnim, -3.0f);
    this->leftTentacleCollider.dim.quad[2] = this->leftTentacleCollider.dim.quad[3] =
        this->rightTentacleCollider.dim.quad[2] = this->rightTentacleCollider.dim.quad[3] =
            this->leftTentacleCollider.dim.quad[0] = this->leftTentacleCollider.dim.quad[1] =
                this->rightTentacleCollider.dim.quad[0] = this->rightTentacleCollider.dim.quad[1] =
                    this->actor.world.pos;
    this->leftTentacleCollider.dim.quad[2].y = this->leftTentacleCollider.dim.quad[3].y =
        this->rightTentacleCollider.dim.quad[2].y = this->rightTentacleCollider.dim.quad[3].y =
            this->leftTentacleCollider.dim.quad[0].y = this->leftTentacleCollider.dim.quad[1].y =
                this->rightTentacleCollider.dim.quad[0].y = this->rightTentacleCollider.dim.quad[1].y =
                    this->actor.world.pos.y - 10.0f;
    this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->collider.base.acFlags |= AC_ON;
    this->spinTimer = 0;
    this->actionFunc = EnVali_Spin;
    this->spinHeight = this->actor.world.pos.y;
}

void EnVali_SetupElectrify(EnVali* this) {
    this->electrifyTimer = 20;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnVali_Electrify;
}

void EnVali_SetupDamaged(EnVali* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gBariDamagedAnim, -5.0f);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 150, COLORFILTER_BUFFLAG_XLU, 30);
    this->actor.params = 0;
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = EnVali_Damaged;
}

void EnVali_SetupRecover(EnVali* this) {
    Animation_PlayOnce(&this->skelAnime, &gBariRecoverAnim);
    this->actionFunc = EnVali_Recover;
}

void EnVali_SetupBurnt(EnVali* this) {
    this->timer = 2;
    this->collider.base.acFlags &= ~AC_ON;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 150, COLORFILTER_BUFFLAG_XLU, 30);
    this->actionFunc = EnVali_Burnt;
}

void EnVali_SetupDie(EnVali* this, PlayState* play) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BILI, this->actor.world.pos.x, this->actor.world.pos.y,
                    this->actor.world.pos.z, 0, this->actor.world.rot.y, 0, EN_BILI_TYPE_SPAWNED_BY_BARI);
        this->actor.world.rot.y += 0x5555;
    }
    Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                               COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_5, false));
    this->timer = Rand_S16Offset(10, 10);
    this->collider.base.acFlags &= ~AC_ON;
    SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 40, NA_SE_EN_BARI_SPLIT);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.draw = NULL;
    this->actionFunc = EnVali_Die;
}

void EnVali_SetupStunned(EnVali* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gBariAttackAnim, 10.0f);
    this->timer = 80;
    this->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_XLU, 80);
    this->collider.elem.acDmgInfo.effect = HIT_BACKLASH_NONE;
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->actionFunc = EnVali_Stunned;
    this->actor.velocity.y = 1.0f;
}

void EnVali_SetupFrozen(EnVali* this) {
    this->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_XLU, 36);
    this->collider.base.acFlags &= ~AC_ON;
    this->timer = 36;
    this->actionFunc = EnVali_Frozen;
}

void EnVali_SetupBury(EnVali* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gBariIdleAnim, 10.0f);
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actionFunc = EnVali_Bury;
}

void EnVali_DoElectricEffectsAndSfx(EnVali* this, PlayState* play) {
    static Color_RGBA8 sEffPrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sEffEnvColor = { 200, 255, 255, 255 };
    Vec3f effPos;
    f32 cosCamYaw;
    f32 sinCamYaw;
    s32 i;
    s16 angle;

    for (i = 0; i < 4; i++) {
        cosCamYaw = -Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)));
        sinCamYaw = Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)));
        if (!((this->electrifyTimer + (i << 1)) % 4)) {
            angle = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            effPos.x = this->actor.world.pos.x + (Math_SinS(angle) * 12.0f * cosCamYaw);
            effPos.y = this->actor.world.pos.y - (Math_CosS(angle) * 12.0f) + 10.0f;
            effPos.z = this->actor.world.pos.z + (Math_SinS(angle) * 12.0f * sinCamYaw);
            EffectSsLightning_Spawn(play, &effPos, &sEffPrimColor, &sEffEnvColor, 17, angle, 6, 2);
        }
    }
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_BIRI_SPARK - SFX_FLAG);
}

void EnVali_WaitPlayerNear(EnVali* this, PlayState* play) {
    if (this->actor.xzDistToPlayer < 150.0f) {
        EnVali_SetupAppear(this);
    }
}

void EnVali_Appear(EnVali* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->actor.velocity.y *= 1.5f;
    this->actor.velocity.y = CLAMP_MAX(this->actor.velocity.y, 40.0f);
    if (Math_StepToF(&this->actor.world.pos.y, this->actor.floorHeight, this->actor.velocity.y)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        EnVali_SetupSpin(this);
    }
}

void EnVali_Spin(EnVali* this, PlayState* play) {
    s32 animCurFrame;

    SkelAnime_Update(&this->skelAnime);
    if (this->spinTimer != 0) {
        this->spinTimer -= 1;
    }
    animCurFrame = (s32)this->skelAnime.curFrame;
    Math_StepToF(&this->spinHeight, this->actor.floorHeight + 40.0f, 1.2f);
    this->actor.world.pos.y = this->spinHeight - (sinf(animCurFrame * M_PI * 0.0125f) * 8.0f);
    if (this->spinTimer != 0) {
        this->actor.shape.rot.y += 0x800;
        if (((this->spinTimer % 6) == 0) && (animCurFrame >= 16) && (animCurFrame < 56)) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BARI_ROLL);
        }
    } else if ((animCurFrame == 16) || (animCurFrame == 30) || (animCurFrame == 42) || (animCurFrame == 55)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BARI_ROLL);
    }
    if (animCurFrame > 40) {
        animCurFrame = 80 - animCurFrame;
    }
    this->actor.shape.rot.y += DEG_TO_BINANG((animCurFrame + 4) * 0.4f);
    if (this->actor.xzDistToPlayer > 250.0f) {
        EnVali_SetupBury(this);
    }
}

void EnVali_Electrify(EnVali* this, PlayState* play) {
    if (this->electrifyTimer != 0) {
        this->electrifyTimer--;
    }
    EnVali_DoElectricEffectsAndSfx(this, play);
    if (this->electrifyTimer == 0) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->collider.base.acFlags |= AC_ON;
        if (this->actor.params == 1) {
            EnVali_SetupDamaged(this);
        } else {
            this->actionFunc = EnVali_Spin;
        }
    } else {
        if (this->electrifyTimer & 1) {
            this->actor.world.pos.y += 1.0f;
        } else {
            this->actor.world.pos.y -= 1.0f;
        }
    }
}

void EnVali_Damaged(EnVali* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->actor.colChkInfo.health != 0) {
            EnVali_SetupRecover(this);
        } else {
            EnVali_SetupDie(this, play);
        }
    }
}

void EnVali_Recover(EnVali* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        EnVali_SetupSpin(this);
    }
}

void EnVali_Burnt(EnVali* this, PlayState* play) {
    if (this->timer != 0) {
        this->timer--;
    }
    if (this->timer == 0) {
        EnVali_SetupDie(this, play);
    }
}

void EnVali_Die(EnVali* this, PlayState* play) {
    static Vec3f sEffVel = { 0.0f, 0.0f, 0.0f };
    static Vec3f sEffAccel = { 0.0f, 0.0f, 0.0f };
    s16 effScale;
    Vec3f effPos;
    s32 i;

    if (this->timer != 0) {
        this->timer--;
    }
    for (i = 0; i < 2; i++) {
        effPos.x = Rand_CenteredFloat(20.0f) + this->actor.world.pos.x;
        effPos.y = Rand_CenteredFloat(8.0f) + this->actor.world.pos.y;
        effPos.z = Rand_CenteredFloat(20.0f) + this->actor.world.pos.z;
        sEffVel.y = Rand_ZeroOne() + 1.0f;
        effScale = Rand_S16Offset(40, 40);
        if (Rand_ZeroOne() < 0.7f) {
            EffectSsDtBubble_SpawnColorProfile(play, &effPos, &sEffVel, &sEffAccel, effScale, 25, 2, 1);
        } else {
            EffectSsDtBubble_SpawnColorProfile(play, &effPos, &sEffVel, &sEffAccel, effScale, 25, 0, 1);
        }
    }
    if (this->timer == 0) {
        Actor_Kill(&this->actor);
    }
}

void EnVali_Stunned(EnVali* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (this->timer != 0) {
        this->timer--;
    }
    if (this->actor.velocity.y != 0.0f) {
        if (Math_StepToF(&this->actor.world.pos.y, this->actor.floorHeight, this->actor.velocity.y)) {
            this->actor.velocity.y = 0.0f;
            Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        } else {
            this->actor.velocity.y += 1.0f;
        }
    }
    if (this->timer == 0) {
        this->collider.elem.acDmgInfo.effect = HIT_BACKLASH_ELECTRIC;
        EnVali_SetupSpin(this);
    }
}

void EnVali_Frozen(EnVali* this, PlayState* play) {
    Vec3f effPos;
    s32 temp_v0_2;
    s32 temp_v1;

    if (this->timer != 0) {
        this->timer--;
    }
    temp_v1 = this->timer - 20;
    this->actor.colorFilterTimer = 36;
    if (temp_v1 > 0) {
        temp_v0_2 = temp_v1 >> 1;
        if (this->timer & 1) {
            effPos.y = (this->actor.world.pos.y - 20.0f) + ((-temp_v0_2 * 5) + 40);
            effPos.x = this->actor.world.pos.x + ((temp_v0_2 & 2) ? 12.0f : -12.0f);
            effPos.z = this->actor.world.pos.z + ((temp_v0_2 & 1) ? 12.0f : -12.0f);
            EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &effPos, 150, 150, 150, 250, 235, 245, 255,
                                           (Rand_ZeroOne() * 0.2f) + 1.3f);
        }
    } else if (this->timer == 0) {
        this->actor.velocity.y += 1.0f;
        if (Math_StepToF(&this->actor.world.pos.y, this->actor.floorHeight, this->actor.velocity.y)) {
            EnVali_SetupDie(this, play);
            this->actor.colorFilterTimer = 0;
        }
    }
}

void EnVali_Bury(EnVali* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.5f, 15.0f, 0.1f) < 0.01f) {
        EnVali_SetupWaitPlayerNear(this);
    }
}

void EnVali_CheckCollide(EnVali* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.elem, true);
        if ((this->actor.colChkInfo.damageEffect != EN_VALI_DMG_REACT_NONE) || (this->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&this->actor) == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_BARI_DEAD);
                Enemy_StartFinishingBlow(play, &this->actor);
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            } else {
                if ((this->actor.colChkInfo.damageEffect != EN_VALI_DMG_REACT_STUN) &&
                    (this->actor.colChkInfo.damageEffect != EN_VALI_DMG_REACT_SLINGSHOT)) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_BARI_DAMAGE);
                }
            }
            if (this->actor.colChkInfo.damageEffect == EN_VALI_DMG_REACT_STUN) {
                if (this->actionFunc != EnVali_Stunned) {
                    EnVali_SetupStunned(this);
                }
            } else {
                if (this->actor.colChkInfo.damageEffect == EN_VALI_DMG_REACT_SWORD_SLASH) {
                    if (this->actionFunc != EnVali_Stunned) {
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 150, COLORFILTER_BUFFLAG_XLU, 30);
                        this->actor.params = 1;
                        EnVali_SetupElectrify(this);
                    } else {
                        EnVali_SetupDamaged(this);
                    }
                } else if (this->actor.colChkInfo.damageEffect == EN_VALI_DMG_REACT_FIRE) {
                    EnVali_SetupBurnt(this);
                } else if (this->actor.colChkInfo.damageEffect == EN_VALI_DMG_REACT_ICE) {
                    EnVali_SetupFrozen(this);
                } else if (this->actor.colChkInfo.damageEffect == EN_VALI_DMG_REACT_SLINGSHOT) {
                    if (this->spinTimer == 0) {
                        this->spinTimer = 20;
                    }
                } else {
                    EnVali_SetupDamaged(this);
                }
            }
        }
    }
}

void EnVali_Update(Actor* thisx, PlayState* play) {
    EnVali* this = (EnVali*)thisx;

    if ((this->collider.base.atFlags & AT_HIT) || (this->leftTentacleCollider.base.atFlags & AT_HIT) ||
        (this->rightTentacleCollider.base.atFlags & AT_HIT)) {
        this->leftTentacleCollider.base.atFlags &= ~AT_HIT;
        this->rightTentacleCollider.base.atFlags &= ~AT_HIT;
        this->collider.base.atFlags &= ~AT_HIT;
        EnVali_SetupElectrify(this);
    }
    EnVali_CheckCollide(this, play);
    this->actionFunc(this, play);
    if ((this->actionFunc != EnVali_Die) && (this->actionFunc != EnVali_WaitPlayerNear)) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        if (this->actionFunc == EnVali_Spin) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->leftTentacleCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->rightTentacleCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
        if (this->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        Actor_SetFocus(&this->actor, 0.0f);
    }
}

void EnVali_GetHoodScale(EnVali* this, f32 frame, Vec3f* scale) {
    f32 f;
    s32 n;

    if (this->actionFunc == EnVali_Electrify) {
        n = 20 - (this->electrifyTimer % 20);
        if (n >= 10) {
            n -= 10;
        }
        scale->y -= 0.2f * sinf(n * (M_PI / 10));
    } else if (this->actionFunc == EnVali_Damaged) {
        f = sinf((M_PI / 10) * frame);
        scale->y -= 0.24f * f;
        scale->x -= 0.13f * f;
        scale->z = scale->x;
    } else if (this->actionFunc == EnVali_Recover) {
        f = cosf((M_PI / 50) * frame);
        scale->y -= 0.24f * f;
        scale->x -= 0.13f * f;
        scale->z = scale->x;
    } else if (this->actionFunc == EnVali_Stunned) {
        f = sinf(this->timer * (M_PI / 10)) * 0.08f;
        scale->x += f;
        scale->y -= f;
        scale->z += f;
    } else {
        if (frame >= 40.0f) {
            frame -= 40.0f;
        }
        scale->y -= 0.2f * sinf((M_PI / 40) * frame);
    }
}

void EnVali_GetCoreScale(EnVali* this, f32 frame, Vec3f* scale) {
    f32 f;
    s32 n;

    if (this->actionFunc == EnVali_Electrify) {
        n = 0x14 - (this->electrifyTimer % 20);
        if (n >= 0xA) {
            n -= 0xA;
        }
        scale->y -= 0.13f * sinf(n * (M_PI / 10));
    } else if (this->actionFunc == EnVali_Damaged) {
        f = sinf((M_PI / 10) * frame);
        scale->y -= 0.18f * f;
        scale->x -= 0.1f * f;
        scale->z = scale->x;
    } else if (this->actionFunc == EnVali_Recover) {
        f = cosf((M_PI / 50) * frame);
        scale->y -= 0.18f * f;
        scale->x -= 0.1f * f;
        scale->z = scale->x;
    } else if (this->actionFunc == EnVali_Stunned) {
        f = sinf(this->timer * (M_PI / 10)) * 0.08f;
        scale->x -= f;
        scale->y += f;
        scale->z -= f;
    } else {
        if (frame >= 40.0f) {
            frame -= 40.0f;
        }
        scale->y -= 0.13f * sinf((M_PI / 40) * frame);
    }
}

s32 EnVali_UpdateTentaclesLengthFactor(EnVali* this, f32 frame) {
    f32 target;

    if (this->actionFunc == EnVali_Spin) {
        if (frame <= 10.0f) {
            target = (frame * 0.05f) + 1.0f;
        } else if (frame > 70.0f) {
            target = ((80.0f - frame) * 0.05f) + 1.0f;
        } else {
            target = 1.5f;
        }
    } else if (this->actionFunc == EnVali_Damaged) {
        target = 1.0f - (sinf((M_PI / 10) * frame) * 0.35f);
    } else if (this->actionFunc == EnVali_Recover) {
        target = 1.0f - (cosf((M_PI / 50) * frame) * 0.35f);
    } else if ((this->actionFunc == EnVali_Electrify) || (this->actionFunc == EnVali_Frozen)) {
        target = this->tentaclesLengthFactor;
    } else {
        target = 1.0f;
    }
    Math_StepToF(&this->tentaclesLengthFactor, target, 0.1f);
    if (this->tentaclesLengthFactor == 1.0f) {
        return false;
    } else {
        return true;
    }
}

s32 EnVali_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                            Gfx** gfx) {
    EnVali* this = (EnVali*)thisx;
    f32 animCurFrame;

    if ((limbIndex == BARI_LIMB_NUCLEUS) || (limbIndex == BARI_LIMB_HOOD) || (limbIndex == BARI_LIMB_CORE)) {
        *dList = NULL;
        return false;
    } else {
        animCurFrame = this->skelAnime.curFrame;
        if (((limbIndex == BARI_LIMB_LEFT_TENTACLE_ROOT) || (limbIndex == BARI_LIMB_RIGHT_TENTACLE_ROOT)) &&
            EnVali_UpdateTentaclesLengthFactor(this, animCurFrame)) {
            Matrix_Scale(this->tentaclesLengthFactor, 1.0f, 1.0f, MTXMODE_APPLY);
        }
        return false;
    }
}

void EnVali_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    static Vec3f D_80B28970 = { 3000.0f, 0.0f, 0.0f };
    static Vec3f D_80B2897C = { -1000.0f, 0.0f, 0.0f };
    Vec3f tentacleTipPos;
    Vec3f tentacleBasePos;
    EnVali* this = (EnVali*)thisx;

    if (this->actionFunc == EnVali_Spin) {
        switch (limbIndex) {
            case BARI_LIMB_LEFT_TENTACLE_BODY_ROOT:
            case BARI_LIMB_RIGHT_TENTACLE_BODY_ROOT:
                Matrix_MultVec3f(&D_80B28970, &tentacleTipPos);
                Matrix_MultVec3f(&D_80B2897C, &tentacleBasePos);
                if (limbIndex == BARI_LIMB_LEFT_TENTACLE_BODY_ROOT) {
                    Collider_SetQuadVertices(&this->leftTentacleCollider, &tentacleBasePos, &tentacleTipPos,
                                             &this->leftTentacleCollider.dim.quad[0],
                                             &this->leftTentacleCollider.dim.quad[1]);
                } else {
                    Collider_SetQuadVertices(&this->rightTentacleCollider, &tentacleBasePos, &tentacleTipPos,
                                             &this->rightTentacleCollider.dim.quad[0],
                                             &this->rightTentacleCollider.dim.quad[1]);
                }
                break;
        }
    }
}

void EnVali_DrawBody(EnVali* this, PlayState* play) {
    MtxF mf;
    f32 cosYaw;
    f32 sinYaw;
    f32 animCurFrame;
    Vec3f scale = { 1.0f, 1.0f, 1.0f };

    OPEN_DISPS(play->state.gfxCtx);

    Matrix_Get(&mf);

    animCurFrame = this->skelAnime.curFrame;
    EnVali_GetCoreScale(this, animCurFrame, &scale);
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gBariCoreDL);

    Matrix_Put(&mf);

    Matrix_RotateY(BINANG_TO_RAD(-this->actor.shape.rot.y), MTXMODE_APPLY);
    cosYaw = Math_CosS(this->actor.shape.rot.y);
    sinYaw = Math_SinS(this->actor.shape.rot.y);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);
    Matrix_Translate((506.0f * cosYaw) + (372.0f * sinYaw), 1114.0f, (372.0f * cosYaw) - (506.0f * sinYaw),
                     MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);
    Matrix_Translate((-964.0f * cosYaw) - (804.0f * sinYaw), -108.0f, (-804.0f * cosYaw) + (964.0f * sinYaw),
                     MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);

    Matrix_Put(&mf);

    scale.x = scale.y = scale.z = 1.0f;
    EnVali_GetHoodScale(this, animCurFrame, &scale);
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gBariHoodDL);

    Matrix_Put(&mf);

    CLOSE_DISPS();
}

static Gfx sBariCoreInvertTexCombinerDL[] = {
    gsDPSetCombineLERP(1, TEXEL0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};
static Gfx sBariCoreNormalCombinerDL[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};

void EnVali_Draw(Actor* thisx, PlayState* play) {
    EnVali* this = (EnVali*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TexScroll(play->state.gfxCtx, 0, (0x7F - (play->gameplayFrames * 12)) & 0x7F, 32, 32));
    if (this->electrifyTimer & 1) {
        gSPSegment(POLY_XLU_DISP++, 9, sBariCoreInvertTexCombinerDL);
    } else {
        gSPSegment(POLY_XLU_DISP++, 9, sBariCoreNormalCombinerDL);
    }
    EnVali_DrawBody(this, play);
    POLY_XLU_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnVali_OverrideLimbDraw,
                                   EnVali_PostLimbDraw, this, POLY_XLU_DISP);
    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x63, En_Vali_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_INIT, EnVali_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_DESTROY, EnVali_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_UPDATE, EnVali_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_DRAW, EnVali_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_PROFILE, En_Vali_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x63, 0x56, 0x832b9f33 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VALI_ORIGIN, kNativePortOrigin);
