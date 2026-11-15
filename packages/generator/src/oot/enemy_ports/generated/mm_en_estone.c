#if defined(GAME_MM)
#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/global.h>
#include <combo/sfx.h>
#include "types/en_estone.h"
#define KREG(index) 0
#define PLAYER_STATE2_80 PLAYER_STATE2_MM_80
#define UPDBGCHECKINFO_FLAG_1 UPDBGCHECKINFO_FLAG_MM_1
#define UPDBGCHECKINFO_FLAG_4 UPDBGCHECKINFO_FLAG_MM_4
#define UPDBGCHECKINFO_FLAG_8 UPDBGCHECKINFO_FLAG_MM_8
#define UPDBGCHECKINFO_FLAG_10 UPDBGCHECKINFO_FLAG_MM_10
#define gameplay_keep_DL_06AB30 (*((Gfx (*)[1])0x0406ab30))
#define gEyegoreStoneDL (*((Gfx (*)[1])0x06010240))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))0x800b670c)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))0x800b8ec8)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 countMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800bbdac)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x800b78b8)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e11ec)
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e1374)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))0x800e2740)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))0x800e2558)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x800ff50c)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(GraphicsContext* gfxCtx))0x80181a40)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_RotateXFApply
#define Matrix_RotateXFApply ((void (*)(f32 x))0x801807b8)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))0x80180478)
#undef Matrix_RotateYF
#define Matrix_RotateYF ((void (*)(f32 y, MatrixMode mode))0x80180b48)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))0x801809ac)
#undef Matrix_RotateZF
#define Matrix_RotateZF ((void (*)(f32 z, MatrixMode mode))0x80180e90)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))0x80180cf8)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))0x8018039c)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))0x8018029c)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x801795c0)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80179594)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#else
#include "mm_native_compat.h"
#define gameplay_keep_DL_06AB30 (*((Gfx (*)[1])0x0406ab30))
#define gEyegoreStoneDL (*((Gfx (*)[1])0x06010240))

#include "types/en_estone.h"
#undef ACTOR_EN_ESTONE
#define ACTOR_EN_ESTONE 0x18e
#undef OBJECT_EG
#define OBJECT_EG 0x118d
#undef NA_SE_EV_WALL_BROKEN
#define NA_SE_EV_WALL_BROKEN 0x2810
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateXFApply
#define Matrix_RotateXFApply ((void (*)(f32 x))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_APPLY)
#undef Matrix_RotateZF
#define Matrix_RotateZF ((void (*)(f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(struct PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 countMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800260e8u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x800aa6ecu)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x800aa724u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#endif

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

void EnEstone_Init(Actor* thisx, PlayState* play);
void EnEstone_Destroy(Actor* thisx, PlayState* play);
void EnEstone_Update(Actor* thisx, PlayState* play2);
void EnEstone_Draw(Actor* thisx, PlayState* play2);

void EnEstone_Active(EnEstone* this, PlayState* play);
void EnEstone_Inactive(EnEstone* this, PlayState* play);
void EnEstone_SpawnEffect(EnEstone* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 lifetime);
void EnEstone_UpdateEffects(EnEstone* this, PlayState* play);
void EnEstone_DrawEffects(EnEstone* this, PlayState* play);

ActorProfile En_Estone_Profile = {
#if defined(GAME_MM)
      0x18e,
#else
      0xd8e,
#endif
      ACTORCAT_PROP,
    FLAGS,
#if defined(GAME_MM)
      0x18d,
#else
      0x118d,
#endif
      sizeof(EnEstone),
    EnEstone_Init,
    EnEstone_Destroy,
    EnEstone_Update,
    EnEstone_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 30, 30, -10, { 0, 0, 0 } },
};

void EnEstone_Init(Actor* thisx, PlayState* play) {
    EnEstone* this = (EnEstone*)thisx;
    Vec3f accel;
    Vec3f velocity;
    f32 scale;
    s32 i;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 15.0f);
    this->rotVel.x = this->rotVel.y = this->rotVel.z = Rand_CenteredFloat(1.0f) * 20.0f;
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.world.rot.y += TRUNCF_BINANG(Rand_CenteredFloat(0x2710));
    this->actor.shape.rot.y = this->actor.world.rot.y;

    if (this->actor.params == ENESTONE_TYPE_LARGE) {
        this->actor.speed = Rand_ZeroFloat(5.0f) + 2.0f;
        this->scale = (Rand_ZeroFloat(1.0f) * 0.005f) + 0.005f;
        this->actor.velocity.y = Rand_ZeroFloat(10.0f) + 15.0f;
        this->actor.gravity = -2.0f;
    } else {
        this->actor.speed = Rand_ZeroFloat(3.0f) + 1.0f;
        this->scale = (Rand_ZeroFloat(1.0f) * 0.003f) + 0.003f;
        this->actor.velocity.y = Rand_ZeroFloat(5.0f) + 7.0f;
        this->actor.gravity = -1.0f;
    }

    this->collider.dim.radius = this->scale * 3000.0f;
    this->collider.dim.height = this->scale * 3000.0f;
    this->collider.dim.yShift = this->scale * -1300.0f;

    for (i = 0; i < 7; i++) {
        accel.x = 0.5f * (Rand_ZeroOne() - 0.5f);
        accel.y = -1.0f;
        accel.z = 0.5f * (Rand_ZeroOne() - 0.5f);

        velocity.x = 2.0f * (Rand_ZeroOne() - 0.5f);
        velocity.y = 8.0f + (Rand_ZeroOne() * 10.0f);
        velocity.z = 2.0f * (Rand_ZeroOne() - 0.5f);

        scale = (Rand_ZeroFloat(1.0f) * 0.001f) + 0.001f;

        EnEstone_SpawnEffect(this, &this->actor.world.pos, &velocity, &accel, scale, 70);
    }
    this->timer = 5;
#if defined(GAME_OOT)
    this->standaloneLifetime = 240;
#endif
    this->inactive = false;
    Actor_SetScale(&this->actor, 1.0f);
    this->actionFunc = EnEstone_Active;
}

void EnEstone_Destroy(Actor* thisx, PlayState* play) {
    EnEstone* this = (EnEstone*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnEstone_Active(EnEstone* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f accel;
    Vec3f velocity;
    s32 i;

    if ((this->actor.floorHeight == -10000.0f) || ((player->actor.world.pos.y - 200.0f) > this->actor.world.pos.y)) {
        Actor_Kill(&this->actor);
        return;
    }
    if (((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && (this->actor.velocity.y < 0.0f)) ||
        (this->collider.base.atFlags & AT_BOUNCED)) {
        this->collider.base.atFlags &= ~AT_BOUNCED;
        if (this->actor.params == ENESTONE_TYPE_LARGE) {
            for (i = 0; i < 2; i++) {
                accel.x = 2.0f * (Rand_ZeroOne() - 0.5f);
                accel.y = -1.0f;
                accel.z = 2.0f * (Rand_ZeroOne() - 0.5f);
                velocity.x = 3.0f * (Rand_ZeroOne() - 0.5f);
                velocity.y = 5.0f + (Rand_ZeroOne() * 10.0f);
                velocity.z = 3.0f * (Rand_ZeroOne() - 0.5f);
                EnEstone_SpawnEffect(this, &this->actor.world.pos, &velocity, &accel,
                                     0.003f + (0.002f * Rand_ZeroFloat(1.0f)), 20);
            }
        }
        Math_Vec3f_Copy(&velocity, &this->actor.world.pos);
        velocity.y = this->actor.floorHeight;
        Actor_SpawnFloorDustRing(play, &this->actor, &velocity, 0.0f, 10, 6.0f, 50, 30, true);
        this->actor.velocity.y = this->actor.gravity = 0.0f;
        this->actor.speed *= 0.3f;
        this->actor.shape.shadowScale = 0.0f;
        this->inactive = true;
        this->timer = 50;
        Actor_PlaySfx(&this->actor, NA_SE_EV_WALL_BROKEN);
        this->actionFunc = EnEstone_Inactive;
    } else {
        this->actor.shape.shadowScale = 4500.0f * this->scale;
        Actor_SetScale(&this->actor, this->scale);
    }
}

void EnEstone_Inactive(EnEstone* this, PlayState* play) {
    if (this->timer == 0) {
        Actor_Kill(&this->actor);
    }
}

void EnEstone_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnEstone* this = (EnEstone*)thisx;

#if defined(GAME_OOT)
    if (this->standaloneLifetime && --this->standaloneLifetime == 0) {
        Actor_Kill(thisx);
        return;
    }
#endif
    DECR(this->timer);

    this->actionFunc(this, play);

    this->rot.x -= this->rotVel.x;
    this->rot.y -= this->rotVel.y;
    this->rot.z -= this->rotVel.z;

    Actor_MoveWithGravity(&this->actor);
    if ((this->timer == 0) && !this->inactive) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 50.0f, 50.0f, 100.0f,
                                UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    }
    if (!this->inactive) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    EnEstone_UpdateEffects(this, play);
}

void EnEstone_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnEstone* this = (EnEstone*)thisx;

    if (this->inactive != true) {
        Matrix_Push();

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_RotateXFApply(DEG_TO_RAD(this->rot.x));
        Matrix_RotateYF(DEG_TO_RAD(this->rot.y), MTXMODE_APPLY);
        Matrix_RotateZF(DEG_TO_RAD(this->rot.z), MTXMODE_APPLY);
        Matrix_Scale(this->scale, this->scale, this->scale, MTXMODE_APPLY);
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gEyegoreStoneDL);

        CLOSE_DISPS();

        Matrix_Pop();
    }
    EnEstone_DrawEffects(this, play);
}

void EnEstone_SpawnEffect(EnEstone* this, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 scale, s16 lifetime) {
    EnEstoneEffect* effect = this->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (!effect->isActive) {
            effect->isActive = true;
            effect->pos = *pos;
            effect->velocity = *velocity;
            effect->accel = *accel;
            effect->scale = scale;
            effect->timer = lifetime;
            effect->rot.x = Rand_CenteredFloat(0x7530);
            effect->rot.y = Rand_CenteredFloat(0x7530);
            effect->rot.z = Rand_CenteredFloat(0x7530);
            break;
        }
    }
}

void EnEstone_UpdateEffects(EnEstone* this, PlayState* play) {
    EnEstoneEffect* effect = this->effects;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            effect->rot.x += 0x100;
            effect->rot.z += 0x130;

            effect->pos.x += effect->velocity.x;
            effect->pos.y += effect->velocity.y;
            effect->pos.z += effect->velocity.z;

            effect->velocity.y += effect->accel.y;
            if ((effect->pos.y < this->actor.floorHeight) && (effect->velocity.y < 0.0f)) {
                effect->timer = 0;
                Actor_SpawnFloorDustRing(play, &this->actor, &effect->pos, 0.0f, 5, 3.0f, 30, 15, true);
            }
            if (effect->timer != 0) {
                effect->timer--;
            } else {
                effect->isActive = false;
            }
        }
    }
}

void EnEstone_DrawEffects(EnEstone* this, PlayState* play) {
    EnEstoneEffect* effect = this->effects;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    s16 i;

    OPEN_DISPS(gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(this->effects); i++, effect++) {
        if (effect->isActive) {
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateXS(effect->rot.x, MTXMODE_APPLY);
            Matrix_RotateYS(effect->rot.y, MTXMODE_APPLY);
            Matrix_RotateZS(effect->rot.z, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
            Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx);
            gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
            gSPDisplayList(POLY_OPA_DISP++, gameplay_keep_DL_06AB30);
        }
    }

    CLOSE_DISPS();
}

#if defined(GAME_MM)
OVL_INFO_ACTOR(0x18e, En_Estone_Profile);
EXPORT_SYMBOL(ENEMY_NATIVE_EYEGORE_STONE_COMPACT_MM_V1, EnEstone_Init);
#else
OVL_INFO_NATIVE_ENEMY(0x18e, En_Estone_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_ESTONE, En_Estone_Profile, EnEstone_Init, EnEstone_Destroy,
                     EnEstone_Update, EnEstone_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x18eu, 0x18du, 0x9f9645dcu };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_ESTONE_ORIGIN, generatedNativeOrigin);
#endif
