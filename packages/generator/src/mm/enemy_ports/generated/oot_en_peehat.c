#include "oot_variant_compat.h"
#include "types/en_peehat.h"
#define IS_DAY (!gSave.isNight)
#undef ACTOR_EN_PEEHAT
#define ACTOR_EN_PEEHAT 0xc1d
#undef OBJECT_PEEHAT
#define OBJECT_PEEHAT 0x1018
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
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
#undef Actor_SetDropFlagJntSph
extern void FOREIGN_ENEMY_API_DROP_FLAG_JOINT(Actor* actor, ColliderJntSph* jntSph, s32 freezeFlag);
#define Actor_SetDropFlagJntSph FOREIGN_ENEMY_API_DROP_FLAG_JOINT
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_Spawn
extern Actor* FOREIGN_ENEMY_API_SPAWN(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                      f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_Spawn FOREIGN_ENEMY_API_SPAWN
#undef Actor_SpawnAsChild
extern Actor* FOREIGN_ENEMY_API_SPAWN_CHILD(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId,
                                            f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_SpawnAsChild FOREIGN_ENEMY_API_SPAWN_CHILD
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x80134748)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137430)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*))0x800c55c4)
#undef Collider_DestroyCylinder
extern s32 FOREIGN_ENEMY_API_CYLINDER_DESTROY(PlayState* play, ColliderCylinder* cyl);
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_DestroyJntSph
extern s32 FOREIGN_ENEMY_API_JOINT_DESTROY(PlayState* play, ColliderJntSph* jntSph);
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_InitQuad
#define Collider_InitQuad ((s32 (*)(PlayState* play, ColliderQuad* quad))0x800e1e18)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef Collider_SetQuad
#define Collider_SetQuad ((s32 (*)(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* src))0x800e1f20)
#undef Collider_SetQuadVertices
#define Collider_SetQuadVertices ((void (*)(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d))0x800e7e5c)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
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
#undef DMG_ARROW
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
#undef EffectSsDeadDb_Spawn
extern void FOREIGN_ENEMY_API_DEATH_FLAME(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale,
                                          s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG,
                                          s16 envB, s16 unused, s32 arg14, s16 playSfx);
#define EffectSsDeadDb_Spawn FOREIGN_ENEMY_API_DEATH_FLAME
#undef EffectSsEnFire_SpawnVec3f
#define EffectSsEnFire_SpawnVec3f ((void (*)(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart))0x800b2df4)
#undef EffectSsHahen_Spawn
extern void FOREIGN_ENEMY_API_DEFAULT_FRAGMENT(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 unused,
                                               s16 scale, s16 objId, s16 life, Gfx* dList);
#define EffectSsHahen_Spawn FOREIGN_ENEMY_API_DEFAULT_FRAGMENT
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
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
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_RotateX
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_XF(f32 x, u8 mode);
#define Matrix_RotateX FOREIGN_ENEMY_API_MATRIX_ROTATE_XF
#undef Matrix_RotateY
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_YF(f32 y, u8 mode);
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZ
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF(f32 z, u8 mode);
#define Matrix_RotateZ FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF
#undef Matrix_Scale
extern void FOREIGN_ENEMY_API_MATRIX_SCALE(f32 x, f32 y, f32 z, u8 mode);
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
extern void FOREIGN_ENEMY_API_MATRIX_TRANSLATE(f32 x, f32 y, f32 z, u8 mode);
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
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
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389E
#undef NA_SE_EN_PIHAT_UP
#define NA_SE_EN_PIHAT_UP 0x3954
#undef NA_SE_EN_PIHAT_FLY
#define NA_SE_EN_PIHAT_FLY 0x3955
#undef NA_SE_EN_PIHAT_DAMAGE
#define NA_SE_EN_PIHAT_DAMAGE 0x3956
#undef NA_SE_EN_PIHAT_LAND
#define NA_SE_EN_PIHAT_LAND 0x3957
#undef NA_SE_EN_PIHAT_SM_FLY
#define NA_SE_EN_PIHAT_SM_FLY 0x3984
#undef NA_SE_EN_PIHAT_SM_DEAD
#define NA_SE_EN_PIHAT_SM_DEAD 0x3985
#define gPeehatSkel (*(SkeletonHeader*)0x06001c80)
#define gPeehatLandingAnim (*(AnimationHeader*)0x06000350)
#define gPeehatFlyingAnim (*(AnimationHeader*)0x060005c4)
#define gPeehatRecoilAnim (*(AnimationHeader*)0x06000844)
#define gPeehatRisingAnim (*(AnimationHeader*)0x060009c4)

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT)

#define MAX_LARVA 3

void EnPeehat_Init(Actor* thisx, PlayState* play);
void EnPeehat_Destroy(Actor* thisx, PlayState* play);
void EnPeehat_Update(Actor* thisx, PlayState* play);
void EnPeehat_Draw(Actor* thisx, PlayState* play);

void EnPeehat_Ground_SetStateGround(EnPeehat* this);
void EnPeehat_Flying_SetStateGround(EnPeehat* this);
void EnPeehat_Larva_SetStateSeekPlayer(EnPeehat* this);
void EnPeehat_Ground_StateGround(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_SetStateRise(EnPeehat* this);
void EnPeehat_Flying_StateGrounded(EnPeehat* this, PlayState* play);
void EnPeehat_Flying_SetStateRise(EnPeehat* this);
void EnPeehat_Flying_StateFly(EnPeehat* this, PlayState* play);
void EnPeehat_Flying_SetStateLanding(EnPeehat* this);
void EnPeehat_Ground_StateRise(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_SetStateHover(EnPeehat* this);
void EnPeehat_Flying_StateRise(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_StateSeekPlayer(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_SetStateReturnHome(EnPeehat* this);
void EnPeehat_Ground_SetStateLanding(EnPeehat* this);
void EnPeehat_Larva_StateSeekPlayer(EnPeehat* this, PlayState* play);
void EnPeehat_SetStateAttackRecoil(EnPeehat* this);
void EnPeehat_Ground_StateLanding(EnPeehat* this, PlayState* play);
void EnPeehat_Flying_StateLanding(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_StateHover(EnPeehat* this, PlayState* play);
void EnPeehat_Ground_StateReturnHome(EnPeehat* this, PlayState* play);
void EnPeehat_StateAttackRecoil(EnPeehat* this, PlayState* play);
void EnPeehat_StateBoomerangStunned(EnPeehat* this, PlayState* play);
void EnPeehat_Adult_StateDie(EnPeehat* this, PlayState* play);
void EnPeehat_SetStateExplode(EnPeehat* this);
void EnPeehat_StateExplode(EnPeehat* this, PlayState* play);

ActorProfile En_Peehat_Profile = {
    ACTOR_EN_PEEHAT, ACTORCAT_ENEMY,   FLAGS,           OBJECT_PEEHAT, sizeof(EnPeehat),
    EnPeehat_Init,   EnPeehat_Destroy, EnPeehat_Update, EnPeehat_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_WOOD,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 50, 160, -70, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 20 }, 100 },
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

static ColliderQuadInit sQuadInit = {
    {
        COL_MATERIAL_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x10 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum EnPeehatDamageReaction {
    PEAHAT_DMG_REACT_ATTACK = 0,
    PEAHAT_DMG_REACT_LIGHT_ICE_ARROW = 6,
    PEAHAT_DMG_REACT_FIRE = 12,
    PEAHAT_DMG_REACT_HOOKSHOT = 13,
    PEAHAT_DMG_REACT_BOOMERANG = 14,
    PEAHAT_DMG_REACT_NUT = 15
} EnPeehatDamageReaction;

static DamageTable sDamageTable = {
    DMG_ENTRY(0, PEAHAT_DMG_REACT_NUT),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(1, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_BOOMERANG),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_HOOKSHOT),
    DMG_ENTRY(1, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(4, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(4, PEAHAT_DMG_REACT_FIRE),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(3, PEAHAT_DMG_REACT_FIRE),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_LIGHT_ICE_ARROW),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_LIGHT_ICE_ARROW),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(1, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(4, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(2, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(8, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(4, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(4, PEAHAT_DMG_REACT_ATTACK),
    DMG_ENTRY(0, PEAHAT_DMG_REACT_ATTACK),
};

typedef enum PeahatState {
    PEAHAT_STATE_DYING,
    PEAHAT_STATE_EXPLODE,
    PEAHAT_STATE_3 = 3,
    PEAHAT_STATE_4,
    PEAHAT_STATE_FLY,
    PEAHAT_STATE_ATTACK_RECOIL = 7,
    PEAHAT_STATE_8,
    PEAHAT_STATE_9,
    PEAHAT_STATE_LANDING,
    PEAHAT_STATE_RETURN_HOME = 12,
    PEAHAT_STATE_STUNNED,
    PEAHAT_STATE_SEEK_PLAYER,
    PEAHAT_STATE_15
} PeahatState;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(lockOnArrowOffset, 700, ICHAIN_STOP),
};

void EnPeehat_SetupAction(EnPeehat* this, EnPeehatActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnPeehat_Init(Actor* thisx, PlayState* play) {
    EnPeehat* this = (EnPeehat*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Actor_SetScale(&this->actor, 36.0f * 0.001f);
    SkelAnime_Init(play, &this->skelAnime, &gPeehatSkel, &gPeehatRisingAnim, this->jointTable, this->morphTable, 24);
    ActorShape_Init(&this->actor.shape, 100.0f, ActorShadow_DrawCircle, 27.0f);
    this->actor.focus.pos = this->actor.world.pos;
    this->unk_2D4 = 0;
    this->actor.world.rot.y = 0;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 6;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.floorHeight = this->actor.world.pos.y;
    Collider_InitCylinder(play, &this->colliderCylinder);
    Collider_SetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);
    Collider_InitQuad(play, &this->colliderQuad);
    Collider_SetQuad(play, &this->colliderQuad, &this->actor, &sQuadInit);
    Collider_InitJntSph(play, &this->colliderJntSph);
    Collider_SetJntSph(play, &this->colliderJntSph, &this->actor, &sJntSphInit, this->colliderJntSphElements);

    this->actor.naviEnemyId = TATL_HINT_ID_PEAHAT;
    this->xzDistToRise = 740.0f;
    this->xzDistMax = 1200.0f;
    this->actor.cullingVolumeDistance = 4000.0f;
    this->actor.cullingVolumeScale = 800.0f;
    this->actor.cullingVolumeDownward = 1800.0f;
    switch (this->actor.params) {
        case PEAHAT_TYPE_GROUNDED:
            EnPeehat_Ground_SetStateGround(this);
            break;
        case PEAHAT_TYPE_FLYING:
            this->actor.cullingVolumeDistance = 4200.0f;
            this->xzDistToRise = 2800.0f;
            this->xzDistMax = 1400.0f;
            EnPeehat_Flying_SetStateGround(this);
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            break;
        case PEAHAT_TYPE_LARVA:
            this->actor.scale.x = this->actor.scale.z = 0.006f;
            this->actor.scale.y = 0.003f;
            this->colliderCylinder.dim.radius = 25;
            this->colliderCylinder.dim.height = 15;
            this->colliderCylinder.dim.yShift = -5;
            this->colliderCylinder.elem.acDmgInfo.dmgFlags = DMG_ARROW | DMG_SLINGSHOT;
            this->colliderQuad.base.atFlags = AT_ON | AT_TYPE_ENEMY;
            this->colliderQuad.base.acFlags = AC_ON | AC_TYPE_PLAYER;
            this->actor.naviEnemyId = TATL_HINT_ID_PEAHAT_LARVA;
            EnPeehat_Larva_SetStateSeekPlayer(this);
            break;
    }
}

void EnPeehat_Destroy(Actor* thisx, PlayState* play) {
    EnPeehat* this = (EnPeehat*)thisx;
    EnPeehat* parent;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
    Collider_DestroyJntSph(play, &this->colliderJntSph);

    if (this->actor.params == 0) {
        for (Actor* child = play->actorCtx.actors[ACTORCAT_ENEMY].first; child; child = child->next) {
            if (child->id == this->actor.id && child->parent == &this->actor)
                child->parent = NULL;
        }
    }

    if (this->actor.params > 0) {
        parent = (EnPeehat*)this->actor.parent;
        if (parent != NULL && parent->actor.update != NULL) {
            parent->unk_2FA--;
        }
    }
}

void EnPeehat_SpawnDust(PlayState* play, EnPeehat* this, Vec3f* pos, f32 arg3, s32 arg4, f32 arg5, f32 arg6) {
    Vec3f dustPos;
    Vec3f dustVel = { 0.0f, 8.0f, 0.0f };
    Vec3f dustAccel = { 0.0f, -1.5f, 0.0f };
    f32 rot;
    s32 pScale;

    rot = (Rand_ZeroOne() - 0.5f) * 6.28f;
    dustPos.y = this->actor.floorHeight;
    dustPos.x = Math_SinF(rot) * arg3 + pos->x;
    dustPos.z = Math_CosF(rot) * arg3 + pos->z;
    dustAccel.x = (Rand_ZeroOne() - 0.5f) * arg5;
    dustAccel.z = (Rand_ZeroOne() - 0.5f) * arg5;
    dustVel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    pScale = (Rand_ZeroOne() * 5 + 12) * arg6;
    EffectSsHahen_Spawn(play, &dustPos, &dustVel, &dustAccel, arg4, pScale, HAHEN_OBJECT_DEFAULT, 10, NULL);
}

void EnPeehat_HitWhenGrounded(EnPeehat* this, PlayState* play) {
    this->colliderCylinder.base.acFlags &= ~AC_HIT;
    if ((play->gameplayFrames & 0xF) == 0) {
        Vec3f itemDropPos = this->actor.world.pos;

        itemDropPos.y += 70.0f;
        Item_DropCollectibleRandom(play, &this->actor, &itemDropPos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        Item_DropCollectibleRandom(play, &this->actor, &itemDropPos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        Item_DropCollectibleRandom(play, &this->actor, &itemDropPos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        this->unk_2D4 = 240;
    } else {
        s32 i;

        this->colliderCylinder.base.acFlags &= ~AC_HIT;
        for (i = MAX_LARVA - this->unk_2FA; i > 0; i--) {
            Actor* larva = Actor_SpawnAsChild(
                &play->actorCtx, &this->actor, play, 0x1d, Rand_CenteredFloat(25.0f) + this->actor.world.pos.x,
                Rand_CenteredFloat(25.0f) + (this->actor.world.pos.y + 50.0f),
                Rand_CenteredFloat(25.0f) + this->actor.world.pos.z, 0, 0, 0, PEAHAT_TYPE_LARVA);

            if (larva != NULL) {
                larva->velocity.y = 6.0f;
                larva->shape.rot.y = larva->world.rot.y = Rand_CenteredFloat(0xFFFF);
                this->unk_2FA++;
            }
        }
        this->unk_2D4 = 8;
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_DAMAGE);
}

void EnPeehat_Ground_SetStateGround(EnPeehat* this) {
    Animation_Change(&this->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, Animation_GetLastFrame(&gPeehatRisingAnim),
                     ANIMMODE_ONCE, 0.0f);
    this->seekPlayerTimer = 600;
    this->unk_2D4 = 0;

    this->state = PEAHAT_STATE_3;
    this->colliderCylinder.base.acFlags &= ~AC_HIT;
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateGround);
}

void EnPeehat_Ground_StateGround(EnPeehat* this, PlayState* play) {
    if (IS_DAY) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->riseDelayTimer == 0) {
            if (this->actor.xzDistToPlayer < this->xzDistToRise) {
                EnPeehat_Ground_SetStateRise(this);
            }
        } else {
            Math_SmoothStepToF(&this->actor.shape.yOffset, -1000.0f, 1.0f, 10.0f, 0.0f);
            this->riseDelayTimer--;
        }
    } else {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        Math_SmoothStepToF(&this->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
        if (this->unk_2D4 != 0) {
            this->unk_2D4--;
            if (this->unk_2D4 & 4) {
                Math_SmoothStepToF(&this->scaleShift, 0.205f, 1.0f, 0.235f, 0.0f);
            } else {
                Math_SmoothStepToF(&this->scaleShift, 0.0f, 1.0f, 0.005f, 0.0f);
            }
        } else if (this->colliderCylinder.base.acFlags & AC_HIT) {
            EnPeehat_HitWhenGrounded(this, play);
        }
    }
}

void EnPeehat_Flying_SetStateGround(EnPeehat* this) {
    Animation_Change(&this->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, Animation_GetLastFrame(&gPeehatRisingAnim),
                     ANIMMODE_ONCE, 0.0f);
    this->seekPlayerTimer = 400;
    this->unk_2D4 = 0;

    this->state = PEAHAT_STATE_4;
    EnPeehat_SetupAction(this, EnPeehat_Flying_StateGrounded);
}

void EnPeehat_Flying_StateGrounded(EnPeehat* this, PlayState* play) {
    if (IS_DAY) {
        if (this->actor.xzDistToPlayer < this->xzDistToRise) {
            EnPeehat_Flying_SetStateRise(this);
        }
    } else {
        Math_SmoothStepToF(&this->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
        if (this->unk_2D4 != 0) {
            this->unk_2D4--;
            if (this->unk_2D4 & 4) {
                Math_SmoothStepToF(&this->scaleShift, 0.205f, 1.0f, 0.235f, 0.0f);
            } else {
                Math_SmoothStepToF(&this->scaleShift, 0.0f, 1.0f, 0.005f, 0.0f);
            }
        } else if (this->colliderCylinder.base.acFlags & AC_HIT) {
            EnPeehat_HitWhenGrounded(this, play);
        }
    }
}

void EnPeehat_Flying_SetStateFly(EnPeehat* this) {
    Animation_PlayLoop(&this->skelAnime, &gPeehatFlyingAnim);
    this->state = PEAHAT_STATE_FLY;
    EnPeehat_SetupAction(this, EnPeehat_Flying_StateFly);
}

void EnPeehat_Flying_StateFly(EnPeehat* this, PlayState* play) {
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
    SkelAnime_Update(&this->skelAnime);
    if (!IS_DAY || this->xzDistToRise < this->actor.xzDistToPlayer) {
        EnPeehat_Flying_SetStateLanding(this);
    } else if (this->actor.xzDistToPlayer < this->xzDistMax) {
        if (this->unk_2FA < MAX_LARVA && (play->gameplayFrames & 7) == 0) {
            Actor* larva = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, 0x1d,
                                              Rand_CenteredFloat(25.0f) + this->actor.world.pos.x,
                                              Rand_CenteredFloat(5.0f) + this->actor.world.pos.y,
                                              Rand_CenteredFloat(25.0f) + this->actor.world.pos.z, 0, 0, 0, 1);
            if (larva != NULL) {
                larva->shape.rot.y = larva->world.rot.y = Rand_CenteredFloat(0xFFFF);
                this->unk_2FA++;
            }
        }
    }
    this->bladeRot += this->bladeRotVel;
}

void EnPeehat_Ground_SetStateRise(EnPeehat* this) {
    f32 lastFrame = Animation_GetLastFrame(&gPeehatRisingAnim);

    if (this->state != PEAHAT_STATE_STUNNED) {
        Animation_Change(&this->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    }
    this->state = PEAHAT_STATE_8;
    this->animTimer = lastFrame;
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_UP);
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateRise);
}

void EnPeehat_Ground_StateRise(EnPeehat* this, PlayState* play) {
    Vec3f pos;

    Math_SmoothStepToF(&this->actor.shape.yOffset, 0.0f, 1.0f, 50.0f, 0.0f);
    if (Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 800, 0) == 0) {
        if (this->animTimer != 0) {
            this->animTimer--;
            if (this->skelAnime.playSpeed == 0.0f) {
                if (this->animTimer == 0) {
                    this->animTimer = 40;
                    this->skelAnime.playSpeed = 1.0f;
                }
            }
        }
        if (SkelAnime_Update(&this->skelAnime) || this->animTimer == 0) {
            EnPeehat_Ground_SetStateHover(this);
        } else {
            this->actor.world.pos.y += 6.5f;
        }
        if (this->actor.world.pos.y - this->actor.floorHeight < 80.0f) {
            pos = this->actor.world.pos;
            pos.y = this->actor.floorHeight;
            func_80033480(play, &pos, 90.0f, 1, 0x96, 100, 1);
        }
    }
    EnPeehat_SpawnDust(play, this, &this->actor.world.pos, 75.0f, 2, 1.05f, 2.0f);
    Math_SmoothStepToF(&this->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    this->bladeRot += this->bladeRotVel;
}

void EnPeehat_Flying_SetStateRise(EnPeehat* this) {
    f32 lastFrame;

    lastFrame = Animation_GetLastFrame(&gPeehatRisingAnim);
    if (this->state != PEAHAT_STATE_STUNNED) {
        Animation_Change(&this->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    }
    this->state = PEAHAT_STATE_9;
    this->animTimer = lastFrame;
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_UP);
    EnPeehat_SetupAction(this, EnPeehat_Flying_StateRise);
}

void EnPeehat_Flying_StateRise(EnPeehat* this, PlayState* play) {
    Vec3f pos;

    Math_SmoothStepToF(&this->actor.shape.yOffset, 0.0f, 1.0f, 50.0f, 0.0f);
    if (Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 800, 0) == 0) {
        if (this->animTimer != 0) {
            this->animTimer--;
            if (this->skelAnime.playSpeed == 0.0f) {
                if (this->animTimer == 0) {
                    this->animTimer = 40;
                    this->skelAnime.playSpeed = 1.0f;
                }
            }
        }
        if (SkelAnime_Update(&this->skelAnime) || this->animTimer == 0) {

            EnPeehat_Flying_SetStateFly(this);
        } else {
            this->actor.world.pos.y += 18.0f;
        }
        if (this->actor.world.pos.y - this->actor.floorHeight < 80.0f) {
            pos = this->actor.world.pos;
            pos.y = this->actor.floorHeight;
            func_80033480(play, &pos, 90.0f, 1, 0x96, 100, 1);
        }
    }
    EnPeehat_SpawnDust(play, this, &this->actor.world.pos, 75.0f, 2, 1.05f, 2.0f);
    Math_SmoothStepToF(&this->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    this->bladeRot += this->bladeRotVel;
}

void EnPeehat_Ground_SetStateSeekPlayer(EnPeehat* this) {
    Animation_PlayLoop(&this->skelAnime, &gPeehatFlyingAnim);
    this->state = PEAHAT_STATE_SEEK_PLAYER;
    this->unk_2E0 = 0.0f;
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateSeekPlayer);
}

void EnPeehat_Ground_StateSeekPlayer(EnPeehat* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speed, 3.0f, 1.0f, 0.25f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight + 80.0f, 1.0f, 3.0f, 0.0f);
    if (this->seekPlayerTimer <= 0) {
        EnPeehat_Ground_SetStateLanding(this);
        this->riseDelayTimer = 40;
    } else {
        this->seekPlayerTimer--;
    }
    if (IS_DAY && (Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < this->xzDistMax)) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 1000, 0);
        if (this->unk_2FA != 0) {
            this->actor.shape.rot.y += 0x1C2;
        } else {
            this->actor.shape.rot.y -= 0x1C2;
        }
    } else {
        EnPeehat_Ground_SetStateReturnHome(this);
    }
    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 500, 0);
    this->bladeRot += this->bladeRotVel;
    Math_SmoothStepToF(&this->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_Larva_SetStateSeekPlayer(EnPeehat* this) {
    Animation_PlayLoop(&this->skelAnime, &gPeehatFlyingAnim);
    this->state = PEAHAT_STATE_SEEK_PLAYER;
    this->unk_2D4 = 0;
    EnPeehat_SetupAction(this, EnPeehat_Larva_StateSeekPlayer);
}

void EnPeehat_Larva_StateSeekPlayer(EnPeehat* this, PlayState* play) {
    f32 speedXZ = 5.3f;

    if (this->actor.xzDistToPlayer <= 5.3f) {
        speedXZ = this->actor.xzDistToPlayer + 0.0005f;
    }
    if (this->actor.parent != NULL && this->actor.parent->update == NULL) {
        this->actor.parent = NULL;
    }
    this->actor.speed = speedXZ;
    if (this->actor.world.pos.y - this->actor.floorHeight >= 70.0f) {
        Math_SmoothStepToF(&this->actor.velocity.y, -1.3f, 1.0f, 0.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->actor.velocity.y, -0.135f, 1.0f, 0.05f, 0.0f);
    }
    if (this->unk_2D4 == 0) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 830, 0);
    } else {
        this->unk_2D4--;
    }
    this->actor.shape.rot.y += 0x15E;
    SkelAnime_Update(&this->skelAnime);
    Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 500, 0);
    this->bladeRot += this->bladeRotVel;
    Math_SmoothStepToF(&this->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_SM_FLY - SFX_FLAG);
    if (this->colliderQuad.base.atFlags & AT_BOUNCED) {
        this->actor.colChkInfo.health = 0;
        this->colliderQuad.base.acFlags &= ~AC_BOUNCED;
        EnPeehat_SetStateAttackRecoil(this);
    } else if ((this->colliderQuad.base.atFlags & AT_HIT) || (this->colliderCylinder.base.acFlags & AC_HIT) ||
               (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        Player* player = GET_PLAYER(play);
        this->colliderQuad.base.atFlags &= ~AT_HIT;
        if (!(this->colliderCylinder.base.acFlags & AC_HIT) && &player->actor == this->colliderQuad.base.at) {
            if (Rand_ZeroOne() > 0.5f) {
                this->actor.world.rot.y += 0x2000;
            } else {
                this->actor.world.rot.y -= 0x2000;
            }
            this->unk_2D4 = 40;
        } else if (this->colliderCylinder.base.acFlags & AC_HIT || this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
            Vec3f zeroVec = { 0, 0, 0 };
            s32 i;
            for (i = 4; i >= 0; i--) {
                Vec3f pos;
                pos.x = Rand_CenteredFloat(20.0f) + this->actor.world.pos.x;
                pos.y = Rand_CenteredFloat(10.0f) + this->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + this->actor.world.pos.z;
                EffectSsDeadDb_Spawn(play, &pos, &zeroVec, &zeroVec, 40, 7, 255, 255, 255, 255, 255, 0, 0, 1, 9, 1);
            }
        }
        if (&player->actor != this->colliderQuad.base.at || this->colliderCylinder.base.acFlags & AC_HIT) {
            if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
                EffectSsDeadSound_SpawnStationary(play, &this->actor.projectedPos, NA_SE_EN_PIHAT_SM_DEAD, 1, 1, 40);
            }
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_2, false));
            Actor_Kill(&this->actor);
        }
    }
}

void EnPeehat_Ground_SetStateLanding(EnPeehat* this) {
    this->state = PEAHAT_STATE_LANDING;
    Animation_PlayOnce(&this->skelAnime, &gPeehatLandingAnim);
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateLanding);
}

void EnPeehat_Ground_StateLanding(EnPeehat* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 50, 0);
    if (SkelAnime_Update(&this->skelAnime)) {
        EnPeehat_Ground_SetStateGround(this);
        this->actor.world.pos.y = this->actor.floorHeight;
        Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_LAND);
    } else if (this->actor.floorHeight < this->actor.world.pos.y) {
        Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight, 0.3f, 3.5f, 0.25f);
        if (this->actor.world.pos.y - this->actor.floorHeight < 60.0f) {
            Vec3f pos = this->actor.world.pos;
            pos.y = this->actor.floorHeight;
            func_80033480(play, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(play, this, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
    }
    Math_SmoothStepToS(&this->bladeRotVel, 0, 1, 100, 0);
    this->bladeRot += this->bladeRotVel;
}

void EnPeehat_Flying_SetStateLanding(EnPeehat* this) {
    Animation_PlayOnce(&this->skelAnime, &gPeehatLandingAnim);
    this->state = PEAHAT_STATE_LANDING;
    EnPeehat_SetupAction(this, EnPeehat_Flying_StateLanding);
}

void EnPeehat_Flying_StateLanding(EnPeehat* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 50, 0);
    if (SkelAnime_Update(&this->skelAnime)) {
        EnPeehat_Flying_SetStateGround(this);
        Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_LAND);
        this->actor.world.pos.y = this->actor.floorHeight;
    } else if (this->actor.floorHeight < this->actor.world.pos.y) {
        Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight, 0.3f, 13.5f, 0.25f);
        if (this->actor.world.pos.y - this->actor.floorHeight < 60.0f) {
            Vec3f pos = this->actor.world.pos;
            pos.y = this->actor.floorHeight;
            func_80033480(play, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(play, this, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
    }
    Math_SmoothStepToS(&this->bladeRotVel, 0, 1, 100, 0);
    this->bladeRot += this->bladeRotVel;
}

void EnPeehat_Ground_SetStateHover(EnPeehat* this) {
    Animation_PlayLoop(&this->skelAnime, &gPeehatFlyingAnim);
    this->actor.speed = Rand_ZeroOne() * 0.5f + 2.5f;
    this->unk_2D4 = Rand_ZeroOne() * 10 + 10;
    this->state = PEAHAT_STATE_15;
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateHover);
}

void EnPeehat_Ground_StateHover(EnPeehat* this, PlayState* play) {
    f32 cos;
    Player* player = GET_PLAYER(play);

    if (this->actor.world.pos.y - this->actor.floorHeight > 75.0f) {
        this->actor.world.pos.y -= 1.0f;
    }
    this->actor.world.pos.y += Math_CosF(this->unk_2E0) * 1.4f;
    cos = Math_CosF(this->unk_2E0) * 0.18f;
    this->unk_2E0 += ((0.0f <= cos) ? cos : -cos) + 0.07f;
    this->unk_2D4--;
    if (this->unk_2D4 <= 0) {
        this->actor.speed = Rand_ZeroOne() * 0.5f + 2.5f;
        this->unk_2D4 = Rand_ZeroOne() * 10.0f + 10.0f;
        this->unk_2F4 = (Rand_ZeroOne() - 0.5f) * 1000.0f;
    }
    SkelAnime_Update(&this->skelAnime);
    this->actor.world.rot.y += this->unk_2F4;
    if (this->seekPlayerTimer <= 0) {
        EnPeehat_Ground_SetStateLanding(this);
        this->riseDelayTimer = 40;
    } else {
        this->seekPlayerTimer--;
    }
    this->actor.shape.rot.y += 0x15E;

    if (IS_DAY && Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < this->xzDistMax) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        EnPeehat_Ground_SetStateSeekPlayer(this);
        this->unk_2FA = play->gameplayFrames & 1;
    } else {
        EnPeehat_Ground_SetStateReturnHome(this);
    }
    Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 500, 0);
    this->bladeRot += this->bladeRotVel;
    Math_SmoothStepToF(&this->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_Ground_SetStateReturnHome(EnPeehat* this) {
    this->state = PEAHAT_STATE_RETURN_HOME;
    this->actor.speed = 2.5f;
    EnPeehat_SetupAction(this, EnPeehat_Ground_StateReturnHome);
}

void EnPeehat_Ground_StateReturnHome(EnPeehat* this, PlayState* play) {
    f32 cos;
    s16 yRot;
    Player* player;

    player = GET_PLAYER(play);
    if (this->actor.world.pos.y - this->actor.floorHeight > 75.0f) {
        this->actor.world.pos.y -= 1.0f;
    } else {
        this->actor.world.pos.y += 1.0f;
    }
    this->actor.world.pos.y += Math_CosF(this->unk_2E0) * 1.4f;
    cos = Math_CosF(this->unk_2E0) * 0.18f;
    this->unk_2E0 += ((0.0f <= cos) ? cos : -cos) + 0.07f;
    yRot = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
    Math_SmoothStepToS(&this->actor.world.rot.y, yRot, 1, 600, 0);
    Math_SmoothStepToS(&this->actor.shape.rot.x, 4500, 1, 600, 0);
    this->actor.shape.rot.y += 0x15E;
    this->bladeRot += this->bladeRotVel;
    if (Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos) < 2.0f) {
        EnPeehat_Ground_SetStateLanding(this);
        this->riseDelayTimer = 60;
    }
    if (IS_DAY && Math_Vec3f_DistXZ(&this->actor.home.pos, &player->actor.world.pos) < this->xzDistMax) {
        this->seekPlayerTimer = 400;
        EnPeehat_Ground_SetStateSeekPlayer(this);
        this->unk_2FA = (play->gameplayFrames & 1);
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_SetStateAttackRecoil(EnPeehat* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gPeehatRecoilAnim, -4.0f);
    this->state = PEAHAT_STATE_ATTACK_RECOIL;
    this->actor.speed = -9.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    EnPeehat_SetupAction(this, EnPeehat_StateAttackRecoil);
}

void EnPeehat_StateAttackRecoil(EnPeehat* this, PlayState* play) {
    this->bladeRot += this->bladeRotVel;
    SkelAnime_Update(&this->skelAnime);
    this->actor.speed += 0.5f;
    if (this->actor.speed == 0.0f) {

        if (this->actor.params > 0) {
            Vec3f zeroVec = { 0, 0, 0 };
            s32 i;
            for (i = 4; i >= 0; i--) {
                Vec3f pos;
                pos.x = Rand_CenteredFloat(20.0f) + this->actor.world.pos.x;
                pos.y = Rand_CenteredFloat(10.0f) + this->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + this->actor.world.pos.z;
                EffectSsDeadDb_Spawn(play, &pos, &zeroVec, &zeroVec, 40, 7, 255, 255, 255, 255, 255, 0, 0, 1, 9, 1);
            }
            Actor_Kill(&this->actor);
        } else {
            EnPeehat_Ground_SetStateSeekPlayer(this);

            if (this->actor.params < 0) {
                this->unk_2FA = (this->unk_2FA != 0) ? 0 : 1;
            }
        }
    }
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_SetStateBoomerangStunned(EnPeehat* this) {
    this->state = PEAHAT_STATE_STUNNED;
    if (this->actor.floorHeight < this->actor.world.pos.y) {
        this->actor.speed = -9.0f;
    }
    this->bladeRotVel = 0;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 200, COLORFILTER_BUFFLAG_OPA, 80);
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnPeehat_SetupAction(this, EnPeehat_StateBoomerangStunned);
}

void EnPeehat_StateBoomerangStunned(EnPeehat* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight, 1.0f, 8.0f, 0.0f);
    if (this->actor.colorFilterTimer == 0) {
        EnPeehat_Ground_SetStateRise(this);
    }
}

void EnPeehat_Adult_SetStateDie(EnPeehat* this) {
    this->bladeRotVel = 0;
    this->isStateDieFirstUpdate = 1;
    this->actor.speed = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    this->state = PEAHAT_STATE_DYING;
    this->scaleShift = 0.0f;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    EnPeehat_SetupAction(this, EnPeehat_Adult_StateDie);
}

void EnPeehat_Adult_StateDie(EnPeehat* this, PlayState* play) {
    if (this->isStateDieFirstUpdate) {
        this->unk_2D4--;
        if (this->unk_2D4 <= 0 || this->actor.colChkInfo.health == 0) {
            Animation_MorphToPlayOnce(&this->skelAnime, &gPeehatRecoilAnim, -4.0f);
            this->bladeRotVel = 4000;
            this->unk_2D4 = 14;
            this->actor.speed = 0;
            this->actor.velocity.y = 6;
            this->isStateDieFirstUpdate = 0;
            this->actor.shape.rot.z = this->actor.shape.rot.x = 0;
        } else if (this->actor.colorFilterTimer & 4) {
            Math_SmoothStepToF(&this->scaleShift, 0.205f, 1.0f, 0.235f, 0);
        } else {
            Math_SmoothStepToF(&this->scaleShift, 0, 1.0f, 0.005f, 0);
        }
    } else {
        SkelAnime_Update(&this->skelAnime);
        this->bladeRot += this->bladeRotVel;
        Math_SmoothStepToS(&this->bladeRotVel, 4000, 1, 250, 0);
        if (this->actor.colChkInfo.health == 0) {
            this->actor.scale.x -= 0.0015f;
            Actor_SetScale(&this->actor, this->actor.scale.x);
        }
        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight + 88.5f, 1.0f, 3.0f, 0.0f) == 0.0f &&
            this->actor.world.pos.y - this->actor.floorHeight < 59.0f) {
            Vec3f pos = this->actor.world.pos;
            pos.y = this->actor.floorHeight;
            func_80033480(play, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(play, this, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
        if (this->actor.speed < 0) {
            this->actor.speed += 0.25f;
        }
        this->unk_2D4--;
        if (this->unk_2D4 <= 0) {
            if (this->actor.colChkInfo.health == 0) {
                EnPeehat_SetStateExplode(this);

            } else if (this->actor.params < 0) {
                EnPeehat_Ground_SetStateHover(this);
                this->riseDelayTimer = 60;
            } else {
                EnPeehat_Flying_SetStateFly(this);
            }
        }
    }
}

void EnPeehat_SetStateExplode(EnPeehat* this) {
    Animation_PlayLoop(&this->skelAnime, &gPeehatFlyingAnim);
    this->state = PEAHAT_STATE_EXPLODE;
    this->animTimer = 5;
    this->unk_2E0 = 0.0f;
    EnPeehat_SetupAction(this, EnPeehat_StateExplode);
}

void EnPeehat_StateExplode(EnPeehat* this, PlayState* play) {
    s32 pad[2];

    if (this->animTimer == 5) {
        EnBom* bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, 0x10, this->actor.world.pos.x, this->actor.world.pos.y,
                                          this->actor.world.pos.z, 0, 0, 0x602, 0);
        if (bomb != NULL) {
            bomb->timer = 0;
        }
    }
    this->animTimer--;
    if (this->animTimer == 0) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_4, false));
        Actor_Kill(&this->actor);
    }
}

void EnPeehat_Adult_CollisionCheck(EnPeehat* this, PlayState* play) {
    if ((this->colliderCylinder.base.acFlags & AC_BOUNCED) || (this->colliderQuad.base.acFlags & AC_BOUNCED)) {
        this->colliderQuad.base.acFlags &= ~AC_BOUNCED;
        this->colliderCylinder.base.acFlags &= ~AC_BOUNCED;
        this->colliderJntSph.base.acFlags &= ~AC_HIT;
    } else if (this->colliderJntSph.base.acFlags & AC_HIT) {
        this->colliderJntSph.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&this->actor, &this->colliderJntSph, true);
        if (this->actor.colChkInfo.damageEffect == PEAHAT_DMG_REACT_NUT ||
            this->actor.colChkInfo.damageEffect == PEAHAT_DMG_REACT_LIGHT_ICE_ARROW) {
            return;
        }
        if (this->actor.colChkInfo.damageEffect == PEAHAT_DMG_REACT_HOOKSHOT) {
            this->actor.colChkInfo.health = 0;
        } else if (this->actor.colChkInfo.damageEffect == PEAHAT_DMG_REACT_BOOMERANG) {
            if (this->state != PEAHAT_STATE_STUNNED) {
                EnPeehat_SetStateBoomerangStunned(this);
            }
            return;
        } else {
            Actor_ApplyDamage(&this->actor);
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
            Actor_PlaySfx(&this->actor, NA_SE_EN_PIHAT_DAMAGE);
        }

        if (this->actor.colChkInfo.damageEffect == PEAHAT_DMG_REACT_FIRE) {
            Vec3f pos;
            s32 i;
            for (i = 4; i >= 0; i--) {
                pos.x = Rand_CenteredFloat(20.0f) + this->actor.world.pos.x;
                pos.y = Rand_ZeroOne() * 25.0f + this->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + this->actor.world.pos.z;
                EffectSsEnFire_SpawnVec3f(play, &this->actor, &pos, 70, 0, 0, -1);
            }
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 200, COLORFILTER_BUFFLAG_OPA, 100);
        }
        if (this->actor.colChkInfo.health == 0) {
            EnPeehat_Adult_SetStateDie(this);
        }
    }
}

static void peahatLimitCeiling(Actor* actor, PlayState* play, Vec3f from)
{
    Vec3f to = actor->world.pos;
    Vec3f start = from;
    Vec3f end = to;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    const f32 clearance = 90.f;
    int movedXZ = from.x != to.x || from.z != to.z;

    if (movedXZ)
    {
        start.y += clearance;
        end.y += clearance;
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &hit, &poly,
                                    1, 0, 1, 0, &bgId))
        {
            actor->world.pos.x = from.x;
            actor->world.pos.z = from.z;
            if (actor->world.pos.y > from.y)
                actor->world.pos.y = from.y;
            if (actor->velocity.y > 0.f)
                actor->velocity.y = 0.f;
        }
    }

    if (actor->world.pos.y >= from.y)
    {
        start = actor->world.pos;
        start.y = from.y;
        end = actor->world.pos;
        end.y += clearance;
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &hit, &poly,
                                    0, 0, 1, 0, &bgId))
        {
            f32 limit = hit.y - clearance;
            if (limit < from.y)
                limit = from.y;
            if (actor->world.pos.y > limit)
                actor->world.pos.y = limit;
            if (actor->velocity.y > 0.f)
                actor->velocity.y = 0.f;
        }
    }
}

void EnPeehat_Update(Actor* thisx, PlayState* play) {
    EnPeehat* this = (EnPeehat*)thisx;
    s32 i;
    Player* player = GET_PLAYER(play);
    Vec3f posResult;
    CollisionPoly* poly;
    s32 bgId;
    Vec3f* posB;
    Vec3f from = thisx->world.pos;

    if (thisx->params <= 0) {
        EnPeehat_Adult_CollisionCheck(this, play);
    }
    if (thisx->colChkInfo.damageEffect != PEAHAT_DMG_REACT_LIGHT_ICE_ARROW) {
        if (thisx->speed != 0.0f || thisx->velocity.y != 0.0f) {
            Actor_MoveXZGravity(thisx);
            Actor_UpdateBgCheckInfo(play, thisx, 25.0f, 30.0f, 30.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        }

        this->actionFunc(this, play);
        if (thisx->update && thisx->params <= 0)
            peahatLimitCeiling(thisx, play, from);
        if ((play->gameplayFrames & 0x7F) == 0) {
            this->jiggleRotInc = (Rand_ZeroOne() * 0.25f) + 0.5f;
        }
        this->jiggleRot += this->jiggleRotInc;
    }

    if (thisx->params < 0) {

        thisx->focus.pos.x = this->colliderJntSph.elements[0].dim.worldSphere.center.x;
        thisx->focus.pos.y = this->colliderJntSph.elements[0].dim.worldSphere.center.y;
        thisx->focus.pos.z = this->colliderJntSph.elements[0].dim.worldSphere.center.z;
        if (this->state == PEAHAT_STATE_SEEK_PLAYER) {
            Math_SmoothStepToS(&thisx->shape.rot.x, 6000, 1, 300, 0);
        } else {
            Math_SmoothStepToS(&thisx->shape.rot.x, 0, 1, 300, 0);
        }
    } else {
        thisx->focus.pos = thisx->world.pos;
    }
    Collider_UpdateCylinder(thisx, &this->colliderCylinder);
    if (thisx->colChkInfo.health > 0) {

        if (thisx->params <= 0) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinder.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderJntSph.base);
            if (thisx->colorFilterTimer == 0 || !(thisx->colorFilterParams & 0x4000)) {
                if (this->state != PEAHAT_STATE_EXPLODE) {
                    CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderJntSph.base);
                }
            }
        }
        if (thisx->params != PEAHAT_TYPE_FLYING && this->colliderQuad.base.atFlags & AT_HIT) {
            this->colliderQuad.base.atFlags &= ~AT_HIT;
            if (&player->actor == this->colliderQuad.base.at) {
                EnPeehat_SetStateAttackRecoil(this);
            }
        }
    }
    if (this->state == PEAHAT_STATE_15 || this->state == PEAHAT_STATE_SEEK_PLAYER || this->state == PEAHAT_STATE_FLY ||
        this->state == PEAHAT_STATE_RETURN_HOME || this->state == PEAHAT_STATE_EXPLODE) {
        if (thisx->params != PEAHAT_TYPE_FLYING) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderQuad.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderQuad.base);
        }

        if (thisx->params < 0 && (thisx->flags & ACTOR_FLAG_INSIDE_CULLING_VOLUME)) {
            for (i = 1; i >= 0; i--) {
                poly = NULL;
                posB = &this->bladeTip[i];

                if (BgCheck_EntityLineTest1(&play->colCtx, &thisx->world.pos, posB, &posResult, &poly, true, true,
                                            false, true, &bgId) == true) {
                    func_80033480(play, &posResult, 0.0f, 1, 300, 150, 1);
                    EnPeehat_SpawnDust(play, this, &posResult, 0.0f, 3, 1.05f, 1.5f);
                }
            }
        } else if (thisx->params != PEAHAT_TYPE_FLYING) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
    }
    Math_SmoothStepToF(&this->scaleShift, 0.0f, 1.0f, 0.001f, 0.0f);
}

s32 EnPeehat_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnPeehat* this = (EnPeehat*)thisx;

    if (limbIndex == 4) {
        rot->x = -this->bladeRot;
    }
    if (limbIndex == 3 || (limbIndex == 23 && (this->state == PEAHAT_STATE_DYING || this->state == PEAHAT_STATE_3 ||
                                               this->state == PEAHAT_STATE_4))) {
        OPEN_DISPS(play->state.gfxCtx);
        Matrix_Push();
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateX(this->jiggleRot * 0.115f, MTXMODE_APPLY);
        Matrix_RotateY(this->jiggleRot * 0.13f, MTXMODE_APPLY);
        Matrix_RotateZ(this->jiggleRot * 0.1f, MTXMODE_APPLY);
        Matrix_Scale(1.0f - this->scaleShift, this->scaleShift + 1.0f, 1.0f - this->scaleShift, MTXMODE_APPLY);
        Matrix_RotateZ(-(this->jiggleRot * 0.1f), MTXMODE_APPLY);
        Matrix_RotateY(-(this->jiggleRot * 0.13f), MTXMODE_APPLY);
        Matrix_RotateX(-(this->jiggleRot * 0.115f), MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
        CLOSE_DISPS();
        return true;
    }
    return false;
}

void EnPeehat_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f peahatBladeTip[] = { { 0.0f, 0.0f, 5500.0f }, { 0.0f, 0.0f, -5500.0f } };

    EnPeehat* this = (EnPeehat*)thisx;
    f32 damageYRot;

    if (limbIndex == 4) {
        Matrix_MultVec3f(&peahatBladeTip[0], &this->bladeTip[0]);
        Matrix_MultVec3f(&peahatBladeTip[1], &this->bladeTip[1]);
        return;
    }

    if (limbIndex == 3 && this->actor.params <= 0) {
        damageYRot = 0.0f;
        OPEN_DISPS(play->state.gfxCtx);
        Matrix_Push();
        Matrix_Translate(-1000.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Collider_UpdateSpheres(0, &this->colliderJntSph);
        Matrix_Translate(500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        if (this->actor.colorFilterTimer != 0 && (this->actor.colorFilterParams & 0x4000)) {
            damageYRot = Math_SinS(this->actor.colorFilterTimer * 0x4E20) * 0.35f;
        }
        Matrix_RotateY(3.2f + damageYRot, MTXMODE_APPLY);
        Matrix_Scale(0.3f, 0.2f, 0.2f, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
        CLOSE_DISPS();
    }
}

void EnPeehat_Draw(Actor* thisx, PlayState* play) {
    static Vec3f D_80AD285C[] = {
        { 0.0f, 0.0f, -4500.0f }, { -4500.0f, 0.0f, 0.0f }, { 4500.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 4500.0f }
    };
    EnPeehat* this = (EnPeehat*)thisx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnPeehat_OverrideLimbDraw,
                      EnPeehat_PostLimbDraw, this);
    if (this->actor.speed != 0.0f || this->actor.velocity.y != 0.0f) {
        Matrix_MultVec3f(&D_80AD285C[0], &this->colliderQuad.dim.quad[1]);
        Matrix_MultVec3f(&D_80AD285C[1], &this->colliderQuad.dim.quad[0]);
        Matrix_MultVec3f(&D_80AD285C[2], &this->colliderQuad.dim.quad[3]);
        Matrix_MultVec3f(&D_80AD285C[3], &this->colliderQuad.dim.quad[2]);
        Collider_SetQuadVertices(&this->colliderQuad, &this->colliderQuad.dim.quad[0], &this->colliderQuad.dim.quad[1],
                                 &this->colliderQuad.dim.quad[2], &this->colliderQuad.dim.quad[3]);
    }
}

OVL_INFO_NATIVE_ENEMY(0x1d, En_Peehat_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_INIT, EnPeehat_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_DESTROY, EnPeehat_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_UPDATE, EnPeehat_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_DRAW, EnPeehat_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_PROFILE, En_Peehat_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x1d, 0x18, 0xaf1d8462 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_PEEHAT_ORIGIN, kNativePortOrigin);
