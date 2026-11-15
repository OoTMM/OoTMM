#include "mm_native_compat.h"
#define gSkullFishDieAnim (*(AnimationHeader*)0x06003904)
#define gSkullFishSkel (*(FlexSkeletonHeader*)0x06004188)
#define gSkullFishAttackAnim (*(AnimationHeader*)0x06004274)
#define gSkullFishSwimAnim (*(AnimationHeader*)0x06004340)

#include "types/en_pr2.h"
#define TATL_HINT_ID_SKULLFISH 0xff
#undef ACTOR_EN_PR2
#define ACTOR_EN_PR2 0x180
#undef OBJECT_PR
#define OBJECT_PR 0x116b
#undef NA_SE_EN_PIRANHA_EXIST
#define NA_SE_EN_PIRANHA_EXIST 0x39f3
#undef NA_SE_EN_PIRANHA_ATTACK
#define NA_SE_EN_PIRANHA_ATTACK 0x39f4
#undef NA_SE_EN_PIRANHA_DEAD
#define NA_SE_EN_PIRANHA_DEAD 0x39f5
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Scene_SetRenderModeXlu
#define Scene_SetRenderModeXlu ((void (*)(struct PlayState* play, s32 index, u32 flags))(void*)FOREIGN_ENEMY_API_RENDER_XLU)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f34u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef BgCheck_SphVsFirstPoly
#define BgCheck_SphVsFirstPoly ((s32 (*)(CollisionContext* colCtx, Vec3f* center, f32 radius))0x80030d08u)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef EffectSsBubble_Spawn
#define EffectSsBubble_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale))0x8001cedcu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef SkelAnime_DrawFlex
#define SkelAnime_DrawFlex ((Gfx* (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, struct Actor* actor, Gfx* gfx))0x8008a88cu)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnPr2_Init(Actor* thisx, PlayState* play);
void EnPr2_Destroy(Actor* thisx, PlayState* play);
void EnPr2_Update(Actor* thisx, PlayState* play);
void EnPr2_Draw(Actor* thisx, PlayState* play);

void EnPr2_SetupFollowPath(EnPr2* this);
void EnPr2_FollowPath(EnPr2* this, PlayState* play);
void EnPr2_SetupIdle(EnPr2* this);
void EnPr2_Idle(EnPr2* this, PlayState* play);
void EnPr2_SetupAttack(EnPr2* this, PlayState* play);
void EnPr2_Attack(EnPr2* this, PlayState* play);
void EnPr2_SetupDie(EnPr2* this);
void EnPr2_Die(EnPr2* this, PlayState* play);

typedef enum EnPr2DamageEffect {
    PR2_DMGEFF_NONE,
    PR2_DMGEFF_BREAK = 0xF,
} EnPr2DamageEffect;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(1, PR2_DMGEFF_BREAK),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK),
    DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(2, PR2_DMGEFF_BREAK), DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK), DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(0, PR2_DMGEFF_NONE),
    DMG_ENTRY(0, PR2_DMGEFF_NONE),  DMG_ENTRY(1, PR2_DMGEFF_BREAK),
} };

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_1,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x08, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 17, 32, -10, { 0, 0, 0 } },
};

ActorProfile En_Pr2_Profile = {
    0xd80, ACTORCAT_ENEMY, FLAGS, 0x116b, sizeof(EnPr2), EnPr2_Init, EnPr2_Destroy, EnPr2_Update, EnPr2_Draw,
};

typedef enum EnPr2Animation { PR2_ANIM_SWIM, PR2_ANIM_ATTACK, PR2_ANIM_DIE, PR2_ANIM_MAX } EnPr2Animation;

static AnimationHeader* sAnimations[PR2_ANIM_MAX] = {
    &gSkullFishSwimAnim,
    &gSkullFishAttackAnim,
    &gSkullFishDieAnim,
};

static u8 sAnimationModes[PR2_ANIM_MAX] = {
    ANIMMODE_LOOP,
    ANIMMODE_LOOP,
    ANIMMODE_ONCE,
};

void EnPr2_Init(Actor* thisx, PlayState* play) {
    if (((thisx->params & 15) != 2) && (((thisx->params & 15) < 10) || ((thisx->params & 15) > 14))) {
        Actor_Kill(thisx);
        return;
    }
    thisx->world.rot.z = 0;
    EnPr2* this = (EnPr2*)thisx;

    this->actor.attentionRangeType = ATTENTION_RANGE_3;
    this->actor.hintId = TATL_HINT_ID_SKULLFISH;
    this->primColor = 255;
    this->actor.colChkInfo.health = 1;
    this->actor.colChkInfo.damageTable = &sDamageTable;

    SkelAnime_InitFlex(play, &this->skelAnime, &gSkullFishSkel, &gSkullFishSwimAnim, this->jointTable, this->morphTable,
                       PR_2_LIMB_MAX);
    this->type = ENPR2_GET_TYPE(&this->actor);
    this->actor.colChkInfo.mass = 10;
    Math_Vec3f_Copy(&this->newHome, &this->actor.home.pos);

    if (this->type == PR2_TYPE_RESIDENT) {
        this->agroDistance = ENPR2_GET_AGRO_DISTANCE(&this->actor) * 20.0f;
    }

    this->alpha = 255;
    this->actor.shape.yOffset = 500.0f;
    this->actor.shape.shadowScale = 12.0f;

    if (this->type < PR2_TYPE_BROKEN) {
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 19.0f);
        Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
        this->dropID = -1;
        this->scale = 0.0f;

        EnPr2_SetupIdle(this);
    } else {
        this->scale = 0.02f;
        EnPr2_SetupDie(this);
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 20.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);

    if (!(this->actor.bgCheckFlags & (BGCHECKFLAG_WATER | BGCHECKFLAG_WATER_TOUCH))) {
        Actor_Kill(&this->actor);
    }
}

void EnPr2_Destroy(Actor* thisx, PlayState* play) {
    EnPr2* this = (EnPr2*)thisx;

    if (this->type < PR2_TYPE_BROKEN) {
        Collider_DestroyCylinder(play, &this->collider);
    }
}

s32 EnPr2_IsOnScreen(EnPr2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 screenPosX;
    s16 screenPosY;

    Actor_GetScreenPos(play, &this->actor, &screenPosX, &screenPosY);

    if ((fabsf(player->actor.world.pos.y - this->actor.world.pos.y) > 160.0f) ||
        (this->actor.projectedPos.z < -40.0f) || (screenPosX < 0) || (screenPosX > SCREEN_WIDTH) || (screenPosY < 0) ||
        (screenPosY > SCREEN_HEIGHT)) {
        return false;
    }

    if (!(player->stateFlags1 & PLAYER_STATE1_8000000)) {
        return false;
    } else {
        return true;
    }
}

void EnPr2_HandleYaw(EnPr2* this, s16 targetYRot) {
    s16 yawDiff = targetYRot - this->actor.world.rot.y;

    if (yawDiff > 10000) {
        yawDiff = 10000;
    } else if (yawDiff < -10000) {
        yawDiff = -10000;
    }

    Math_ApproachF(&this->actor.world.pos.y, this->waypointPos.y, 0.3f, 5.0f);

    if (fabsf(this->actor.world.pos.y - this->waypointPos.y) > 10.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.x,
                           Math_Vec3f_Pitch(&this->actor.world.pos, &this->waypointPos) * 0.3f, 20, 0x1388, 0x1F4);
    } else {
        Math_SmoothStepToS(&this->actor.world.rot.x, 0, 20, 0x1388, 0x1F4);
    }

    if (fabsf(this->actor.world.rot.y - targetYRot) < 30.0f) {
        Math_ApproachZeroF(&this->slowLimbYaw, 0.5f, 20.0f);
    } else {
        Math_ApproachF(&this->slowLimbYaw, yawDiff, 0.5f, 3000.0f);
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, targetYRot, 1, 0x7D0, 300);
}

void EnPr2_ChangeAnim(EnPr2* this, s32 animIndex) {
    f32 playSpeed = 1.0f;

    this->animIndex = animIndex;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[animIndex]);
    if (this->animIndex == PR2_ANIM_MAX) {
        playSpeed = 0.0f;
    }
    Animation_Change(&this->skelAnime, sAnimations[animIndex], playSpeed, 0.0f, this->animEndFrame,
                     sAnimationModes[animIndex], 0.0f);
}

void EnPr2_SetupFollowPath(EnPr2* this) {
    EnPr2_ChangeAnim(this, PR2_ANIM_SWIM);
    this->state = PR2_STATE_PATHING;
    this->actionFunc = EnPr2_FollowPath;
}

void EnPr2_FollowPath(EnPr2* this, PlayState* play) {
    EnPr2_SetupIdle(this);
}

void EnPr2_SetupIdle(EnPr2* this) {
    EnPr2_ChangeAnim(this, PR2_ANIM_SWIM);
    this->idleTimer = 2;
    this->mainTimer = 0;
    Math_Vec3f_Copy(&this->waypointPos, &this->newHome);
    this->state = PR2_STATE_IDLE;
    this->actionFunc = EnPr2_Idle;
}

void EnPr2_Idle(EnPr2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 deltaX;
    f32 deltaZ;
    f32 sqrtXZ;
    s32 changingCourse = false;
    s32 swimmingStraight = false;
    Vec3f targetPos;

    Math_ApproachF(&this->scale, 0.02f, 0.1f, 0.005f);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIRANHA_EXIST - SFX_FLAG);

    if (fabsf(this->actor.world.rot.y - this->yawTowardsWaypoint) < 200.0f) {
        swimmingStraight = true;
        SkelAnime_Update(&this->skelAnime);
    }

    if (this->alpha != 255) {
        this->actor.speed = 0.0f;
        Math_SmoothStepToS(&this->alpha, 0, 1, 30, 100);
        if (this->alpha < 2) {
            Actor_Kill(&this->actor);
        }
    } else {
        switch (this->type) {
            case PR2_TYPE_SPAWNED:
                if (this->targetingTimer == 0) {
                    changingCourse = true;
                    EnPr2_SetupAttack(this, play);
                } else if (!EnPr2_IsOnScreen(this, play) && (this->alpha == 255)) {
                    this->alpha = 254;
                }
                break;

            case PR2_TYPE_RESIDENT:
                if (this->returnHomeTimer == 0) {
                    deltaX = player->actor.world.pos.x - this->newHome.x;
                    deltaZ = player->actor.world.pos.z - this->newHome.z;
                    sqrtXZ = sqrtf(SQ(deltaX) + SQ(deltaZ));

                    if (swimmingStraight && (player->stateFlags1 & PLAYER_STATE1_8000000) &&
                        (sqrtXZ < this->agroDistance)) {
                        changingCourse = true;
                        EnPr2_SetupAttack(this, play);
                    }
                } else {
                    deltaX = this->actor.world.pos.x - this->newHome.x;
                    deltaZ = this->actor.world.pos.z - this->newHome.z;
                    sqrtXZ = sqrtf(SQ(deltaX) + SQ(deltaZ));

                    if (sqrtXZ > 20.0f) {
                        this->returnHomeTimer = 5;
                        changingCourse = true;
                        this->targetingTimer = 0;
                        Math_Vec3f_Copy(&this->waypointPos, &this->newHome);
                        Math_ApproachF(&this->actor.speed, 3.0f, 0.3f, 0.2f);
                    }
                }
                break;

            default:
                break;
        }

        if (!changingCourse) {
            this->waypointPos.y = this->actor.world.pos.y;
            if (this->idleTimer != 0) {
                if ((Rand_ZeroOne() < 0.3f) && !this->bubbleToggle) {
                    this->bubbleToggle = true;
                }

                Math_ApproachZeroF(&this->actor.speed, 0.1f, 0.2f);

                if (this->idleTimer == 1) {
                    this->mainTimer = Rand_S16Offset(100, 100);
                    Math_Vec3f_Copy(&targetPos, &this->newHome);
                    targetPos.x += Rand_CenteredFloat(300.0f);
                    targetPos.z += Rand_CenteredFloat(300.0f);
                    Math_Vec3f_Copy(&this->waypointPos, &targetPos);
                }
            } else {
                Math_ApproachF(&this->actor.speed, 2.0f, 0.3f, 0.2f);
                Math_Vec3f_Copy(&targetPos, &this->actor.world.pos);
                targetPos.x += Math_SinS(this->actor.world.rot.y) * 20.0f;
                targetPos.z += Math_CosS(this->actor.world.rot.y) * 20.0f;
                if (fabsf(this->actor.world.rot.y - this->yawTowardsWaypoint) < 100.0f) {
                    if (BgCheck_SphVsFirstPoly(&play->colCtx, &targetPos, 20.0f) ||
                        (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
                        this->targetingTimer = 0;
                        this->wallCollisionCounter++;
                        Math_Vec3f_Copy(&this->waypointPos, &this->newHome);
                        if (this->wallCollisionCounter > 10) {
                            this->wallCollisionAngleAdjustment += 0x2000;
                        }
                    } else {
                        Math_SmoothStepToS(&this->wallCollisionAngleAdjustment, 0, 1, 0x3E8, 0x64);
                        this->wallCollisionCounter = 0;
                    }
                }

                if ((this->mainTimer == 0) || ((fabsf(this->waypointPos.x - this->actor.world.pos.x) < 10.0f) &&
                                               (fabsf(this->waypointPos.z - this->actor.world.pos.z) < 10.0f))) {
                    this->idleTimer = Rand_S16Offset(20, 30);
                }
            }
        }

        if (this->idleTimer == 0) {
            this->yawTowardsWaypoint =
                Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos) + this->wallCollisionAngleAdjustment;
            EnPr2_HandleYaw(this, this->yawTowardsWaypoint);
        }
    }
}

void EnPr2_SetupAttack(EnPr2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->wallCollisionAngleAdjustment = 0;
    EnPr2_ChangeAnim(this, PR2_ANIM_ATTACK);
    Actor_PlaySfx(&this->actor, NA_SE_EN_PIRANHA_ATTACK);
    Math_Vec3f_Copy(&this->waypointPos, &player->actor.world.pos);

    this->yawTowardsWaypoint = Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos);
    this->randomTargetHeightOffset = Rand_ZeroFloat(30.0f);
    this->targetingTimer = 0;
    this->mainTimer = 3.5 * 20;
    this->state = PR2_STATE_ATTACKING;
    this->actionFunc = EnPr2_Attack;
}

void EnPr2_Attack(EnPr2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    WaterBox* waterBox;

    Math_ApproachF(&this->scale, 0.02f, 0.1f, 0.005f);
    if ((this->mainTimer == 0) || !(player->stateFlags1 & PLAYER_STATE1_8000000) || (this->type == PR2_TYPE_PASSIVE)) {
        EnPr2_SetupIdle(this);
        return;
    }

    if (this->alpha != 255) {
        this->actor.speed = 0.0f;
        Math_SmoothStepToS(&this->alpha, 0, 1, 30, 100);
        if (this->alpha < 2) {
            Actor_Kill(&this->actor);
        }
    } else {
        SkelAnime_Update(&this->skelAnime);

        if ((this->targetingTimer == 0) && (fabsf(this->actor.world.rot.y - this->yawTowardsWaypoint) < 200.0f)) {
            Math_Vec3f_Copy(&this->waypointPos, &player->actor.world.pos);
        }

        if ((Rand_ZeroOne() < 0.3f) && !this->bubbleToggle) {
            this->bubbleToggle = true;
            this->randomTargetHeightOffset = Rand_ZeroFloat(30.0f);
        }

        this->waypointPos.y = player->actor.world.pos.y + 30.0f + this->randomTargetHeightOffset;
        Math_ApproachF(&this->actor.speed, 5.0f, 0.3f, 1.0f);
        this->wallCollisionAngleAdjustment = 0;

        if (this->type == PR2_TYPE_RESIDENT) {
            f32 deltaX = this->actor.world.pos.x - this->newHome.x;
            f32 deltaZ = this->actor.world.pos.z - this->newHome.z;
            f32 homeDistance = sqrtf(SQ(deltaX) + SQ(deltaZ));

            if (homeDistance > this->agroDistance) {
                this->returnHomeTimer = 20;
                EnPr2_SetupIdle(this);
                return;
            }
        } else {
            Math_Vec3f_Copy(&this->newHome, &this->actor.world.pos);
        }

        if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                             &this->waterSurfaceHeight, &waterBox)) {
            if (this->waypointPos.y > (this->waterSurfaceHeight - 40.0f)) {
                this->waypointPos.y = this->waterSurfaceHeight - 40.0f;
            }
        }

        if ((this->type == PR2_TYPE_SPAWNED) && !EnPr2_IsOnScreen(this, play)) {
            if (this->alpha == 255) {
                this->alpha = 254;
            }
        } else {
            if (this->collider.base.atFlags & AT_HIT) {
                this->targetingTimer = Rand_S16Offset(30, 30);
                this->mainTimer = 5 * 20;
                if (this->type != PR2_TYPE_RESIDENT) {
                    EnPr2_SetupIdle(this);
                }
            }
            this->yawTowardsWaypoint = Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos);
            EnPr2_HandleYaw(this, this->yawTowardsWaypoint);
        }
    }
}

void EnPr2_SetupDie(EnPr2* this) {
    this->primColor = 0;
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (this->type < PR2_TYPE_BROKEN) {
        EnPr2_ChangeAnim(this, PR2_ANIM_DIE);
    } else {
        this->animEndFrame = Animation_GetLastFrame(&gSkullFishDieAnim);
        Animation_Change(&this->skelAnime, &gSkullFishDieAnim, 1.0f, this->animEndFrame, this->animEndFrame,
                         ANIMMODE_ONCE, 0.0f);
        this->mainTimer = Rand_S16Offset(20, 30);
        this->targetZRot = 0x4000;
        if (Rand_ZeroOne() < 0.5f) {
            this->targetZRot = -0x4000;
        }
        this->targetYRot = this->actor.world.rot.y;
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->actor.shape.rot.z = this->actor.world.rot.z;
        this->mainTimer = 1.5 * 20;
        this->actor.speed = Rand_ZeroFloat(0.5f);
        this->actor.world.rot.y = Rand_CenteredFloat(0x8000);
    }
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 10);
    this->state = PR2_STATE_DEAD;
    this->actionFunc = EnPr2_Die;
}

void EnPr2_Die(EnPr2* this, PlayState* play) {
    s32 nearSurface;
    f32 curFrame;
    WaterBox* waterBox;

    SkelAnime_Update(&this->skelAnime);
    if (this->type < PR2_TYPE_BROKEN) {
        s32 i;

        curFrame = this->skelAnime.curFrame;

        if (curFrame >= this->animEndFrame) {
            for (i = 0; i < ARRAY_COUNT(this->limbPos); i++) {
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PR2, this->limbPos[i].x, this->limbPos[i].y,
                            this->limbPos[i].z, this->actor.world.rot.x, this->actor.world.rot.y,
                            this->actor.world.rot.z, i + PR2_TYPE_BROKEN);
            }

            Actor_Kill(&this->actor);
            return;
        }
    } else {
        Vec3f bubblePos;
        nearSurface = false;

        Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 5, 0x2710, 0x3E8);
        Math_SmoothStepToS(&this->actor.shape.rot.z, this->targetZRot, 5, 0x2710, 0x3E8);
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->targetYRot, 5, 0x2710, 0x3E8);

        if ((Rand_ZeroOne() < 0.3f) && !this->bubbleToggle) {
            this->bubbleToggle = true;
        }

        if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                             &this->waterSurfaceHeight, &waterBox)) {
            if (this->actor.world.pos.y >= (this->waterSurfaceHeight - 15.0f)) {
                nearSurface = true;
            } else {
                Math_ApproachF(&this->actor.world.pos.y, this->waterSurfaceHeight - 15.0f, 0.3f, 1.0f);
            }
        }

        if ((this->mainTimer == 0) || nearSurface) {
            s32 i;

            Math_SmoothStepToS(&this->alpha, 0, 1, 15, 50);

            if (this->alpha < 2) {
                for (i = 0; i < 10; i++) {
                    Math_Vec3f_Copy(&bubblePos, &this->actor.world.pos);

                    bubblePos.x += Rand_CenteredFloat(20.0f);
                    bubblePos.y += Rand_CenteredFloat(5.0f);
                    bubblePos.z += Rand_CenteredFloat(20.0f);

                    EffectSsBubble_Spawn(play, &bubblePos, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
                }

                Actor_Kill(&this->actor);
            }
        }
    }
}

void EnPr2_ApplyDamage(EnPr2* this, PlayState* play) {

    if (this->collider.base.acFlags & AC_HIT) {
        Actor_ApplyDamage(&this->actor);

        if ((this->actor.colChkInfo.health <= 0) && (this->state != PR2_STATE_DEAD)) {
            Enemy_StartFinishingBlow(play, &this->actor);
            this->actor.speed = 0.0f;
            Actor_PlaySfx(&this->actor, NA_SE_EN_PIRANHA_DEAD);

            this->actor.colChkInfo.health = 0;
            EnPr2_SetupDie(this);
        }
    }

    if (this->collider.base.atFlags & AT_BOUNCED) {
        this->actor.speed = -10.0f;
    }
}

void EnPr2_Update(Actor* thisx, PlayState* play) {
    EnPr2* this = (EnPr2*)thisx;

    Actor_SetScale(&this->actor, this->scale);

    this->actionFunc(this, play);

    DECR(this->idleTimer);
    DECR(this->mainTimer);
    DECR(this->targetingTimer);
    DECR(this->returnHomeTimer);

    Actor_SetFocus(&this->actor, 10.0f);
    EnPr2_ApplyDamage(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0, 10.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);

    if (this->bubbleToggle) {
        s32 i;
        Vec3f bubblePos;
        f32 bubbleScale;

        Math_Vec3f_Copy(&bubblePos, &this->limbJawPos);
        this->bubbleToggle = false;

        bubblePos.x += Rand_CenteredFloat(20.0f);
        bubblePos.y += Rand_CenteredFloat(5.0f);
        bubblePos.z += Rand_CenteredFloat(20.0f);

        for (i = 0; i < 2; i++) {
            bubbleScale = Rand_ZeroFloat(0.03f) + 0.07f;
            EffectSsBubble_Spawn(play, &bubblePos, 0, 5.0f, 5.0f, bubbleScale);
        }
    }

    if ((this->alpha == 255) && (this->type < PR2_TYPE_BROKEN)) {
        this->actor.shape.rot.x = this->actor.world.rot.x;
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->actor.shape.rot.z = this->actor.world.rot.z;
        if (this->state != PR2_STATE_DEAD) {
            Collider_UpdateCylinder(&this->actor, &this->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

s32 EnPr2_OverrideLimbDrawOpa(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnPr2* this = (EnPr2*)thisx;

    if (this->type < PR2_TYPE_BROKEN) {
        if (limbIndex == PR_2_LIMB_SKULL) {
            rot->y += TRUNCF_BINANG(this->slowLimbYaw) * -1;
        }
    } else if (this->type != limbIndex + PR2_TYPE_BROKEN) {
        *dList = NULL;
    }
    return false;
}

void EnPr2_PostLimbDrawOpa(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnPr2* this = (EnPr2*)thisx;

    if (this->type < PR2_TYPE_BROKEN) {
        if (limbIndex == PR_2_LIMB_SKULL) {
            Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            Matrix_MultZero(&this->limbJawPos);
        }

        Matrix_MultZero(&this->limbPos[limbIndex]);
    }
}

s32 EnPr2_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                           Gfx** gfx) {
    EnPr2* this = (EnPr2*)thisx;

    if (this->type < PR2_TYPE_BROKEN) {
        if (limbIndex == PR_2_LIMB_SKULL) {
            rot->y += TRUNCF_BINANG(this->slowLimbYaw) * -1;
        }
    } else if ((limbIndex + PR2_TYPE_BROKEN) != this->type) {
        *dList = NULL;
    }
    return false;
}

void EnPr2_Draw(Actor* thisx, PlayState* play) {
    EnPr2* this = (EnPr2*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    if (this->alpha == 255) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, this->primColor, this->primColor, this->primColor, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->alpha);

        Scene_SetRenderModeXlu(play, 0, 0x1);
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              EnPr2_OverrideLimbDrawOpa, EnPr2_PostLimbDrawOpa, &this->actor);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);

        Scene_SetRenderModeXlu(play, 1, 0x2);
        POLY_XLU_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               EnPr2_OverrideLimbDraw, NULL, &this->actor, POLY_XLU_DISP);
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x180, En_Pr2_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_PR2, En_Pr2_Profile, EnPr2_Init, EnPr2_Destroy, EnPr2_Update, EnPr2_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x180u, 0x16bu, 0xac0bb273u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_PR2_ORIGIN, generatedNativeOrigin);
