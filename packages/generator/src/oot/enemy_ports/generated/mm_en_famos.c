#include "mm_native_compat.h"
#define FAMOS_OOT_DEBRIS_DL 0x0400c0d0
#define gFamosShakeAnim (*(AnimationHeader*)0x060000f8)
#define gFamosSkel (*(SkeletonHeader*)0x06003d38)
#define gFamosIdleAnim (*(AnimationHeader*)0x06003dc8)
#define gFamosNormalGlowingEmblemTexAnim (*((u8 (*)[1])0x06003e30))
#define gFamosFlippedGlowingEmblemTexAnim (*((u8 (*)[1])0x06003e38))

#include "types/en_famos.h"
#define TATL_HINT_ID_DEATH_ARMOS 0xff
#undef ACTOR_EN_BOM
#define ACTOR_EN_BOM 0x9
#undef ACTOR_EN_FAMOS
#define ACTOR_EN_FAMOS 0x2d
#undef OBJECT_FAMOS
#define OBJECT_FAMOS 0x101d
#undef NA_SE_EV_EXPLOSION
#define NA_SE_EV_EXPLOSION 0x2802
#undef NA_SE_EN_EYEGOLE_DEAD
#define NA_SE_EN_EYEGOLE_DEAD 0x3846
#undef NA_SE_EN_EYEGOLE_DAMAGE
#define NA_SE_EN_EYEGOLE_DAMAGE 0x3847
#undef NA_SE_EN_AMOS_VOICE
#define NA_SE_EN_AMOS_VOICE 0x3848
#undef NA_SE_EN_LAST1_FALL_OLD
#define NA_SE_EN_LAST1_FALL_OLD 0x39d7
#undef NA_SE_EN_FAMOS_REVERSE1
#define NA_SE_EN_FAMOS_REVERSE1 0x3a91
#undef NA_SE_EN_FAMOS_REVERSE2
#define NA_SE_EN_FAMOS_REVERSE2 0x3a92
#undef NA_SE_EN_FAMOS_FLOAT
#define NA_SE_EN_FAMOS_FLOAT 0x3a93
#undef NA_SE_EN_FAMOS_FLOAT_REVERSE
#define NA_SE_EN_FAMOS_FLOAT_REVERSE 0x3a94
#undef Lib_SegmentedToVirtual
#define Lib_SegmentedToVirtual ((void* (*)(void* ptr))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_SpawnAsChild
#define Actor_SpawnAsChild ((Actor* (*)(ActorContext* actorCtx, Actor* parent, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN_CHILD)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef AnimatedMat_Draw
#define AnimatedMat_Draw ((void (*)(struct PlayState* play, AnimatedMaterial* matAnim))(void*)FOREIGN_ENEMY_API_MATERIAL_DRAW)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(struct PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c684u)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 angle))0x80021a28u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_Vec3s_ToVec3f
#define Math_Vec3s_ToVec3f ((void (*)(Vec3f* dest, Vec3s* src))0x80063cccu)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Actor_WorldPitchTowardPoint
#define Actor_WorldPitchTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x80021488u)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef SurfaceType_GetFloorProperty2
#define SurfaceType_GetFloorProperty2 ((FloorProperty (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x80034518u)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Camera_AddQuake
#define Camera_AddQuake ((s32 (*)(Camera* camera, s32 arg1, s16 y, s32 countdown))0x800497a4u)
#undef Rumble_Request
#define Rumble_Request ((void (*)(f32 distSq, u8 sourceIntensity, u8 decayTimer, u8 decayStep))0x800915ccu)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Math_Vec3f_Sum
#define Math_Vec3f_Sum ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d10u)
#undef Actor_MoveWithoutGravity
#define Actor_MoveWithoutGravity ((void (*)(Actor* actor))0x800212b8u)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Matrix_SetTranslateRotateYXZ
#define Matrix_SetTranslateRotateYXZ ((void (*)(f32 x, f32 y, f32 z, Vec3s* rot))0x800ab510u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x8008993cu)
#undef func_800AE5A0
#define func_800AE5A0 ((void (*)(struct PlayState* play))0x8001a468u)
typedef u32 FloorProperty;
#define FLOOR_PROPERTY_12 12
#define FLOOR_PROPERTY_13 13

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnFamos_Init(Actor* thisx, PlayState* play);
void EnFamos_Destroy(Actor* thisx, PlayState* play);
void EnFamos_Update(Actor* thisx, PlayState* play);
void EnFamos_Draw(Actor* thisx, PlayState* play);

void EnFamos_SetupStillIdle(EnFamos* this);
void EnFamos_StillIdle(EnFamos* this, PlayState* play);
void EnFamos_SetupPathingIdle(EnFamos* this);
void EnFamos_PathingIdle(EnFamos* this, PlayState* play);
void EnFamos_SetupTurnHome(EnFamos* this);
void EnFamos_TurnHome(EnFamos* this, PlayState* play);
void EnFamos_SetupReturnHome(EnFamos* this);
void EnFamos_ReturnHome(EnFamos* this, PlayState* play);
void EnFamos_SetupAlert(EnFamos* this);
void EnFamos_Alert(EnFamos* this, PlayState* play);
void EnFamos_SetupChase(EnFamos* this);
void EnFamos_Chase(EnFamos* this, PlayState* play);
void EnFamos_SetupAttackAim(EnFamos* this);
void EnFamos_AttackAim(EnFamos* this, PlayState* play);
void EnFamos_SetupAttack(EnFamos* this);
void EnFamos_Attack(EnFamos* this, PlayState* play);
void EnFamos_SetupFinishAttack(EnFamos* this);
void EnFamos_FinishAttack(EnFamos* this, PlayState* play);
void EnFamos_SetupAttackRebound(EnFamos* this);
void EnFamos_AttackRebound(EnFamos* this, PlayState* play);
void EnFamos_SetupScanForPlayer(EnFamos* this);
void EnFamos_ScanForPlayer(EnFamos* this, PlayState* play);
void EnFamos_SetupDeathSlam(EnFamos* this);
void EnFamos_DeathSlam(EnFamos* this, PlayState* play);
void EnFamos_SetupDeathExplosion(EnFamos* this);
void EnFamos_DeathExplosion(EnFamos* this, PlayState* play);
void EnFamos_SetupDeathFade(EnFamos* this);
void EnFamos_DeathFade(EnFamos* this, PlayState* play);

ActorProfile En_Famos_Profile = {
    0xc2d, ACTORCAT_ENEMY, FLAGS, 0x101d, sizeof(EnFamos), EnFamos_Init, EnFamos_Destroy, EnFamos_Update, EnFamos_Draw,
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COL_MATERIAL_METAL,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x20000000, 0x04, 0x10 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 20, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
    {
        COL_MATERIAL_NONE,
        AT_NONE | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x04, 0x08 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { 70, 10, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00002000, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { FAMOS_LIMB_EMBLEM, { { 2500, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00002000, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { FAMOS_LIMB_EMBLEM, { { -1500, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

typedef enum { FAMOS_ANIMATED_MAT_NORMAL, FAMOS_ANIMATED_MAT_FLIPPED } FamosAnimatedMatArrayIndexes;

static AnimatedMaterial* sEmblemAnimatedMats[] = {
    (AnimatedMaterial*)gFamosNormalGlowingEmblemTexAnim,
    (AnimatedMaterial*)gFamosFlippedGlowingEmblemTexAnim,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, TATL_HINT_ID_DEATH_ARMOS, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3500, ICHAIN_STOP),
};

static s32 sAnimatedMaterialsDesgmented = false;

void EnFamos_Init(Actor* thisx, PlayState* play) {
    if (thisx->params != 0xff) {
        Actor_Kill(thisx);
        return;
    }
    EnFamos* this = (EnFamos*)thisx;
    Path* path;
    s32 i;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (FAMOS_GET_PATH_INDEX(&this->actor) != FAMOS_PATH_INDEX_NONE) {
        path = &play->pathList[FAMOS_GET_PATH_INDEX(&this->actor)];
        this->pathPoints = Lib_SegmentedToVirtual(path->points);
        this->pathCount = path->count;
        if (this->pathCount == 1) {
            this->pathPoints = NULL;
            this->pathCount = 0;
        }
    }

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_Init(play, &this->skelAnime, &gFamosSkel, &gFamosIdleAnim, this->jointTable, this->morphTable,
                   FAMOS_LIMB_MAX);
    Collider_InitAndSetCylinder(play, &this->collider1, &this->actor, &sCylinderInit1);
    Collider_InitAndSetCylinder(play, &this->collider2, &this->actor, &sCylinderInit2);
    Collider_InitAndSetJntSph(play, &this->emblemCollider, &this->actor, &sJntSphInit, this->emblemColliderElements);

    if (!sAnimatedMaterialsDesgmented) {
        for (i = 0; i < ARRAY_COUNT(sEmblemAnimatedMats); i++) {
            sEmblemAnimatedMats[i] = Lib_SegmentedToVirtual(sEmblemAnimatedMats[i]);
        }
        sAnimatedMaterialsDesgmented = true;
    }

    this->actor.colChkInfo.mass = 250;
    this->baseHeight = this->actor.world.pos.y;
    this->aggroDistance = (FAMOS_GET_AGGRO_DISTANCE(thisx) <= 0) ? (200.0f) : (this->actor.shape.rot.x * 40.0f * 0.1f);
    this->actor.shape.rot.x = 0;
    this->actor.world.rot.x = 0;
    this->hasFinishedRotating = true;
    this->isCalm = true;
    if (this->pathPoints != NULL) {
        EnFamos_SetupPathingIdle(this);
    } else {
        EnFamos_SetupStillIdle(this);
    }
}

void EnFamos_Destroy(Actor* thisx, PlayState* play) {
    EnFamos* this = (EnFamos*)thisx;

    Collider_DestroyCylinder(play, &this->collider1);
    Collider_DestroyCylinder(play, &this->collider2);
    Collider_DestroyJntSph(play, &this->emblemCollider);
}

void EnFamos_SetupAttackDebris(EnFamos* this) {
    EnFamosRock* rock;
    f32 randFloat;
    s16 randOffset;
    s16 randVelDirection;
    s32 i;

    this->debrisTimer = 40;
    rock = &this->rocks[0];
    for (i = 0; i < ARRAY_COUNT(this->rocks); i++, rock++) {
        randVelDirection = (s32)Rand_Next() >> 0x10;
        randOffset = Rand_S16Offset(0x1800, 0x2800);
        randFloat = Rand_ZeroFloat(5.0f) + 5.0f;
        rock->velocity.x = randFloat * Math_CosS(randOffset) * Math_SinS(randVelDirection);
        rock->velocity.y = Math_SinS(randOffset) * randFloat + 3.0f;
        rock->velocity.z = randFloat * Math_CosS(randOffset) * Math_CosS(randVelDirection);
        rock->rot.x = (s32)Rand_Next() >> 0x10;
        rock->rot.y = (s32)Rand_Next() >> 0x10;
        rock->rot.z = (s32)Rand_Next() >> 0x10;
        rock->pos.x = (Math_SinS(randVelDirection) * 20.0f) + this->actor.world.pos.x;
        rock->pos.y = this->actor.floorHeight;
        rock->pos.z = (Math_CosS(randVelDirection) * 20.0f) + this->actor.world.pos.z;
        rock->scale = Rand_ZeroFloat(0.0015f) + (2.0f * 0.001f);
    }
}

void EnFamos_SetupDeathDebris(EnFamos* this) {
    f32 randFloat;
    s16 randSmaller;
    s16 randVelDirection;
    EnFamosRock* rock;
    s32 i;

    this->debrisTimer = 40;
    rock = &this->rocks[0];
    for (i = 0; i < ARRAY_COUNT(this->rocks); i++, rock++) {
        randVelDirection = (s32)Rand_Next() >> 0x10;
        randSmaller = Rand_Next() >> 0x12;
        randFloat = Rand_ZeroFloat(6.0f) + 7.0f;
        rock->velocity.x = randFloat * Math_CosS(randSmaller) * Math_SinS(randVelDirection);
        rock->velocity.y = Math_SinS(randSmaller) * randFloat + 4.5f;
        rock->velocity.z = randFloat * Math_CosS(randSmaller) * Math_CosS(randVelDirection);
        rock->rot.x = (s32)Rand_Next() >> 0x10;
        rock->rot.y = (s32)Rand_Next() >> 0x10;
        rock->rot.z = (s32)Rand_Next() >> 0x10;
        rock->pos.x = Math_SinS(randVelDirection) * 20.0f + this->actor.world.pos.x;
        rock->pos.y = Rand_CenteredFloat(60.0f) + (this->actor.world.pos.y + 40.0f);
        rock->pos.z = Math_CosS(randVelDirection) * 20.0f + this->actor.world.pos.z;
        rock->scale = Rand_ZeroFloat(0.002f) + (2.5f * 0.001f);
    }
}

s32 EnFamos_IsPlayerSeen(EnFamos* this, PlayState* play) {
    if ((Player_GetMask(play) != PLAYER_MASK_STONE) &&
        (Actor_WorldDistXZToPoint(&GET_PLAYER(play)->actor, &this->calmPos) < this->aggroDistance) &&
        Actor_IsFacingPlayer(&this->actor, 0x5000)) {
        return true;
    } else {
        return false;
    }
}

void EnFamos_UpdateBobbingHeight(EnFamos* this) {
    if (this->hoverTimer == 0) {
        this->hoverTimer = 30;
    }

    this->hoverTimer--;
    this->actor.world.pos.y = (Math_SinS(this->hoverTimer * 0x888) * 10.0f) + this->baseHeight;

    if (ABS_ALT(this->flipRot) > 0x4000) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT_REVERSE - SFX_FLAG);
    } else {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT - SFX_FLAG);
    }
}

void EnFamos_UpdateFlipStatus(EnFamos* this) {
    u8 hasFinishedRotating;

    if (this->emblemCollider.base.acFlags & AC_HIT) {
        this->emblemCollider.base.acFlags &= ~AC_HIT;
        if (this->hasFinishedRotating == true) {
            if (this->animatedMaterialIndex != FAMOS_ANIMATED_MAT_NORMAL) {
                this->animatedMaterialIndex = FAMOS_ANIMATED_MAT_NORMAL;
                Actor_PlaySfx(&this->actor, NA_SE_EN_FAMOS_REVERSE2);
            } else {
                this->animatedMaterialIndex = FAMOS_ANIMATED_MAT_FLIPPED;
                this->flippedTimer = 100;
                Actor_PlaySfx(&this->actor, NA_SE_EN_FAMOS_REVERSE1);
                Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_DAMAGE);
            }
            this->hasFinishedRotating = false;
        }

    } else if (this->flippedTimer > 0) {
        this->flippedTimer--;
        if (this->flippedTimer == 0) {
            if (this->animatedMaterialIndex != FAMOS_ANIMATED_MAT_NORMAL) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_FAMOS_REVERSE2);
            }

            this->animatedMaterialIndex = FAMOS_ANIMATED_MAT_NORMAL;
            this->hasFinishedRotating = false;
        }
    }

    if (this->hasFinishedRotating == false) {
        if (this->animatedMaterialIndex != FAMOS_ANIMATED_MAT_NORMAL) {
            hasFinishedRotating = Math_ScaledStepToS(&this->flipRot, -0x8000, 0x1000);
        } else {
            hasFinishedRotating = Math_ScaledStepToS(&this->flipRot, 0, 0x1000);
        }

        this->hasFinishedRotating = hasFinishedRotating;
    }
}

void EnFamos_SetupStillIdle(EnFamos* this) {
    this->actionFunc = EnFamos_StillIdle;
    this->actor.speed = 0.0f;
}

void EnFamos_StillIdle(EnFamos* this, PlayState* play) {
    EnFamos_UpdateBobbingHeight(this);
    if (this->isCalm) {
        Math_Vec3f_Copy(&this->calmPos, &this->actor.world.pos);
    }
    if (EnFamos_IsPlayerSeen(this, play)) {
        EnFamos_SetupAlert(this);
    }
}

void EnFamos_SetupPathingIdle(EnFamos* this) {
    if (this->isCalm) {
        this->waypointIndex++;
        if (this->waypointIndex == this->pathCount) {
            this->waypointIndex = 0;
        }
    } else {
        this->isCalm = true;
    }

    Math_Vec3s_ToVec3f(&this->targetDest, &this->pathPoints[this->waypointIndex]);
    this->targetYaw = Actor_WorldYawTowardPoint(&this->actor, &this->targetDest);
    this->actionFunc = EnFamos_PathingIdle;
    this->actor.speed = 0.0f;
}

void EnFamos_PathingIdle(EnFamos* this, PlayState* play) {
    EnFamos_UpdateBobbingHeight(this);
    if (this->isCalm) {
        Math_Vec3f_Copy(&this->calmPos, &this->actor.world.pos);
    }

    if (EnFamos_IsPlayerSeen(this, play)) {
        EnFamos_SetupAlert(this);
    } else if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->targetYaw, 0x200)) {
        EnFamos_SetupReturnHome(this);
    }
}

void EnFamos_SetupTurnHome(EnFamos* this) {
    this->targetYaw = Actor_WorldYawTowardPoint(&this->actor, &this->calmPos);
    Math_Vec3f_Copy(&this->targetDest, &this->calmPos);
    this->actionFunc = EnFamos_TurnHome;
    this->actor.speed = 0.0f;
}

void EnFamos_TurnHome(EnFamos* this, PlayState* play) {
    EnFamos_UpdateBobbingHeight(this);
    if (EnFamos_IsPlayerSeen(this, play)) {
        EnFamos_SetupAlert(this);
    } else if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->targetYaw, 0x200)) {
        EnFamos_SetupReturnHome(this);
    }
}

void EnFamos_SetupReturnHome(EnFamos* this) {
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.world.rot.x = -Actor_WorldPitchTowardPoint(&this->actor, &this->targetDest);
    this->actionFunc = EnFamos_ReturnHome;
}

void EnFamos_ReturnHome(EnFamos* this, PlayState* play) {
    f32 distanceToHome = Actor_WorldDistXZToPoint(&this->actor, &this->targetDest);

    this->actor.shape.rot.y = Actor_WorldYawTowardPoint(&this->actor, &this->targetDest);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnFamos_UpdateBobbingHeight(this);
    if (this->isCalm) {
        Math_Vec3f_Copy(&this->calmPos, &this->actor.world.pos);
    }

    if (EnFamos_IsPlayerSeen(this, play)) {
        EnFamos_SetupAlert(this);
    } else if (distanceToHome < 20.0f) {
        if (this->pathPoints != NULL) {
            EnFamos_SetupPathingIdle(this);
        } else {
            EnFamos_SetupStillIdle(this);
        }
    } else if (distanceToHome < 40.0f) {
        Math_StepToF(&this->actor.speed, 0.5f, 0.3f);
    } else {
        Math_StepToF(&this->actor.speed, 3.0f, 0.3f);
    }
}

void EnFamos_SetupAlert(EnFamos* this) {
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->stateTimer = 8;
    this->actor.speed = 0.0f;

    if (this->isCalm == true) {
        this->isCalm = false;
        Math_Vec3f_Copy(&this->calmPos, &this->actor.world.pos);
    }

    this->actionFunc = EnFamos_Alert;
}

void EnFamos_Alert(EnFamos* this, PlayState* play) {
    if (ABS_ALT(this->flipRot) > 0x4000) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT_REVERSE - SFX_FLAG);
    } else {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT - SFX_FLAG);
    }

    this->stateTimer--;
    if (this->stateTimer == 0) {
        this->actor.world.pos.y = this->baseHeight;
        EnFamos_SetupChase(this);
    } else {
        this->actor.world.pos.y = Math_SinS(this->stateTimer * 0x1000) * 30.0f + this->baseHeight;
    }
}

void EnFamos_SetupChase(EnFamos* this) {
    this->hoverTimer = 0;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionFunc = EnFamos_Chase;
}

void EnFamos_Chase(EnFamos* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f abovePlayerPos;
    FloorProperty surfaceType;

    EnFamos_UpdateBobbingHeight(this);
    Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x800);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    abovePlayerPos.x = player->actor.world.pos.x;
    abovePlayerPos.y = player->actor.world.pos.y + 100.0f;
    abovePlayerPos.z = player->actor.world.pos.z;
    this->actor.world.rot.x = -Actor_WorldPitchTowardPoint(&this->actor, &abovePlayerPos);
    Math_StepToF(&this->actor.speed, 6.0f, 0.5f);

    surfaceType = SurfaceType_GetFloorProperty2(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
    if ((this->actor.xzDistToPlayer < 30.0f) && (this->actor.floorHeight > BGCHECK_Y_MIN) &&
        ((surfaceType != FLOOR_PROPERTY_12) && (surfaceType != FLOOR_PROPERTY_13))) {
        EnFamos_SetupAttackAim(this);

    } else if ((Player_GetMask(play) == PLAYER_MASK_STONE) ||
               (this->aggroDistance < Actor_WorldDistXZToPoint(&GET_PLAYER(play)->actor, &this->calmPos)) ||
               !Actor_IsFacingPlayer(&this->actor, 0x6000)) {
        EnFamos_SetupScanForPlayer(this);
    }
}

void EnFamos_SetupAttackAim(EnFamos* this) {
    Animation_PlayOnce(&this->skelAnime, &gFamosShakeAnim);
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_AMOS_VOICE);
    this->actionFunc = EnFamos_AttackAim;
}

void EnFamos_AttackAim(EnFamos* this, PlayState* play) {
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_LAST1_FALL_OLD - SFX_FLAG);
    if (SkelAnime_Update(&this->skelAnime)) {
        EnFamos_SetupAttack(this);
    }
}

void EnFamos_SetupAttack(EnFamos* this) {
    this->actor.world.rot.x = -0x4000;
    this->collider1.base.atFlags |= AT_ON;
    this->stateTimer = 4;
    this->actionFunc = EnFamos_Attack;
}

void EnFamos_Attack(EnFamos* this, PlayState* play) {
    s32 hitFloor;
    u32 surfaceType;

    Math_StepToF(&this->actor.speed, 20.0f, 2.0f);
    this->stateTimer--;
    if (this->stateTimer == 0) {
        this->emblemCollider.base.acFlags &= ~AC_ON;
    }

    surfaceType = SurfaceType_GetFloorProperty2(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
    hitFloor = this->actor.bgCheckFlags & BGCHECKFLAG_GROUND;
    if (hitFloor || (this->actor.floorHeight == BGCHECK_Y_MIN) || (surfaceType == FLOOR_PROPERTY_12) ||
        (surfaceType == FLOOR_PROPERTY_13)) {
        this->collider1.base.atFlags &= ~AT_ON;
        this->collider2.base.atFlags |= AT_ON;
        if (hitFloor) {
            Camera_AddQuake(GET_ACTIVE_CAM(play), 2, 15, 10);
            Rumble_Request(this->actor.xyzDistToPlayerSq, 180, 20, 100);
            EnFamos_SetupAttackDebris(this);

            if (this->animatedMaterialIndex != FAMOS_ANIMATED_MAT_NORMAL) {
                this->cratorDespawnTimer = 70;
                EnFamos_SetupDeathSlam(this);
            } else {
                this->cratorDespawnTimer = 20;
                EnFamos_SetupFinishAttack(this);
            }
        } else {
            this->emblemCollider.base.acFlags |= AC_ON;
            EnFamos_SetupAttackRebound(this);
        }
    } else {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_LAST1_FALL_OLD - SFX_FLAG);
    }
}

void EnFamos_SetupFinishAttack(EnFamos* this) {
    Animation_PlayOnce(&this->skelAnime, &gFamosIdleAnim);
    SkelAnime_Update(&this->skelAnime);
    this->emblemCollider.base.acFlags |= AC_ON;
    this->stateTimer = 3;
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EV_EXPLOSION);
    this->actionFunc = EnFamos_FinishAttack;
}

void EnFamos_FinishAttack(EnFamos* this, PlayState* play) {
    if (this->stateTimer == 0) {
        this->collider2.base.atFlags &= ~AT_ON;
    }

    this->stateTimer--;
    if (SkelAnime_Update(&this->skelAnime)) {
        EnFamos_SetupAttackRebound(this);
    }
}

void EnFamos_SetupAttackRebound(EnFamos* this) {
    this->actor.world.rot.x = 0x4000;
    this->actionFunc = EnFamos_AttackRebound;
    this->actor.speed = 0.0f;
}

void EnFamos_AttackRebound(EnFamos* this, PlayState* play) {
    Math_StepToF(&this->actor.speed, 5.0f, 0.3f);
    if (this->actor.speed > 1.0f) {
        if (ABS_ALT(this->flipRot) > 0x4000) {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT_REVERSE - SFX_FLAG);
        } else {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FAMOS_FLOAT - SFX_FLAG);
        }
    }

    if ((this->baseHeight < this->actor.world.pos.y) || (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING)) {
        this->actor.speed = 0.0f;
        EnFamos_SetupChase(this);
    }
}

void EnFamos_SetupScanForPlayer(EnFamos* this) {
    this->stateTimer = 60;
    this->actionFunc = EnFamos_ScanForPlayer;
    this->actor.speed = 0.0f;
}

void EnFamos_ScanForPlayer(EnFamos* this, PlayState* play) {
    EnFamos_UpdateBobbingHeight(this);
    this->stateTimer--;

    if (EnFamos_IsPlayerSeen(this, play)) {
        EnFamos_SetupAlert(this);
    } else if (this->stateTimer == 0) {
        EnFamos_SetupTurnHome(this);
    } else {
        this->actor.shape.rot.y = (s32)(Math_SinS(this->stateTimer * 0x888) * 8192.0f) + this->actor.world.rot.y;
    }
}

void EnFamos_SetupDeathSlam(EnFamos* this) {
    this->emblemCollider.base.acFlags &= ~AC_ON;
    this->stateTimer = 20;
    this->actor.speed = 0.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 20);
    this->flippedTimer = -1;
    this->actor.world.pos.y = this->actor.floorHeight - 60.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_EYEGOLE_DEAD);
    this->actionFunc = EnFamos_DeathSlam;
}

void EnFamos_DeathSlam(EnFamos* this, PlayState* play) {
    if (this->stateTimer == 17) {
        this->collider2.base.atFlags &= ~AT_ON;
    }

    if (this->stateTimer == 0) {
        EnFamos_SetupDeathExplosion(this);
    } else {
        this->stateTimer--;
    }
}

void EnFamos_SetupDeathExplosion(EnFamos* this) {
    this->actor.world.rot.x = 0x4000;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 4);
    this->stateTimer = 25;
    Math_Vec3f_Copy(&this->targetDest, &this->actor.world.pos);
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actionFunc = EnFamos_DeathExplosion;
}

void EnFamos_DeathExplosion(EnFamos* this, PlayState* play) {
    Math_StepToF(&this->actor.speed, 3.0f, 0.3f);
    if (this->actor.colorFilterTimer == 0) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 4);
    }

    this->actor.world.pos.x = Rand_CenteredFloat(5.0f) + this->targetDest.x;
    this->actor.world.pos.z = Rand_CenteredFloat(5.0f) + this->targetDest.z;
    if (this->stateTimer == 1) {
        EnBom* explosion = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x,
                                               this->actor.world.pos.y + 40.0f, this->actor.world.pos.z,
                                               BOMB_EXPLOSIVE_TYPE_BOMB, 0, 0, BOMB_TYPE_BODY);
        if (explosion != NULL) {
            explosion->timer = 0;
        }
        this->stateTimer--;

    } else if (this->stateTimer == 0) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, (0xD << 4));
        EnFamos_SetupDeathFade(this);

    } else {
        this->stateTimer--;
    }
}

void EnFamos_SetupDeathFade(EnFamos* this) {
    EnFamos_SetupDeathDebris(this);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.shape.shadowDraw = NULL;
    this->actionFunc = EnFamos_DeathFade;
    this->actor.speed = 0.0f;
}

void EnFamos_DeathFade(EnFamos* this, PlayState* play) {
    Actor* enBom;

    if (this->debrisTimer == 0) {
        enBom = this->actor.child;
        if (enBom != NULL) {
            enBom->parent = NULL;
        }
        Actor_Kill(&this->actor);
    }
}

void EnFamos_UpdateDebrisPosRot(EnFamos* this) {
    EnFamosRock* rock;
    s32 i;

    rock = &this->rocks[0];
    for (i = 0; i < ARRAY_COUNT(this->rocks); i++, rock++) {
        rock->velocity.y -= 1.0f;
        Math_Vec3f_Sum(&rock->pos, &rock->velocity, &rock->pos);
        rock->rot.x += (s16)((Rand_Next() >> 0x17) + 0x700);
        rock->rot.y += (s16)((Rand_Next() >> 0x17) + 0x900);
        rock->rot.z += (s16)((Rand_Next() >> 0x17) + 0xB00);
    }
}

void EnFamos_Update(Actor* thisx, PlayState* play) {
    EnFamos* this = (EnFamos*)thisx;
    f32 oldHeight;
    s32 oldHoverTimer;

    if ((this->debrisTimer <= 0) ||
        (this->debrisTimer--, EnFamos_UpdateDebrisPosRot(this), (this->actionFunc != EnFamos_DeathFade))) {
        oldHoverTimer = this->hoverTimer;
        EnFamos_UpdateFlipStatus(this);
        if (this->cratorDespawnTimer > 0) {
            this->cratorDespawnTimer--;
            if (this->cratorDespawnTimer == 0) {
                if (this->actor.child != NULL)
                    this->actor.child->parent = NULL;
            }
        }

        this->actionFunc(this, play);
        Vec3f beforeMove = this->actor.world.pos;
        oldHeight = beforeMove.y;
        Actor_MoveWithoutGravity(&this->actor);
        if (oldHoverTimer != this->hoverTimer) {
            this->baseHeight += this->actor.world.pos.y - oldHeight;
        }

        if (this->flippedTimer >= 0) {
            u32 bgFlags =
                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10;
            if (this->actionFunc != EnFamos_Attack)
                bgFlags |= UPDBGCHECKINFO_FLAG_2;
            Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 30.0f, 80.0f, bgFlags);
            if (this->actionFunc == EnFamos_Attack && !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
                Vec3f hit;
                CollisionPoly* floor = NULL;
                s32 bgId;
                if (BgCheck_EntityLineTest1(&play->colCtx, &beforeMove, &this->actor.world.pos, &hit, &floor, 0, 1, 0,
                                            0, &bgId)) {
                    this->actor.world.pos = hit;
                    this->actor.floorPoly = floor;
                    this->actor.floorHeight = hit.y;
                    this->actor.floorBgId = bgId;
                    this->actor.bgCheckFlags |= BGCHECKFLAG_GROUND;
                }
            }
            if ((this->actionFunc == EnFamos_Attack) && (this->animatedMaterialIndex != FAMOS_ANIMATED_MAT_NORMAL) &&
                (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
                this->actor.world.pos.y -= 60.0f;
            }
        }

        this->actor.focus.rot.y = this->actor.shape.rot.y;
        Collider_UpdateCylinder(&this->actor, &this->collider1);
        if (this->collider1.base.atFlags & AT_ON) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider1.base);
        }

        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
        if (this->emblemCollider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->emblemCollider.base);
        }

        if (this->collider2.base.atFlags & AT_ON) {
            Collider_UpdateCylinder(&this->actor, &this->collider2);
            this->collider2.dim.pos.y = this->actor.floorHeight;
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider2.base);
        }
    }
}

s32 EnFamos_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnFamos* this = (EnFamos*)thisx;

    if (limbIndex == FAMOS_LIMB_BODY) {
        Matrix_Translate(0.0f, 4000.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateZS(this->flipRot, MTXMODE_APPLY);
        Matrix_Translate(0.0f, -4000.0f, 0.0f, MTXMODE_APPLY);

    } else if (this->flippedTimer < 0) {
        if ((limbIndex == FAMOS_LIMB_SWORD) || (limbIndex == FAMOS_LIMB_SHIELD) || (limbIndex == FAMOS_LIMB_HEAD)) {
            *dList = NULL;
        }
    }

    return false;
}

void EnFamos_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnFamos* this = (EnFamos*)thisx;

    if (limbIndex == FAMOS_LIMB_EMBLEM) {
        Matrix_MultZero(&this->actor.focus.pos);
        Collider_UpdateSpheres(limbIndex, &this->emblemCollider);
    }
}

void EnFamos_DrawDebris(EnFamos* this, PlayState* play) {
    s32 i;

    if (this->debrisTimer > 0) {
        Gfx* gfx;
        EnFamosRock* rock;

        OPEN_DISPS(play->state.gfxCtx);

        gfx = POLY_OPA_DISP;

        Gfx_SetupDL(&gfx[0], 25);

        gDPSetPrimColor(&gfx[1], 0, 0x80, 255, 255, 255, 255);

        gDPSetEnvColor(&gfx[2], 255, 255, 255, 255);

        rock = &this->rocks[0];
        for (i = 0; i < ARRAY_COUNT(this->rocks); i++, rock++) {

            Matrix_SetTranslateRotateYXZ(rock->pos.x, rock->pos.y, rock->pos.z, &rock->rot);
            Matrix_Scale(rock->scale, rock->scale, rock->scale, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(&gfx[3 + i * 2], play->state.gfxCtx);

            gfx[4 + i * 2] = (Gfx)gsSPDisplayList(FAMOS_OOT_DEBRIS_DL);
        }

        POLY_OPA_DISP = &gfx[3 + (ARRAY_COUNT(this->rocks) * 2)];

        CLOSE_DISPS();
    }
}

void EnFamos_Draw(Actor* thisx, PlayState* play) {
    EnFamos* this = (EnFamos*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    if (this->actionFunc != EnFamos_DeathFade) {
        AnimatedMat_Draw(play, sEmblemAnimatedMats[this->animatedMaterialIndex]);
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnFamos_OverrideLimbDraw,
                          EnFamos_PostLimbDraw, &this->actor);
        if (this->actor.colorFilterTimer != 0) {
            func_800AE5A0(play);
        }
    }
    EnFamos_DrawDebris(this, play);
}

OVL_INFO_NATIVE_ENEMY(0x2d, En_Famos_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_FAMOS, En_Famos_Profile, EnFamos_Init, EnFamos_Destroy, EnFamos_Update,
                     EnFamos_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x2du, 0x1du, 0x5a09797au };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_FAMOS_ORIGIN, generatedNativeOrigin);
