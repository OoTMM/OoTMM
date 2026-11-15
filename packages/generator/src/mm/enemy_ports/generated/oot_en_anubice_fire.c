#include "oot_compat.h"
#include "types/en_anubice_fire.h"
#undef ACTOR_EN_ANUBICE_FIRE
#define ACTOR_EN_ANUBICE_FIRE 0x00e1
#undef OBJECT_ANUBICE
#define OBJECT_ANUBICE 0x10d6
#undef NA_SE_EN_ANUBIS_FIRE
#define NA_SE_EN_ANUBIS_FIRE 0x399c
#undef NA_SE_EN_ANUBIS_FIREBOMB
#define NA_SE_EN_ANUBIS_FIREBOMB 0x399d
#undef NA_SE_IT_SHIELD_REFLECT_SW
#define NA_SE_IT_SHIELD_REFLECT_SW 0x1808
#undef DMG_DEKU_STICK
#define DMG_DEKU_STICK (1 << 1)
#define gDust4Tex (*((u64 (*)[1])0x04057ee0))
#define gDust5Tex (*((u64 (*)[1])0x040582e0))
#define gDust6Tex (*((u64 (*)[1])0x040586e0))
#define gDust7Tex (*((u64 (*)[1])0x04058ae0))
#define gDust8Tex (*((u64 (*)[1])0x04058ee0))
#define gAnubiceFireAttackDL (*((Gfx (*)[1])0x06003510))

#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_UpdatePos
#define Actor_UpdatePos ((void (*)(Actor* actor))0x800b69ac)
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
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef EffectSsBomb2_SpawnLayered
#define EffectSsBomb2_SpawnLayered ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep))0x800b1970)
#undef EffectSsKiraKira_SpawnDispersed
extern void FOREIGN_ENEMY_API_SPARKLE_DISPERSED(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                                                Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);
#define EffectSsKiraKira_SpawnDispersed FOREIGN_ENEMY_API_SPARKLE_DISPERSED
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x801820a0)
#undef Matrix_RotateX
#define Matrix_RotateX FOREIGN_ENEMY_API_MATRIX_ROTATE_XF
#undef Matrix_RotateY
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZ
#define Matrix_RotateZ FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Player_HasMirrorShieldEquipped
extern s32 FOREIGN_ENEMY_API_OOT_MIRROR_SHIELD(PlayState* play);
#define Player_HasMirrorShieldEquipped FOREIGN_ENEMY_API_OOT_MIRROR_SHIELD
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)

#define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED
#define ANUBIS_FIRE_MM_BURN_EFFECT 9

void EnAnubiceFire_Init(Actor* thisx, PlayState* play);
void EnAnubiceFire_Destroy(Actor* thisx, PlayState* play);
void EnAnubiceFire_Update(Actor* thisx, PlayState* play);
void EnAnubiceFire_Draw(Actor* thisx, PlayState* play);

void func_809B26EC(EnAnubiceFire* this, PlayState* play);
void func_809B27D8(EnAnubiceFire* this, PlayState* play);
void func_809B2B48(EnAnubiceFire* this, PlayState* play);

ActorProfile En_Anubice_Fire_Profile = {
    0xce1,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubiceFire),
    EnAnubiceFire_Init,
    EnAnubiceFire_Destroy,
    EnAnubiceFire_Update,
    EnAnubiceFire_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, ANUBIS_FIRE_MM_BURN_EFFECT, 0x04 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 0, 0, 0, { 0, 0, 0 } },
};

void EnAnubiceFire_Init(Actor* thisx, PlayState* play) {
    EnAnubiceFire* this = (EnAnubiceFire*)thisx;
    s32 i;

    Collider_InitCylinder(play, &this->colliderCylinder);
    Collider_SetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);

    this->unk_15A = 30;
    this->unk_154 = 2.0f;
    this->scale = 0.0f;

    for (i = 0; i < 6; i++) {
        this->unk_160[i] = this->actor.world.pos;
    }

    this->unk_15E = 0;
    this->actionFunc = func_809B26EC;
}

void EnAnubiceFire_Destroy(Actor* thisx, PlayState* play) {
    EnAnubiceFire* this = (EnAnubiceFire*)thisx;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
}

void func_809B26EC(EnAnubiceFire* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };

    Matrix_Push();
    Matrix_RotateY(BINANG_TO_RAD_ALT(this->actor.world.rot.y), MTXMODE_NEW);
    Matrix_RotateX(BINANG_TO_RAD_ALT(this->actor.world.rot.x), MTXMODE_APPLY);
    velocity.z = 15.0f;
    Matrix_MultVec3f(&velocity, &this->actor.velocity);
    Matrix_Pop();

    this->actionFunc = func_809B27D8;
    this->actor.world.rot.x = this->actor.world.rot.y = this->actor.world.rot.z = 0;
}

void func_809B27D8(EnAnubiceFire* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Color_RGBA8 primColor = { 255, 255, 0, 255 };
    Color_RGBA8 envColor = { 255, 0, 0, 255 };
    Vec3f sp84 = { 0.0f, 0.0f, 0.0f };
    Vec3f sp78 = { 0.0f, 0.0f, 0.0f };

    this->actor.world.rot.z += 5000;
    if (this->unk_15A == 0) {
        this->unk_154 = 0.0f;
    }

    Math_ApproachF(&this->scale, this->unk_154, 0.2f, 0.4f);
    if ((this->unk_15A == 0) && (this->scale < 0.1f)) {
        Actor_Kill(&this->actor);
    } else if ((this->actor.params == 0) && (this->colliderCylinder.base.atFlags & AT_BOUNCED)) {
        if (Player_HasMirrorShieldEquipped(play)) {
            Actor_PlaySfx(&this->actor, NA_SE_IT_SHIELD_REFLECT_SW);
            this->colliderCylinder.base.atFlags &= ~(AT_HIT | AT_BOUNCED | AT_TYPE_ENEMY);
            this->colliderCylinder.base.atFlags |= AT_TYPE_PLAYER;
            this->colliderCylinder.elem.atDmgInfo.dmgFlags = DMG_DEKU_STICK;
            this->unk_15A = 30;
            this->actor.params = 1;
            this->actor.velocity.x *= -1.0f;
            this->actor.velocity.y *= -0.5f;
            this->actor.velocity.z *= -1.0f;
        } else {
            this->unk_15A = 0;
            EffectSsBomb2_SpawnLayered(play, &this->actor.world.pos, &sp78, &sp84, 10, 5);
            this->actor.velocity.x = this->actor.velocity.y = this->actor.velocity.z = 0.0f;
            Actor_PlaySfx(&this->actor, NA_SE_EN_ANUBIS_FIREBOMB);
            this->actionFunc = func_809B2B48;
        }
    } else if (!(this->scale < .4f)) {
        f32 scale = 1000.0f;
        f32 life = 10.0f;
        s32 i;

        for (i = 0; i < 10; i++) {
            pos.x = this->actor.world.pos.x + (Rand_ZeroOne() - 0.5f) * (this->scale * 20.0f);
            pos.y = this->actor.world.pos.y + (Rand_ZeroOne() - 0.5f) * (this->scale * 20.0f);
            pos.z = this->actor.world.pos.z;
            EffectSsKiraKira_SpawnDispersed(play, &pos, &velocity, &accel, &primColor, &envColor, scale, life);
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_ANUBIS_FIRE - SFX_FLAG);
    }
}

void func_809B2B48(EnAnubiceFire* this, PlayState* play) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Color_RGBA8 primColor = { 255, 255, 0, 255 };
    Color_RGBA8 envColor = { 255, 0, 0, 255 };
    s32 i;

    if (this->unk_15C == 0) {
        for (i = 0; i < 20; i++) {
            pos.x = this->actor.world.pos.x;
            pos.y = this->actor.world.pos.y;
            pos.z = this->actor.world.pos.z;
            accel.x = Rand_CenteredFloat(8.0f);
            accel.y = Rand_CenteredFloat(2.0f);
            accel.z = Rand_CenteredFloat(8.0f);
            EffectSsKiraKira_SpawnDispersed(play, &pos, &velocity, &accel, &primColor, &envColor, 2000, 10);
        }

        this->unk_15C = 2;
        this->unk_15E++;
        if (this->unk_15E >= 6) {
            Actor_Kill(&this->actor);
        }
    }
}

void EnAnubiceFire_Update(Actor* thisx, PlayState* play) {
    EnAnubiceFire* this = (EnAnubiceFire*)thisx;
    s32 i;

    Actor_SetScale(&this->actor, this->scale);
    this->actionFunc(this, play);
    Actor_UpdatePos(&this->actor);
    this->unk_160[0] = this->actor.world.pos;

    for (i = 4; i >= 0; i--) {
        this->unk_160[i + 1] = this->unk_160[i];
    }

    if (this->unk_15A != 0) {
        this->unk_15A--;
    }

    if (this->unk_15C != 0) {
        this->unk_15C--;
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 5.0f, 10.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
    if (!(this->scale < 0.6f || this->actionFunc == func_809B2B48)) {
        this->colliderCylinder.dim.radius = this->scale * 15.0f + 5.0f;
        this->colliderCylinder.dim.height = this->scale * 15.0f + 5.0f;
        this->colliderCylinder.dim.yShift = this->scale * -0.75f + -15.0f;

        if (this->unk_15A != 0) {
            Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderCylinder.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
        }

        if (BgCheck_SphVsFirstPoly(&play->colCtx, &this->actor.world.pos, 30.0f)) {
            this->actor.velocity.x = this->actor.velocity.y = this->actor.velocity.z = 0.0f;
            Actor_PlaySfx(&this->actor, NA_SE_EN_ANUBIS_FIREBOMB);
            this->actionFunc = func_809B2B48;
        }
    }
}

void EnAnubiceFire_Draw(Actor* thisx, PlayState* play) {
    static void* D_809B3270[] = {
        gDust4Tex, gDust5Tex, gDust6Tex, gDust7Tex, gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex,
    };
    EnAnubiceFire* this = (EnAnubiceFire*)thisx;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    gDPPipeSync(POLY_XLU_DISP++);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809B3270[0]));

    Matrix_Push();
    for (i = this->unk_15E; i < 6; ++i) {
        f32 scale = this->actor.scale.x - (i * 0.2f);

        if (scale < 0.0f) {
            scale = 0.0f;
        }

        if (scale >= 0.1f) {
            Matrix_Translate(this->unk_160[i].x, this->unk_160[i].y, this->unk_160[i].z, MTXMODE_NEW);
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_RotateZ(this->actor.world.rot.z + i * 1000.0f, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);

            gSPDisplayList(POLY_XLU_DISP++, gAnubiceFireAttackDL);
        }

        if (this->scale < 0.1f) {
            break;
        }
    }
    Matrix_Pop();

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0xe1, En_Anubice_Fire_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_INIT, EnAnubiceFire_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_DESTROY, EnAnubiceFire_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_UPDATE, EnAnubiceFire_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_DRAW, EnAnubiceFire_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_PROFILE, En_Anubice_Fire_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xe1, 0xd6, 0x5c758d8c };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ANUBICE_FIRE_ORIGIN, kNativePortOrigin);
