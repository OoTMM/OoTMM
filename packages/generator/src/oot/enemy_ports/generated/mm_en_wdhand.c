#include "mm_native_compat.h"
#define gDexihandCloseAnim (*(AnimationHeader*)0x060000f4)
#define object_wdhand_Anim_000364 (*(AnimationHeader*)0x06000364)
#define gDexihandOpenAnim (*(AnimationHeader*)0x06000534)
#define gDexihandIdleAnim (*(AnimationHeader*)0x06000854)
#define gDexihandBaseDL (*((Gfx (*)[1])0x060014c0))
#define gDexihandArmSegmentDL (*((Gfx (*)[1])0x060015b0))
#define gDexihandSkel (*(FlexSkeletonHeader*)0x06001e20)

#include "types/en_wdhand.h"
#define TATL_HINT_ID_DEXIHAND 0xff
#undef ACTOR_EN_WDHAND
#define ACTOR_EN_WDHAND 0x1d1
#undef OBJECT_WDHAND
#define OBJECT_WDHAND 0x11b5
#undef NA_SE_EN_HANDW_GET
#define NA_SE_EN_HANDW_GET 0x39fc
#undef NA_SE_EN_HANDW_RELEASE
#define NA_SE_EN_HANDW_RELEASE 0x39fd
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_MultVecY
#define Matrix_MultVecY ((void (*)(f32 y, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Y)
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Math_Atan2S_XY
#define Math_Atan2S_XY ((s16 (*)(f32 x, f32 y))(void*)FOREIGN_ENEMY_API_ATAN2S_XY)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZYX)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef func_800B8D50
#define func_800B8D50 ((void (*)(struct PlayState* play, Actor* actor, f32 arg2, s16 yaw, f32 arg4, u32 arg5))(void*)FOREIGN_ENEMY_API_KNOCKBACK_LARGE)
#undef Matrix_Mult
#define Matrix_Mult ((void (*)(MtxF* mf, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_MULT)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef Matrix_GetCurrent
#define Matrix_GetCurrent ((MtxF* (*)(void))0x800aa78cu)
#undef Matrix_MtxFCopy
#define Matrix_MtxFCopy ((void (*)(MtxF* dest, MtxF* src))0x800aba10u)
#undef Matrix_MtxFToYXZRot
#define Matrix_MtxFToYXZRot ((void (*)(MtxF* src, Vec3s* dest, s32 nonUniformScale))0x800abf50u)
#undef Matrix_Transpose
#define Matrix_Transpose ((void (*)(MtxF* mf))0x800abe1cu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Math_Vec3f_Diff
#define Math_Vec3f_Diff ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d48u)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Actor_WorldPitchTowardPoint
#define Actor_WorldPitchTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x80021488u)
#undef Math_SinF
#define Math_SinF ((f32 (*)(f32 rad))0x800a45fcu)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d21cu)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x800aa6ecu)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x800aa724u)
#undef Math_Vec3f_Sum
#define Math_Vec3f_Sum ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d10u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnWdhand_Init(Actor* thisx, PlayState* play);
void EnWdhand_Destroy(Actor* thisx, PlayState* play);
void EnWdhand_Update(Actor* thisx, PlayState* play);
void EnWdhand_Draw(Actor* thisx, PlayState* play);

void EnWdhand_ReturnToIdle(EnWdhand* this, PlayState* play);
void EnWdhand_Idle(EnWdhand* this, PlayState* play);
void EnWdhand_LungeForPlayer(EnWdhand* this, PlayState* play);
void EnWdhand_FailedToGrabPlayer(EnWdhand* this, PlayState* play);
void EnWdhand_GrabbedPlayer(EnWdhand* this, PlayState* play);
void EnWdhand_Die(EnWdhand* this, PlayState* play);

void EnWdhand_SetupReturnToIdle(EnWdhand* this);
void EnWdhand_SetupIdle(EnWdhand* this);
void EnWdhand_SetupLungeForPlayer(EnWdhand* this);
void EnWdhand_SetupFailedToGrabPlayer(EnWdhand* this);
void EnWdhand_SetupGrabbedPlayer(EnWdhand* this, PlayState* play);
void EnWdhand_SetupDie(EnWdhand* this);

void EnWdhand_GetInitVelocity(EnWdhand* this, Vec3f* dst);
void EnWdhand_Vec3fToVec3s(Vec3s* dst, Vec3f* src);

ActorProfile En_Wdhand_Profile = {
    0xdd1,         ACTORCAT_ENEMY,   FLAGS,           0x11b5,        sizeof(EnWdhand),
    EnWdhand_Init, EnWdhand_Destroy, EnWdhand_Update, EnWdhand_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[EN_WDHAND_NUM_COLLIDER_ELEMENTS] = {
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 575, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1725, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK1,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 50, { { 0, 1000, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 25, 25, MASS_HEAVY };

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0),
} };

void EnWdhand_Init(Actor* thisx, PlayState* play) {
    EnWdhand* this = (EnWdhand*)thisx;
    s32 i;
    Vec3f initVel;

    SkelAnime_InitFlex(play, &this->skelAnime, &gDexihandSkel, &gDexihandIdleAnim, this->jointTable, this->morphTable,
                       DEXIHAND_LIMB_MAX);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
    Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
    Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);
    Matrix_MultVecY(3.0f, &this->endPoints[EN_WDHAND_HAND_POINT]);
    Matrix_MtxFCopy(&this->relativeToWorldTransform, Matrix_GetCurrent());
    Matrix_MtxFToYXZRot(&this->relativeToWorldTransform, &this->actor.shape.rot, false);
    Matrix_Transpose(&this->relativeToWorldTransform);

    this->actor.world.rot.y = this->actor.shape.rot.y;

    EnWdhand_GetInitVelocity(this, &initVel);
    this->actor.speed = initVel.z;
    this->actor.velocity.y = initVel.y;
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speed;

    Collider_InitAndSetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);

    for (i = 0; i < ARRAY_COUNT(this->colliderElements); i++) {
        ColliderJntSphElement* jntSphElem = &this->collider.elements[i];

        jntSphElem->dim.worldSphere.radius = jntSphElem->dim.modelSphere.radius;
        EnWdhand_Vec3fToVec3s(&jntSphElem->dim.worldSphere.center, &this->actor.world.pos);
    }

    for (i = 0; i < EN_WDHAND_NUM_SEGMENTS; i++) {
        this->limbScaleFactors[i] = 1.0f;
    }

    this->limbIndexBeforeCut = -1;
    this->limbIndexAfterCut = 0;

    this->actor.world.rot.x = 0;
    this->actor.world.rot.z =
        Math_Atan2S_XY(this->relativeToWorldTransform.mf[2][1], this->relativeToWorldTransform.mf[0][1]);
    this->actor.hintId = TATL_HINT_ID_DEXIHAND;
    EnWdhand_SetupIdle(this);
}

void EnWdhand_Destroy(Actor* thisx, PlayState* play) {
    EnWdhand* this = (EnWdhand*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void EnWdhand_GetInitVelocity(EnWdhand* this, Vec3f* dst) {
    s32 param;

    if (this->actor.params == 0) {
        dst->x = 0.f;
        dst->y = dst->z = 8.f;
        return;
    }
    param = EN_WDHAND_GET_Y_INIT_VELOCITY(&this->actor);
    if (param == EN_WDHAND_INIT_VELOCITY_MAX) {
        param = 40;
    }
    dst->y = param * 0.2f;

    param = EN_WDHAND_GET_Z_INIT_VELOCITY(&this->actor);
    if (param == EN_WDHAND_INIT_VELOCITY_MAX) {
        param = 40;
    }
    dst->z = param * 0.2f;

    dst->x = 0.0f;
}

void EnWdhand_GetRelativeRotationsToPlayerRightFoot(EnWdhand* this, Player* player, s16* pitchOut, s16* yawOut) {
    Vec3f diffToPlayerRightFoot;
    Vec3f diffToPlayerRightFootRelative;

    Math_Vec3f_Diff(&player->bodyPartsPos[PLAYER_BODYPART_R_FOOT], &this->actor.world.pos, &diffToPlayerRightFoot);
    Matrix_Put(&this->relativeToWorldTransform);
    Matrix_MultVec3f(&diffToPlayerRightFoot, &diffToPlayerRightFootRelative);
    *yawOut = Actor_WorldYawTowardPoint(&this->actor, &diffToPlayerRightFootRelative);
    *pitchOut = Actor_WorldPitchTowardPoint(&this->actor, &diffToPlayerRightFootRelative) + 0x4000;
}

void EnWdhand_SetTransform(EnWdhand* this, s32 limbIndex, s32 arg2, Vec3f* translation) {
    MtxF* currentMatrix = Matrix_GetCurrent();

    Matrix_RotateZYX(this->limbRotations[limbIndex].x, this->limbRotations[limbIndex].y, 0, MTXMODE_APPLY);
    if (arg2) {
        Matrix_RotateYS(-this->limbRotations[limbIndex].y, MTXMODE_APPLY);
    }

    if (limbIndex == DEXIHAND_LIMB_03) {
        if (this->globalLimbScaleFactor > 1.0f) {
            f32 scale = CLAMP_MAX(this->globalLimbScaleFactor, 1.5f);

            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        }
    } else if ((this->limbScaleFactors[limbIndex] < 1.0f) || (this->globalLimbScaleFactor > 1.0f)) {
        if (this->limbScaleFactors[limbIndex] < 0.1f) {
            Matrix_Scale(0.0f, this->globalLimbScaleFactor * this->limbScaleFactors[limbIndex], 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Scale(1.0f, this->globalLimbScaleFactor * this->limbScaleFactors[limbIndex], 1.0f, MTXMODE_APPLY);
        }
    }

    currentMatrix->mf[3][0] = translation->x;
    currentMatrix->mf[3][1] = translation->y;
    currentMatrix->mf[3][2] = translation->z;
}

void EnWdhand_Vec3fToVec3s(Vec3s* dst, Vec3f* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

s16 EnWdhand_GetLimbXRotation(EnWdhand* this, s32 limbIndex) {
    return this->limbRotations[limbIndex].z * Math_SinF((this->timer - limbIndex * 20) * (M_PIf / 40));
}

void EnWdhand_SetupIdle(EnWdhand* this) {
    s32 i;

    Animation_MorphToLoop(&this->skelAnime, &gDexihandIdleAnim, -5.0f);

    if (this->actionFunc != EnWdhand_ReturnToIdle) {
        this->timer = 80;
        this->limbRotations[0].y = Rand_Next() >> 16;

        for (i = 1; i < ARRAY_COUNT(this->limbRotations); i++) {
            this->limbRotations[i].y = this->limbRotations[i - 1].y + (s32)Rand_ZeroFloat(0x2000) + 0x8000;
        }

        for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
            this->limbRotations[i].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        this->limbRotations[i].x = EnWdhand_GetLimbXRotation(this, i);
    }

    this->actionFunc = EnWdhand_Idle;
    this->globalLimbScaleFactor = 1.0f;
}

void EnWdhand_Idle(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 i;
    s16 pitchToPlayerRightFoot;
    s16 yawToPlayerRightFoot;

    SkelAnime_Update(&this->skelAnime);

    this->timer--;

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        this->limbRotations[i].x = EnWdhand_GetLimbXRotation(this, i);

        if (this->timer == 20 * i) {
            if (i != 0) {
                this->limbRotations[i].y = this->limbRotations[i - 1].y + (s32)Rand_CenteredFloat(16384.0f);
                this->limbRotations[i].y -= 0x8000;
            } else {
                this->limbRotations[0].y += (s16)(s32)Rand_CenteredFloat(16384.0f);
            }
            this->limbRotations[i].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    if (this->timer == 0) {
        this->timer = 80;
    }

    if (!(player->stateFlags2 & PLAYER_STATE2_80) && (this->actor.xyzDistToPlayerSq < SQ(120.75f))) {
        EnWdhand_GetRelativeRotationsToPlayerRightFoot(this, player, &pitchToPlayerRightFoot, &yawToPlayerRightFoot);
        if (ABS_ALT(pitchToPlayerRightFoot) <= 0x4000) {
            EnWdhand_SetupLungeForPlayer(this);
        }
    }
}

void EnWdhand_SetupLungeForPlayer(EnWdhand* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gDexihandOpenAnim, -5.0f);
    this->timer = 5;
    this->actionFunc = EnWdhand_LungeForPlayer;
}

void EnWdhand_LungeForPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 allStepsDone = true;
    s32 i;
    s16 pitchToPlayerRightFoot;
    s16 yawToPlayerRightFoot;

    SkelAnime_Update(&this->skelAnime);

    if (this->timer > 0) {
        this->timer--;
        if (this->timer == 0) {
            this->collider.base.atFlags |= AT_ON;
        }
        return;
    }

    Math_StepToF(&this->globalLimbScaleFactor, 1.5f, 0.05f);
    EnWdhand_GetRelativeRotationsToPlayerRightFoot(this, player, &pitchToPlayerRightFoot, &yawToPlayerRightFoot);

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        s16 yawDiff = yawToPlayerRightFoot - this->limbRotations[i].y;

        if (ABS_ALT(yawDiff) < 0x4000) {
            allStepsDone &=
                Math_ScaledStepToS(&this->limbRotations[i].x, pitchToPlayerRightFoot, (i * 0.1f + 1.0f) * 1920.0f);
            allStepsDone &= Math_ScaledStepToS(&this->limbRotations[i].y, yawToPlayerRightFoot, 0x800);
        } else {
            allStepsDone &=
                Math_ScaledStepToS(&this->limbRotations[i].x, -pitchToPlayerRightFoot, (i * 0.1f + 1.0f) * 1920.0f);
            allStepsDone &= Math_ScaledStepToS(&this->limbRotations[i].y, yawToPlayerRightFoot + 0x8000, 0x800);
        }
    }

    if ((this->collider.base.atFlags & AT_HIT) && play->grabPlayer(play, player)) {
        EnWdhand_SetupGrabbedPlayer(this, play);
    } else if (allStepsDone) {
        EnWdhand_SetupFailedToGrabPlayer(this);
    }

    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
    }
}

void EnWdhand_SetupFailedToGrabPlayer(EnWdhand* this) {
    Animation_PlayOnce(&this->skelAnime, &gDexihandCloseAnim);
    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~AT_HIT;
    this->actionFunc = EnWdhand_FailedToGrabPlayer;
    this->globalLimbScaleFactor = 1.5f;
}

void EnWdhand_FailedToGrabPlayer(EnWdhand* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        EnWdhand_SetupReturnToIdle(this);
    }
}

void EnWdhand_SetupReturnToIdle(EnWdhand* this) {
    s32 i;

    Animation_MorphToLoop(&this->skelAnime, &gDexihandIdleAnim, -10.0f);

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        this->limbRotations[i].z = Rand_S16Offset(0x1800, 0x1000);
    }
    this->timer = 80;
    this->actionFunc = EnWdhand_ReturnToIdle;
}

void EnWdhand_ReturnToIdle(EnWdhand* this, PlayState* play) {
    s32 i;
    s32 allStepsDone;

    SkelAnime_Update(&this->skelAnime);

    this->timer--;

    allStepsDone = Math_ScaledStepToS(&this->limbRotations[0].x, EnWdhand_GetLimbXRotation(this, 0), 0x200);

    for (i = 1; i < ARRAY_COUNT(this->limbRotations); i++) {
        allStepsDone &= Math_ScaledStepToS(&this->limbRotations[i].x, EnWdhand_GetLimbXRotation(this, i), 0x300);
        if (this->timer == 20 * i) {
            this->limbRotations[i].z = Rand_S16Offset(0x1800, 0x1000);
        }
    }

    if (this->timer == 0) {
        this->timer = 80;
        this->limbRotations[0].z = Rand_S16Offset(0x1800, 0x1000);
    }

    Math_StepToF(&this->globalLimbScaleFactor, 1.0f, 0.05f);

    if (allStepsDone) {
        EnWdhand_SetupIdle(this);
    }
}

void EnWdhand_SetupGrabbedPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 i;

    this->collider.base.atFlags &= ~AT_ON;
    this->collider.base.atFlags &= ~AT_HIT;
    this->collider.base.ocFlags1 &= ~OC1_ON;
    Animation_PlayOnce(&this->skelAnime, &gDexihandCloseAnim);

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        this->limbRotations[i].z = Rand_S16Offset(0x2000, 0x1000);
    }
    this->timer = 80;
    player->actor.parent = &this->actor;
    Matrix_Transpose(&this->relativeToWorldTransform);
    this->globalLimbScaleFactor = 1.5f;

    Actor_PlaySfx(&this->actor, NA_SE_EN_HANDW_GET);
    this->actionFunc = EnWdhand_GrabbedPlayer;
}

void EnWdhand_GrabbedPlayer(EnWdhand* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 i;
    s32 t;

    SkelAnime_Update(&this->skelAnime);

    this->timer--;
    player->av2.actionVar2 = 0;
    t = this->timer;

    for (i = 0; i < ARRAY_COUNT(this->limbRotations); i++) {
        if (this->timer < 76) {
            this->limbRotations[i].x = this->limbRotations[i].z * Math_SinF(t * (M_PIf / 8));
        } else {
            Math_ScaledStepToS(&this->limbRotations[i].x, this->limbRotations[i].z * Math_SinF(t * (M_PIf / 8)), 0x400);
        }
        if (t % 16 == 0) {
            if (t == 16) {
                this->limbRotations[i].y = 0;
            } else if (i != 0) {
                this->limbRotations[i].y = this->limbRotations[i - 1].y + (s32)Rand_CenteredFloat(12288.0f);
            } else {
                this->limbRotations[0].y += (s16)(s32)Rand_CenteredFloat(12288.0f);
            }
            this->limbRotations[i].z = Rand_S16Offset(0x2000, 0x1000);
        }
        t += 2;
    }

    if (this->timer < 4) {
        Math_StepToF(&this->globalLimbScaleFactor, 1.5f, 1.0f / 8);
    } else if (this->timer < 16) {
        Math_StepToF(&this->globalLimbScaleFactor, 2.0f, 1.0f / 24);
    }

    if (this->timer == 0) {
        this->collider.base.ocFlags1 |= OC1_ON;
        EnWdhand_SetupReturnToIdle(this);
    } else if (this->timer >= 4) {
        Vec3f limbPos;

        Matrix_Put(&this->relativeToWorldTransform);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        Matrix_MultVecY(300.0f, &limbPos);

        for (i = 0; i < EN_WDHAND_NUM_SEGMENTS; i++) {
            Matrix_Push();
            EnWdhand_SetTransform(this, i, false, &limbPos);
            Matrix_MultVecY(2300.0f, &limbPos);
            Matrix_Pop();
        }

        EnWdhand_SetTransform(this, EN_WDHAND_NUM_SEGMENTS, true, &limbPos);
        Matrix_MtxFToYXZRot(Matrix_GetCurrent(), &player->actor.shape.rot, false);

        if (this->globalLimbScaleFactor > 1.0f) {
            f32 zRotFactor = CLAMP_MAX(this->globalLimbScaleFactor, 1.5f);

            Matrix_RotateZS((zRotFactor - 1.0f) * -32768.0f, MTXMODE_APPLY);
        }
        Matrix_MultVecY(1000.0f, &player->actor.world.pos);

        if ((this->timer == 4) && (player->actor.parent == &this->actor)) {
            s16 throwYaw = Rand_Next() >> 16;
            player->av2.actionVar2 = 100;
            player->actor.parent = NULL;
            Matrix_Transpose(&this->relativeToWorldTransform);
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
            player->actor.world.pos.x += 2.0f * this->actor.speed * Math_SinS(throwYaw);
            player->actor.world.pos.y += 2.0f * this->actor.velocity.y;
            player->actor.world.pos.z += 2.0f * this->actor.speed * Math_CosS(throwYaw);
            func_800B8D50(play, &this->actor, this->actor.speed * 2.f, throwYaw, this->actor.velocity.y * 1.5f, 0);
            Actor_PlaySfx(&this->actor, NA_SE_EN_HANDW_RELEASE);
        } else if (this->timer == 2) {
            Animation_PlayOnce(&this->skelAnime, &object_wdhand_Anim_000364);
        }
    }
}

s32 EnWdhand_ShrinkLimb(EnWdhand* this, s32 limbIndex) {
    this->limbScaleFactors[limbIndex] -= 0.1f;
    if (this->limbScaleFactors[limbIndex] <= 0.05f) {
        this->limbScaleFactors[limbIndex] = 0.05f;
        return true;
    }
    return false;
}

void EnWdhand_SetupDie(EnWdhand* this) {
    s32 i;

    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;

    for (i = 0; i < ARRAY_COUNT(this->colliderElements); i++) {
        if (this->collider.elements[i].base.acElemFlags & ACELEM_HIT) {
            break;
        }
    }

    this->limbIndexAfterCut = ((i + 1) / 2) % 4;
    this->limbIndexBeforeCut = this->limbIndexAfterCut - 1;

    Matrix_Put(&this->relativeToWorldTransform);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

    Matrix_MultVecY(300.0f, &this->endPoints[EN_WDHAND_HAND_POINT]);

    for (i = 0; i < this->limbIndexAfterCut; i++) {
        Matrix_Push();
        EnWdhand_SetTransform(this, i, false, &this->endPoints[EN_WDHAND_HAND_POINT]);
        Matrix_MultVecY(2300.0f, &this->endPoints[EN_WDHAND_HAND_POINT]);
        Matrix_Pop();
    }

    this->actor.velocity.y = 2.45f;
    this->actor.velocity.x = 2.0f * Math_SinS(this->actor.world.rot.z);
    this->actor.velocity.z = 2.0f * Math_CosS(this->actor.world.rot.z);
    this->timer = 5;
    this->actionFunc = EnWdhand_Die;
}

void EnWdhand_Die(EnWdhand* this, PlayState* play) {
    static Vec3f sEffectVelocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f sEffectAccel = { 0.0f, 0.05f, 0.0f };
    Vec3s* handCollider = &this->collider.elements[6].dim.worldSphere.center;
    Vec3f spA0;
    Vec3f sp94;
    s16 effectScale;
    s32 pointIndex;
    s32 i;
    s32 t;
    Vec3f* endPoint = &this->endPoints[EN_WDHAND_HAND_POINT];
    s32 limbIndex;
    Vec3f effPos;

    spA0.x = (endPoint->x - handCollider->x) * 0.5f;
    spA0.y = (endPoint->y - handCollider->y) * 0.5f;
    spA0.z = (endPoint->z - handCollider->z) * 0.5f;
    Matrix_RotateXS(0x100, MTXMODE_NEW);
    Matrix_MultVec3f(&spA0, &sp94);
    endPoint->x = spA0.x + sp94.x + handCollider->x;
    endPoint->y = spA0.y + sp94.y + handCollider->y;
    endPoint->z = spA0.z + sp94.z + handCollider->z;
    if (this->actor.velocity.y > -2.0f) {
        this->actor.velocity.y += -0.15f;
    }
    Math_Vec3f_Sum(endPoint, &this->actor.velocity, endPoint);

    this->actor.world.rot.x += 0x100;

    if (this->limbIndexBeforeCut >= 0 && EnWdhand_ShrinkLimb(this, this->limbIndexBeforeCut)) {
        this->limbIndexBeforeCut--;
    }

    limbIndex = this->limbIndexAfterCut;

    if (this->limbIndexAfterCut < EN_WDHAND_NUM_SEGMENTS) {
        if (EnWdhand_ShrinkLimb(this, this->limbIndexAfterCut)) {
            this->limbIndexAfterCut++;
        }
        Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.z, 0, MTXMODE_NEW);
        Matrix_RotateYS(-this->actor.world.rot.z, MTXMODE_APPLY);
        Matrix_Mult(&this->relativeToWorldTransform, MTXMODE_APPLY);
        EnWdhand_SetTransform(this, limbIndex, false, endPoint);
        Matrix_MultVecY(2.3f / this->limbScaleFactors[limbIndex], endPoint);
    }

    this->timer--;

    limbIndex = this->limbIndexBeforeCut + 1;
    t = this->timer;
    for (i = 0; i < limbIndex; i++) {
        if (this->timer > 0) {
            Math_ScaledStepToS(&this->limbRotations[i].x, this->limbRotations[i].z * Math_SinF(t * (2 * M_PIf / 7)),
                               0x200);
        } else if (Rand_ZeroOne() < 0.65f) {
            this->limbRotations[i].x = this->limbRotations[i].z * Math_SinF(t * (2 * M_PIf / 7));
        }
        if (t % 7 == 0) {
            if (i != 0) {
                this->limbRotations[i].y = this->limbRotations[i - 1].y + (s32)Rand_CenteredFloat(12288.0f);
            } else {
                this->limbRotations[0].y += (s16)(s32)Rand_CenteredFloat(12288.0f);
            }
            this->limbRotations[i].z = Rand_S16Offset(i * 0x100 + 0xC00, 0x800);
        }
        t += 2;
    }

    if ((this->limbIndexBeforeCut < 0) && (this->limbIndexAfterCut >= EN_WDHAND_NUM_SEGMENTS)) {
        if (Math_StepToF(&this->actor.scale.x, 0.0f, 0.001f)) {
            for (pointIndex = 0; pointIndex < ARRAY_COUNT(this->endPoints); pointIndex++) {
                for (i = 0; i < 5; i++) {
                    endPoint = &this->endPoints[pointIndex];

                    sEffectVelocity.y = Rand_ZeroOne() + 1.0f;
                    effectScale = Rand_S16Offset(0x28, 0x28);
                    effPos.x = endPoint->x + Rand_CenteredFloat(12.0f);
                    effPos.y = endPoint->y + Rand_CenteredFloat(12.0f);
                    effPos.z = endPoint->z + Rand_CenteredFloat(12.0f);
                    EffectSsDtBubble_SpawnColorProfile(play, &effPos, &sEffectVelocity, &sEffectAccel, effectScale, 25,
                                                       2, true);
                }
            }

            Actor_Kill(&this->actor);
        } else {
            this->actor.scale.z = this->actor.scale.y = this->actor.scale.x;
        }
    }

    for (i = 0; i < ARRAY_COUNT(this->endPoints); i++) {
        sEffectVelocity.y = Rand_ZeroOne() + 1.0f;
        EffectSsDtBubble_SpawnColorProfile(play, &this->endPoints[i], &sEffectVelocity, &sEffectAccel,
                                           Rand_S16Offset(40, 40), 25, 2, true);
    }
}

void EnWdhand_UpdateDamage(EnWdhand* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        Player* player = GET_PLAYER(play);

        this->collider.base.acFlags &= ~AT_ON;
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.atFlags &= ~AT_ON;
        this->collider.base.ocFlags1 &= ~AT_ON;

        Actor_SetDropFlagJntSph(&this->actor, &this->collider);
        Enemy_StartFinishingBlow(play, &this->actor);

        if ((player->stateFlags2 & PLAYER_STATE2_80) && (&this->actor == player->actor.parent)) {
            player->av2.actionVar2 = 100;
            player->actor.parent = NULL;
            player->actor.shape.rot.x = 0;
            player->actor.shape.rot.z = 0;
            func_800B8D50(play, &this->actor, this->actor.speed, this->actor.world.rot.y, this->actor.velocity.y, 0);
        } else {
            Matrix_Transpose(&this->relativeToWorldTransform);
        }

        EnWdhand_SetupDie(this);
    }
}

void EnWdhand_Update(Actor* thisx, PlayState* play) {
    EnWdhand* this = (EnWdhand*)thisx;

    EnWdhand_UpdateDamage(this, play);

    this->actionFunc(this, play);

    if (this->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.ocFlags1 & OC1_ON) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnWdhand_UpdateColliderLocationsForLimb(EnWdhand* this, s32 limbIndex, Vec3f* limbPos) {
    EnWdhand_SetTransform(this, limbIndex, true, limbPos);
    Matrix_MultVecY(575.0f, limbPos);
    EnWdhand_Vec3fToVec3s(&this->collider.elements[2 * limbIndex + 0].dim.worldSphere.center, limbPos);
    Matrix_MultVecY(1725.0f, limbPos);
    EnWdhand_Vec3fToVec3s(&this->collider.elements[2 * limbIndex + 1].dim.worldSphere.center, limbPos);
    Matrix_MultVecY(2300.0f, limbPos);
}

void EnWdhand_Draw(Actor* thisx, PlayState* play) {
    EnWdhand* this = (EnWdhand*)thisx;
    Vec3f limbPos;
    Gfx* gfx;
    s32 limbIndex;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_OPA_DISP;

    Gfx_SetupDL(&gfx[0], 25);
    MATRIX_FINALIZE_AND_LOAD(&gfx[1], play->state.gfxCtx);
    gSPDisplayList(&gfx[2], gDexihandBaseDL);

    Matrix_MultVecY(300.0f, &limbPos);

    gfx = &gfx[3];

    limbIndex = this->limbIndexBeforeCut + 1;
    for (i = 0; i < limbIndex; i++) {
        Matrix_Push();
        EnWdhand_UpdateColliderLocationsForLimb(this, i, &limbPos);
        MATRIX_FINALIZE_AND_LOAD(gfx++, play->state.gfxCtx);
        gSPDisplayList(gfx++, gDexihandArmSegmentDL);
        Matrix_Pop();
    }

    Math_Vec3f_Copy(&this->endPoints[EN_WDHAND_CUT_POINT], &limbPos);
    Math_Vec3f_Copy(&limbPos, &this->endPoints[EN_WDHAND_HAND_POINT]);

    if (this->actor.world.rot.x != 0) {
        Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.z, 0, MTXMODE_NEW);
        Matrix_RotateYS(-this->actor.world.rot.z, MTXMODE_APPLY);
        Matrix_Mult(&this->relativeToWorldTransform, MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
    }

    for (i = this->limbIndexAfterCut; i < EN_WDHAND_NUM_SEGMENTS; i++) {
        Matrix_Push();
        EnWdhand_UpdateColliderLocationsForLimb(this, i, &limbPos);
        MATRIX_FINALIZE_AND_LOAD(gfx++, play->state.gfxCtx);
        gSPDisplayList(gfx++, gDexihandArmSegmentDL);
        Matrix_Pop();
    }

    EnWdhand_SetTransform(this, EN_WDHAND_NUM_SEGMENTS, true, &limbPos);

    if (this->globalLimbScaleFactor > 1.0f) {
        f32 scale = CLAMP_MAX(this->globalLimbScaleFactor, 1.5f);

        Matrix_RotateZS((scale - 1.0f) * -32768.0f, MTXMODE_APPLY);
    }
    if (this->actor.scale.x < 0.01f) {
        Matrix_Translate(0.0f, (10.0f / this->actor.scale.x) - 1000.0f, 0.0f, MTXMODE_APPLY);
    }

    Math_Vec3f_Copy(&this->actor.focus.pos, &limbPos);
    Matrix_MultVecY(1000.0f, &limbPos);
    EnWdhand_Vec3fToVec3s(&this->collider.elements[6].dim.worldSphere.center, &limbPos);

    POLY_OPA_DISP = gfx;
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x1d1, En_Wdhand_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_WDHAND, En_Wdhand_Profile, EnWdhand_Init, EnWdhand_Destroy,
                     EnWdhand_Update, EnWdhand_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x1d1u, 0x1b5u, 0x87f738a1u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_WDHAND_ORIGIN, generatedNativeOrigin);
