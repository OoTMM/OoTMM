#include "oot_compat.h"
#include "types/en_fd_fire.h"
#undef ACTOR_EN_FD_FIRE
#define ACTOR_EN_FD_FIRE 0x00a3
#undef OBJECT_GAMEPLAY_DANGEON_KEEP
#define OBJECT_GAMEPLAY_DANGEON_KEEP 0x1003
#define gEffFire1DL (*((Gfx (*)[1])0x04052a10))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_UpdatePos
#define Actor_UpdatePos ((void (*)(Actor* actor))0x800b69ac)
#undef Actor_UpdateVelocityXZGravity
#define Actor_UpdateVelocityXZGravity ((void (*)(Actor* actor))0x800b6a10)
#undef Camera_GetCamDirYaw
#define Camera_GetCamDirYaw ((s16 (*)(Camera* camera))0x800dfcdc)
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
#undef CollisionCheck_SetInfo2
#define CollisionCheck_SetInfo2 ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit2* init))0x800e755c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TwoTexScroll
#define Gfx_TwoTexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2))0x8012cbd0)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXYZ
#define Math_Vec3f_DistXYZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff884)
#undef Math_Vec3f_DistXZ
#define Math_Vec3f_DistXZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff92c)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_RotateY
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZ
#define Matrix_RotateZ FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 f))0x80179594)
#undef cosf
#define cosf ((f32 (*)(f32 x))0x80091f40)
#undef sinf
#define sinf ((f32 (*)(f32 x))0x80088350)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnFdFire_Init(Actor* thisx, PlayState* play);
void EnFdFire_Destroy(Actor* thisx, PlayState* play);
void EnFdFire_Update(Actor* thisx, PlayState* play);
void EnFdFire_Draw(Actor* thisx, PlayState* play);
void EnFdFire_Disappear(EnFdFire* this, PlayState* play);
void func_80A0E70C(EnFdFire* this, PlayState* play);
void EnFdFire_DanceTowardsPlayer(EnFdFire* this, PlayState* play);
void EnFdFire_WaitToDie(EnFdFire* this, PlayState* play);

ActorProfile En_Fd_Fire_Profile = {
    0xca3,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnFdFire),
    EnFdFire_Init,
    EnFdFire_Destroy,
    EnFdFire_Update,
    EnFdFire_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, 0x08 },
        { 0x0D840008, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 12, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
} };

void EnFdFire_UpdatePos(EnFdFire* this, Vec3f* targetPos) {
    f32 dist;
    f32 xDiff = targetPos->x - this->actor.world.pos.x;
    f32 yDiff = targetPos->y - this->actor.world.pos.y;
    f32 zDiff = targetPos->z - this->actor.world.pos.z;

    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (fabsf(dist) > fabsf(this->actor.speed)) {
        this->actor.velocity.x = (xDiff / dist) * this->actor.speed;
        this->actor.velocity.z = (zDiff / dist) * this->actor.speed;
    } else {
        this->actor.velocity.x = 0.0f;
        this->actor.velocity.z = 0.0f;
    }

    this->actor.velocity.y += this->actor.gravity;
    if (!(this->actor.minVelocityY <= this->actor.velocity.y)) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

s32 EnFdFire_CheckCollider(EnFdFire* this, PlayState* play) {
    if (this->actionFunc == EnFdFire_Disappear) {
        return false;
    }

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        return true;
    }

    if (this->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        this->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        return true;
    }
    return false;
}

void EnFdFire_Init(Actor* thisx, PlayState* play) {
    EnFdFire* this = (EnFdFire*)thisx;
    Player* player = GET_PLAYER(play);

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInit);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.gravity = -0.6f;
    this->actor.speed = 5.0f;
    this->actor.velocity.y = 12.0f;
    this->spawnRadius = Math_Vec3f_DistXYZ(&this->actor.world.pos, &player->actor.world.pos);
    this->scale = 3.0f;
    this->tile2Y = (s16)Rand_ZeroFloat(5.0f) - 25;
    this->actionFunc = func_80A0E70C;
}

void EnFdFire_Destroy(Actor* thisx, PlayState* play) {
    EnFdFire* this = (EnFdFire*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80A0E70C(EnFdFire* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f targetPos = this->actor.parent->world.pos;

    targetPos.x += this->spawnRadius * Math_SinS(this->actor.world.rot.y);
    targetPos.z += this->spawnRadius * Math_CosS(this->actor.world.rot.y);
    EnFdFire_UpdatePos(this, &targetPos);
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (!(this->actor.velocity.y > 0.0f))) {
        this->actor.velocity = velocity;
        this->actor.speed = 0.0f;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        if (PARAMS_GET_NOSHIFT(this->actor.params, 15, 1)) {
            this->deathTimer = 200;
            this->actionFunc = EnFdFire_DanceTowardsPlayer;
        } else {
            this->deathTimer = 300;
            this->actionFunc = EnFdFire_WaitToDie;
        }
    }
}

void EnFdFire_WaitToDie(EnFdFire* this, PlayState* play) {
    if (DECR(this->deathTimer) == 0) {
        this->actionFunc = EnFdFire_Disappear;
    }
}

void EnFdFire_DanceTowardsPlayer(EnFdFire* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 angles[] = {
        0.0f, 210.0f, 60.0f, 270.0f, 120.0f, 330.0f, 180.0f, 30.0f, 240.0f, 90.0f, 300.0f, 150.0f,
    };
    Vec3f pos;
    s16 idx;

    idx = ((play->state.frameCount / 10) + PARAMS_GET_S(this->actor.params, 0, 15)) % ARRAY_COUNT(angles);
    pos = player->actor.world.pos;
    pos.x += 120.0f * sinf(angles[idx]);
    pos.z += 120.0f * cosf(angles[idx]);

    if (DECR(this->deathTimer) == 0) {
        this->actionFunc = EnFdFire_Disappear;
    } else {
        Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &pos), 8, 0xFA0, 1);
        Math_SmoothStepToF(&this->actor.speed, 0.0f, 0.4f, 1.0f, 0.0f);
        if (this->actor.speed < 0.1f) {
            this->actor.speed = 5.0f;
        }
        Actor_UpdateVelocityXZGravity(&this->actor);
    }
}

void EnFdFire_Disappear(EnFdFire* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 0.6f, 9.0f, 0.0f);
    Actor_UpdateVelocityXZGravity(&this->actor);
    Math_SmoothStepToF(&this->scale, 0.0f, 0.3f, 0.1f, 0.0f);
    this->actor.shape.shadowScale = 20.0f;
    this->actor.shape.shadowScale *= (this->scale / 3.0f);
    if (!(this->scale > 0.01f)) {
        Actor_Kill(&this->actor);
    }
}

void EnFdFire_Update(Actor* thisx, PlayState* play) {
    EnFdFire* this = (EnFdFire*)thisx;

    if (this->actionFunc != EnFdFire_Disappear) {
        if ((this->actor.parent->update == NULL) || EnFdFire_CheckCollider(this, play)) {
            this->actionFunc = EnFdFire_Disappear;
        }
    }

    Actor_UpdatePos(&this->actor);
    this->actionFunc(this, play);
    Actor_UpdateBgCheckInfo(play, &this->actor, 12.0f, 10.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (this->actionFunc != EnFdFire_Disappear) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnFdFire_Draw(Actor* thisx, PlayState* play) {
    Color_RGBA8 primColors[] = {
        { 255, 255, 0, 255 },
        { 255, 255, 255, 255 },
    };
    Color_RGBA8 envColors[] = {
        { 255, 10, 0, 255 },
        { 0, 10, 255, 255 },
    };
    EnFdFire* this = (EnFdFire*)thisx;
    Vec3f scale = { 0.0f, 0.0f, 0.0f };
    s16 sp8E;
    f32 sp88;
    f32 sp84;
    f32 sp80;

    OPEN_DISPS(play->state.gfxCtx);

    Matrix_Translate(thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, MTXMODE_NEW);
    sp8E = Math_Vec3f_Yaw(&scale, &thisx->velocity) - Camera_GetCamDirYaw(GET_ACTIVE_CAM(play));
    sp84 = fabsf(Math_CosS(sp8E));
    sp88 = Math_SinS(sp8E);
    sp80 = Math_Vec3f_DistXZ(&scale, &thisx->velocity) / 1.5f;
    Matrix_RotateY(BINANG_TO_RAD((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) + 0x8000)), MTXMODE_APPLY);
    Matrix_RotateZ(DEG_TO_RAD((sp88 * -10.0f) * sp80), MTXMODE_APPLY);
    scale.x = scale.y = scale.z = this->scale * 0.001f;
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    sp84 = sp80 * ((0.01f * -15.0f) * sp84) + 1.0f;
    if (sp84 < 0.1f) {
        sp84 = 0.1f;
    }
    Matrix_Scale(1.0f, sp84, 1.0f / sp84, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x8,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                play->state.frameCount * this->tile2Y, 0x20, 0x80));
    gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, primColors[PARAMS_GET_S(thisx->params, 15, 1)].r,
                    primColors[PARAMS_GET_S(thisx->params, 15, 1)].g, primColors[PARAMS_GET_S(thisx->params, 15, 1)].b,
                    primColors[PARAMS_GET_S(thisx->params, 15, 1)].a);
    gDPSetEnvColor(POLY_XLU_DISP++, envColors[PARAMS_GET_S(thisx->params, 15, 1)].r,
                   envColors[PARAMS_GET_S(thisx->params, 15, 1)].g, envColors[PARAMS_GET_S(thisx->params, 15, 1)].b,
                   envColors[PARAMS_GET_S(thisx->params, 15, 1)].a);
    gDPPipeSync(POLY_XLU_DISP++);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0xa3, En_Fd_Fire_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_INIT, EnFdFire_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_DESTROY, EnFdFire_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_UPDATE, EnFdFire_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_DRAW, EnFdFire_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_PROFILE, En_Fd_Fire_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xa3, 0x3, 0xccab2cf6 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_FIRE_ORIGIN, kNativePortOrigin);
