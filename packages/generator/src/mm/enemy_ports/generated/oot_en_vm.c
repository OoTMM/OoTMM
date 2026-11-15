#include "oot_variant_compat.h"
#include "types/en_vm.h"
#define gEffEnemyDeathFlame2Tex ((u8*)0x0402dc80)
#define gEffEnemyDeathFlame3Tex ((u8*)0x0402e480)
#define gEffEnemyDeathFlame4Tex ((u8*)0x0402ec80)
#define gEffEnemyDeathFlame5Tex ((u8*)0x0402f480)
#define gEffEnemyDeathFlame6Tex ((u8*)0x0402fc80)
#define gEffEnemyDeathFlame7Tex ((u8*)0x04030480)
#define gEffEnemyDeathFlame8Tex ((u8*)0x04030c80)
#define gEffEnemyDeathFlame9Tex ((u8*)0x04031480)
#define gEffEnemyDeathFlame10Tex ((u8*)0x04031c80)
#define gEffEnemyDeathFlameDL ((Gfx*)0x04032480)
#undef ACTOR_EN_VM
#define ACTOR_EN_VM 0xc8a
#undef OBJECT_VM
#define OBJECT_VM 0x108b
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
extern void FOREIGN_ENEMY_API_KILL(Actor* actor);
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
extern void FOREIGN_ENEMY_API_SFX_ACTOR(Actor* actor, u16 sfxId);
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_SetColorFilter
extern void FOREIGN_ENEMY_API_COLOR_FILTER(Actor* actor, s16 colorFlag, s16 colorIntensityMax, s16 bufFlag,
                                           s16 duration);
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_Spawn
extern Actor* FOREIGN_ENEMY_API_SPAWN(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                      f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_Spawn FOREIGN_ENEMY_API_SPAWN
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x801373e8)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x80134748)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*))0x800c55c4)
#undef Collider_DestroyCylinder
extern s32 FOREIGN_ENEMY_API_CYLINDER_DESTROY(PlayState* play, ColliderCylinder* cyl);
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
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
extern s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
extern s32 FOREIGN_ENEMY_API_COLLISION_AT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
extern s32 FOREIGN_ENEMY_API_COLLISION_OC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Item_DropCollectibleRandom
extern void FOREIGN_ENEMY_API_RANDOM_DROP(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXYZ
#define Math_Vec3f_DistXYZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff884)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x800ffa94)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_RotateY
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_YF(f32 y, u8 mode);
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, u8 mode))0x8018103c)
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
#undef func_80094E78
#define func_80094E78 ((Gfx* (*)(GraphicsContext* gfxCtx, u32 x, u32 y))0x8012cb28)
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389E
#undef NA_SE_EN_BIMOS_ROLL_HEAD
#define NA_SE_EN_BIMOS_ROLL_HEAD 0x3970
#undef NA_SE_EN_BIMOS_LAZER
#define NA_SE_EN_BIMOS_LAZER 0x3971
#undef NA_SE_EN_BIMOS_LAZER_GND
#define NA_SE_EN_BIMOS_LAZER_GND 0x3972
#undef NA_SE_EN_BIMOS_AIM
#define NA_SE_EN_BIMOS_AIM 0x3973
#define gBeamosSkel (*(SkeletonHeader*)0x06003f60)
#define gBeamosAnim (*(AnimationHeader*)0x06000068)
#define gBeamosLaserDL ((Gfx*)0x06002728)
#define gEffEnemyDeathFlame1Tex ((u8*)0x0402d480)

static void func_80033C30(Vec3f* pos, Vec3f* scale, u8 alpha, PlayState* play) {
    MtxF floorMatrix;
    Vec3f check = { pos->x, pos->y + 1.0f, pos->z };
    CollisionPoly* poly = NULL;
    f32 floor = ((f32(*)(PlayState*, CollisionContext*, CollisionPoly**, Vec3f*))0x800c4000)(play, &play->colCtx, &poly,
                                                                                             &check);
    OPEN_DISPS(play->state.gfxCtx);
    POLY_OPA_DISP = ((Gfx * (*)(Gfx*, s32))0x8012bf78)(POLY_OPA_DISP, 44);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, alpha);
    if (poly) {
        ((void (*)(CollisionPoly*, f32, f32, f32, MtxF*))0x800c0094)(poly, pos->x, floor, pos->z, &floorMatrix);
        Matrix_Put(&floorMatrix);
    } else
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
    Matrix_Scale(scale->x, 1.0f, scale->z, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, (Gfx*)0x0404e740);
    CLOSE_DISPS();
}

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnVm_Init(Actor* thisx, PlayState* play);
void EnVm_Destroy(Actor* thisx, PlayState* play);
void EnVm_Update(Actor* thisx, PlayState* play);
void EnVm_Draw(Actor* thisx, PlayState* play2);

void EnVm_SetupWait(EnVm* this);
void EnVm_Wait(EnVm* this, PlayState* play);
void EnVm_SetupAttack(EnVm* this);
void EnVm_Attack(EnVm* this, PlayState* play);
void EnVm_Stun(EnVm* this, PlayState* play);
void EnVm_Die(EnVm* this, PlayState* play);

ActorProfile En_Vm_Profile = {
    ACTOR_EN_VM, ACTORCAT_ENEMY, FLAGS, OBJECT_VM, sizeof(EnVm), EnVm_Init, EnVm_Destroy, EnVm_Update, EnVm_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 25, 70, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit1 = {
    {
        COL_MATERIAL_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x10 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static ColliderQuadInit sQuadInit2 = {
    {
        COL_MATERIAL_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static Vec3f D_80B2EAEC = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EAF8 = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB04 = { 500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB10 = { -500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB1C = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB28 = { 0.0f, 0.0f, 1600.0f };

static Vec3f D_80B2EB34 = { 1000.0f, 700.0f, 2000.0f };

static Vec3f D_80B2EB40 = { 1000.0f, -700.0f, 2000.0f };

static Vec3f D_80B2EB4C = { -1000.0f, 700.0f, 1500.0f };

static Vec3f D_80B2EB58 = { -1000.0f, -700.0f, 1500.0f };

static Vec3f D_80B2EB64 = { 500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB70 = { -500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB7C = { 0.4f, 0.4f, 0.4f };

static void* D_80B2EB88[] = {
    gEffEnemyDeathFlame1Tex, gEffEnemyDeathFlame2Tex,  gEffEnemyDeathFlame3Tex, gEffEnemyDeathFlame4Tex,
    gEffEnemyDeathFlame5Tex, gEffEnemyDeathFlame6Tex,  gEffEnemyDeathFlame7Tex, gEffEnemyDeathFlame8Tex,
    gEffEnemyDeathFlame9Tex, gEffEnemyDeathFlame10Tex,
};

void EnVm_SetupAction(EnVm* this, EnVmActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnVm_Init(Actor* thisx, PlayState* play) {
    EnVm* this = (EnVm*)thisx;

    SkelAnime_Init(play, &this->skelAnime, &gBeamosSkel, &gBeamosAnim, this->jointTable, this->morphTable, 11);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    Collider_InitCylinder(play, &this->colliderCylinder);
    Collider_SetCylinder(play, &this->colliderCylinder, thisx, &sCylinderInit);
    Collider_InitQuad(play, &this->colliderQuad1);
    Collider_SetQuad(play, &this->colliderQuad1, thisx, &sQuadInit1);
    Collider_InitQuad(play, &this->colliderQuad2);
    Collider_SetQuad(play, &this->colliderQuad2, thisx, &sQuadInit2);
    this->beamSightRange = PARAMS_GET_NOMASK(thisx->params, 8) * 40.0f;
    thisx->params &= 0xFF;
    thisx->naviEnemyId = TATL_HINT_ID_BEAMOS;

    if (thisx->params == BEAMOS_LARGE) {
        thisx->colChkInfo.health = 2;
        Actor_SetScale(thisx, 0.014f);
    } else {
        thisx->colChkInfo.health = 1;
        Actor_SetScale(thisx, 0.01f);
    }

    EnVm_SetupWait(this);
}

void EnVm_Destroy(Actor* thisx, PlayState* play) {
    EnVm* this = (EnVm*)thisx;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
}

void EnVm_SetupWait(EnVm* this) {
    f32 frameCount = Animation_GetLastFrame(&gBeamosAnim);

    Animation_Change(&this->skelAnime, &gBeamosAnim, 1.0f, frameCount, frameCount, ANIMMODE_ONCE, 0.0f);
    this->unk_25E = this->unk_260 = 0;
    this->unk_21C = 0;
    this->timer = 10;
    EnVm_SetupAction(this, EnVm_Wait);
}

void EnVm_Wait(EnVm* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 dist;
    s16 headRot;
    s16 pad;
    s16 pitch;

    switch (this->unk_25E) {
        case 0:
            Math_SmoothStepToS(&this->beamRot.x, 0, 10, 1500, 0);
            headRot = this->actor.yawTowardsPlayer - this->headRotY - this->actor.shape.rot.y;
            pitch = Math_Vec3f_Pitch(&this->beamPos1, &player->actor.world.pos);

            if (pitch > 0x1B91) {
                pitch = 0x1B91;
            }

            dist = this->beamSightRange - this->actor.xzDistToPlayer;

            if (this->actor.xzDistToPlayer <= this->beamSightRange && ABS(headRot) <= 0x2710 && pitch >= 0xE38 &&
                this->actor.yDistanceFromLink <= 80.0f && this->actor.yDistanceFromLink >= -160.0f) {
                Math_SmoothStepToS(&this->beamRot.x, pitch, 10, 0xFA0, 0);
                if (Math_SmoothStepToS(&this->headRotY, this->actor.yawTowardsPlayer - this->actor.shape.rot.y, 1,
                                       (ABS((s16)(dist * 180.0f)) / 3) + 0xFA0, 0) <= 5460) {
                    this->timer--;
                    if (this->timer == 0) {
                        this->unk_25E++;
                        this->skelAnime.curFrame = 0.0f;
                        this->skelAnime.startFrame = 0.0f;
                        this->skelAnime.playSpeed = 2.0f;
                        Actor_PlaySfx(&this->actor, NA_SE_EN_BIMOS_AIM);
                    }
                }
            } else {
                this->headRotY -= 0x1F4;
            }

            SkelAnime_Update(&this->skelAnime);
            return;
        case 1:
            break;
        default:
            return;
    }

    Math_SmoothStepToS(&this->headRotY, this->actor.yawTowardsPlayer - this->actor.shape.rot.y, 1, 0x1F40, 0);

    if (SkelAnime_Update(&this->skelAnime)) {
        this->unk_260++;
        this->skelAnime.curFrame = 0.0f;
    }

    if (this->unk_260 == 2) {
        this->beamRot.y = this->actor.yawTowardsPlayer;
        this->beamRot.x = Math_Vec3f_Pitch(&this->beamPos1, &player->actor.world.pos);

        if (this->beamRot.x > 0x1B91) {
            this->beamRot.x = 0x1B91;
        }

        if (this->beamRot.x < 0xAAA) {
            this->skelAnime.startFrame = this->skelAnime.curFrame = this->skelAnime.endFrame;
            this->unk_25E = this->unk_260 = 0;
            this->timer = 10;
            this->skelAnime.playSpeed = 1.0f;
        } else {
            this->skelAnime.curFrame = 6.0f;
            EffectSsDeadDd_Spawn(play, &this->beamPos2, &D_80B2EAEC, &D_80B2EAEC, 150, -25, 0, 0, 255, 0, 255, 255, 255,
                                 16, 20);
            EnVm_SetupAttack(this);
        }
    }
}

void EnVm_SetupAttack(EnVm* this) {
    Animation_Change(&this->skelAnime, &gBeamosAnim, 3.0f, 3.0f, 7.0f, ANIMMODE_ONCE, 0.0f);
    this->timer = 305;
    this->beamScale.x = 0.6f;
    this->beamSpeed = 40.0f;
    this->unk_21C = 1;
    this->colliderQuad1.base.atFlags &= ~AT_HIT;
    EnVm_SetupAction(this, EnVm_Attack);
}

void EnVm_Attack(EnVm* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 pitch = Math_Vec3f_Pitch(&this->beamPos1, &player->actor.world.pos);
    f32 dist;
    Vec3f playerPos;

    if (pitch > 0x1B91) {
        pitch = 0x1B91;
    }

    if (this->colliderQuad1.base.atFlags & AT_HIT) {
        this->colliderQuad1.base.atFlags &= ~AT_HIT;
        this->timer = 0;

        if (this->beamScale.x > 0.1f) {
            this->beamScale.x = 0.1f;
        }
    }

    if (this->beamRot.x < 0xAAA || this->timer == 0) {
        Math_SmoothStepToF(&this->beamScale.x, 0.0f, 1.0f, 0.03f, 0.0f);
        this->unk_260 = 0;

        if (this->beamScale.x == 0.0f) {
            this->beamScale.y = this->beamScale.z = 0.0f;
            EnVm_SetupWait(this);
        }
    } else {
        if (--this->timer > 300) {
            return;
        }

        Math_SmoothStepToS(&this->headRotY, -this->actor.shape.rot.y + this->actor.yawTowardsPlayer, 10, 0xDAC, 0);
        Math_SmoothStepToS(&this->beamRot.y, this->actor.yawTowardsPlayer, 10, 0xDAC, 0);
        Math_SmoothStepToS(&this->beamRot.x, pitch, 10, 0xDAC, 0);
        playerPos = player->actor.world.pos;

        if (player->actor.floorHeight > BGCHECK_Y_MIN) {
            playerPos.y = player->actor.floorHeight;
        }

        dist = Math_Vec3f_DistXYZ(&this->beamPos1, &playerPos);
        Math_SmoothStepToF(&this->beamScale.z, dist, 1.0f, this->beamSpeed, 0.0f);
        Math_SmoothStepToF(&this->beamScale.x, 0.1f, 1.0f, 0.12f, 0.0f);
        Actor_PlaySfx(&this->actor, NA_SE_EN_BIMOS_LAZER - SFX_FLAG);

        if (this->unk_260 > 2) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderQuad1.base);
        }

        this->unk_260 = 3;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        this->skelAnime.curFrame = this->skelAnime.startFrame;
    }
}

void EnVm_SetupStun(EnVm* this) {
    Animation_Change(&this->skelAnime, &gBeamosAnim, -1.0f, Animation_GetLastFrame(&gBeamosAnim), 0.0f, ANIMMODE_ONCE,
                     0.0f);
    this->unk_260 = 0;
    this->timer = 180;
    this->unk_25E = this->unk_260;
    this->unk_21C = 2;
    this->beamScale.z = 0.0f;
    this->beamScale.y = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnVm_SetupAction(this, EnVm_Stun);
}

void EnVm_Stun(EnVm* this, PlayState* play) {
    if (this->timer == 0) {
        if (SkelAnime_Update(&this->skelAnime)) {
            this->unk_25E++;
            if (this->unk_25E == 3) {
                EnVm_SetupWait(this);
            } else if (this->unk_25E == 1) {
                Animation_Change(&this->skelAnime, &gBeamosAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gBeamosAnim),
                                 ANIMMODE_ONCE, 0.0f);
            } else {
                this->timer = 10;
                this->skelAnime.curFrame = 0.0f;
                this->skelAnime.playSpeed = 2.0f;
            }
        }
    } else {
        Math_SmoothStepToS(&this->beamRot.x, 0, 10, 0x5DC, 0);
        this->timer--;
        SkelAnime_Update(&this->skelAnime);
    }
}

void EnVm_SetupDie(EnVm* this) {
    Animation_Change(&this->skelAnime, &gBeamosAnim, -1.0f, Animation_GetLastFrame(&gBeamosAnim), 0.0f, ANIMMODE_ONCE,
                     0.0f);
    this->timer = 33;
    this->unk_25E = this->unk_260 = 0;
    this->unk_21C = 3;
    this->beamScale.z = 0.0f;
    this->beamScale.y = 0.0f;
    this->actor.shape.yOffset = -5000.0f;
    this->actor.world.pos.y += 5000.0f * this->actor.scale.y;
    this->actor.velocity.y = 8.0f;
    this->actor.gravity = -0.5f;
    this->actor.speed = Rand_ZeroOne() + 1.0f;
    this->actor.world.rot.y = Rand_CenteredFloat(65535.0f);
    EnVm_SetupAction(this, EnVm_Die);
}

void EnVm_Die(EnVm* this, PlayState* play) {
    this->beamRot.x += 0x5DC;
    this->headRotY += 0x9C4;
    Actor_MoveXZGravity(&this->actor);

    if (--this->timer == 0) {
        EnBom* bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, 0x10, this->actor.world.pos.x, this->actor.world.pos.y,
                                          this->actor.world.pos.z, 0, 0, 0x6FF, BOMB_BODY);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_10, false));
        Actor_Kill(&this->actor);
    }
}

void EnVm_CheckHealth(EnVm* this, PlayState* play) {
    EnBom* bomb;

    if (Actor_GetCollidedExplosive(play, &this->colliderCylinder.base) != NULL) {
        this->actor.colChkInfo.health--;
    } else {
        if (!(this->colliderQuad2.base.acFlags & AC_HIT) || this->unk_21C == 2) {
            return;
        }
        this->colliderQuad2.base.acFlags &= ~AC_HIT;
    }

    if (this->actor.colChkInfo.health != 0) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
        EnVm_SetupStun(this);
    } else {
        bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, 0x10, this->actor.world.pos.x,
                                   this->actor.world.pos.y + 20.0f, this->actor.world.pos.z, 0, 0, 0x601, BOMB_BODY);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        EnVm_SetupDie(this);
    }
}

void EnVm_Update(Actor* thisx, PlayState* play) {
    EnVm* this = (EnVm*)thisx;
    CollisionCheckContext* colChkCtx = &play->colChkCtx;

    if (this->actor.colChkInfo.health != 0) {
        EnVm_CheckHealth(this, play);
    }

    if (this->unk_260 == 4) {
        EffectSsDeadDs_SpawnStationary(play, &this->beamPos3, 20, -1, 255, 20);
        func_80033480(play, &this->beamPos3, 6.0f, 1, 120, 20, 1);
        Actor_PlaySfx(&this->actor, NA_SE_EN_BIMOS_LAZER_GND - SFX_FLAG);
    }

    this->actionFunc(this, play);
    this->beamTexScroll += 3 << 2;

    if (this->actor.colChkInfo.health != 0 && this->unk_21C != 2) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BIMOS_ROLL_HEAD - SFX_FLAG);
    }

    Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
    CollisionCheck_SetOC(play, colChkCtx, &this->colliderCylinder.base);

    if (this->actor.colorFilterTimer == 0 && this->actor.colChkInfo.health != 0) {
        CollisionCheck_SetAC(play, colChkCtx, &this->colliderCylinder.base);
    }

    CollisionCheck_SetAC(play, colChkCtx, &this->colliderQuad2.base);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += (6500.0f + this->actor.shape.yOffset) * this->actor.scale.y;
}

s32 EnVm_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnVm* this = (EnVm*)thisx;

    if (limbIndex == 2) {
        rot->x += this->beamRot.x;
        rot->y += this->headRotY;
    } else if (limbIndex == 10) {
        if (this->unk_21C == 3) {
            *dList = NULL;
        }
    }

    return false;
}

void EnVm_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnVm* this = (EnVm*)thisx;
    Vec3f sp80 = D_80B2EAF8;
    Vec3f sp74 = D_80B2EB04;
    Vec3f sp68 = D_80B2EB10;
    s32 pad;
    Vec3f posResult;
    CollisionPoly* poly;
    s32 bgId;
    f32 dist;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&D_80B2EB1C, &this->beamPos1);
        Matrix_MultVec3f(&D_80B2EB28, &this->beamPos2);

        if (this->unk_260 >= 3) {
            poly = NULL;
            sp80.z = (this->beamScale.z + 500.0f) * (this->actor.scale.y * 10000.0f);
            Matrix_MultVec3f(&sp80, &this->beamPos3);

            if (BgCheck_EntityLineTest1(&play->colCtx, &this->beamPos1, &this->beamPos3, &posResult, &poly, true, true,
                                        false, true, &bgId) == true) {
                this->beamScale.z = Math_Vec3f_DistXYZ(&this->beamPos1, &posResult) - 5.0f;
                this->unk_260 = 4;
                this->beamPos3 = posResult;
            }
            if (this->beamScale.z != 0.0f) {
                dist = 100.0f;
                if (this->actor.scale.y > 0.01f) {
                    dist = 70.0f;
                }
                sp74.z = sp68.z = Math_Vec3f_DistXYZ(&this->beamPos1, &this->beamPos3) * dist;
                Matrix_MultVec3f(&D_80B2EB64, &this->colliderQuad1.dim.quad[3]);
                Matrix_MultVec3f(&D_80B2EB70, &this->colliderQuad1.dim.quad[2]);
                Matrix_MultVec3f(&sp74, &this->colliderQuad1.dim.quad[1]);
                Matrix_MultVec3f(&sp68, &this->colliderQuad1.dim.quad[0]);
                Collider_SetQuadVertices(&this->colliderQuad1, &this->colliderQuad1.dim.quad[0],
                                         &this->colliderQuad1.dim.quad[1], &this->colliderQuad1.dim.quad[2],
                                         &this->colliderQuad1.dim.quad[3]);
            }
        }
        Matrix_MultVec3f(&D_80B2EB34, &this->colliderQuad2.dim.quad[1]);
        Matrix_MultVec3f(&D_80B2EB40, &this->colliderQuad2.dim.quad[0]);
        Matrix_MultVec3f(&D_80B2EB4C, &this->colliderQuad2.dim.quad[3]);
        Matrix_MultVec3f(&D_80B2EB58, &this->colliderQuad2.dim.quad[2]);
        Collider_SetQuadVertices(&this->colliderQuad2, &this->colliderQuad2.dim.quad[0],
                                 &this->colliderQuad2.dim.quad[1], &this->colliderQuad2.dim.quad[2],
                                 &this->colliderQuad2.dim.quad[3]);
    }
}

void EnVm_Draw(Actor* thisx, PlayState* play2) {
    EnVm* this = (EnVm*)thisx;
    PlayState* play = play2;
    Vec3f actorPos;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnVm_OverrideLimbDraw,
                      EnVm_PostLimbDraw, this);
    actorPos = this->actor.world.pos;
    func_80033C30(&actorPos, &D_80B2EB7C, 255, play);

    if (this->unk_260 >= 3) {
        Matrix_Translate(this->beamPos3.x, this->beamPos3.y + 10.0f, this->beamPos3.z, MTXMODE_NEW);
        Matrix_Scale(0.8f, 0.8f, 0.8f, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 168);
        Gfx_SetupDL_60NoCDXlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 0);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B2EB88[play->gameplayFrames % 8]));
        gSPDisplayList(POLY_XLU_DISP++, gEffEnemyDeathFlameDL);
        Matrix_RotateY(32767.0f, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B2EB88[(play->gameplayFrames + 4) % 8]));
        gSPDisplayList(POLY_XLU_DISP++, gEffEnemyDeathFlameDL);
    }
    gSPSegment(POLY_OPA_DISP++, 0x08, func_80094E78(play->state.gfxCtx, 0, this->beamTexScroll));
    Matrix_Translate(this->beamPos1.x, this->beamPos1.y, this->beamPos1.z, MTXMODE_NEW);
    Matrix_RotateZYX(this->beamRot.x, this->beamRot.y, this->beamRot.z, MTXMODE_APPLY);
    Matrix_Scale(this->beamScale.x * 0.1f, this->beamScale.x * 0.1f, this->beamScale.z * 0.0015f, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, gBeamosLaserDL);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x8a, En_Vm_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_INIT, EnVm_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_DESTROY, EnVm_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_UPDATE, EnVm_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_DRAW, EnVm_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_PROFILE, En_Vm_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x8a, 0x8b, 0x29f6e24e };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_VM_ORIGIN, kNativePortOrigin);
