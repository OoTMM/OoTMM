#include <combo/enemy_actor_layout.h>
#if defined(GAME_MM)
#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/global.h>
#include <combo/sfx.h>
#include <combo/mm/regs.h>
#include "types/en_tanron1.h"
#undef Audio_PlaySfx_AtPos
#define Audio_PlaySfx_AtPos ((void (*)(Vec3f * pos, u16 sfxId))0x8019f1c0)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32(*)(PlayState * play, CollisionContext * colCtx, f32 x, f32 z, f32 * outWaterSurface, WaterBox * *outWaterBox))0x800ca1ac)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext * gfxCtx))0x8012c28c)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32 * pValue, f32 target, f32 scale, f32 maxStep))0x800ffdf8)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16 * pValue, s16 target, s16 scale, s16 maxStep))0x800fffd8)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32 * pValue, f32 scale, f32 maxStep))0x800ffe68)
#undef Math_Atan2S
#define Math_Atan2S ((s16(*)(f32 y, f32 x))0x8017fee4)
#undef Math_SinS
#define Math_SinS ((f32(*)(s16 angle))0x800fed84)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f * dest))0x80181c10)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))0x80180478)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))0x801809ac)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))0x8018039c)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))0x8018029c)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32(*)(f32 scale))0x801795c0)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32(*)(f32 scale))0x80179594)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32(*)(void))0x80086fdc)
#undef SkinMatrix_Vec3fMtxFMultXYZW
#define SkinMatrix_Vec3fMtxFMultXYZW ((void (*)(MtxF * mf, Vec3f * src, Vec3f * xyzDest, f32 * wDest))0x80138ba0)
#else
#include "mm_native_compat.h"

#include "types/en_tanron1.h"
#undef ACTOR_EN_TANRON1
#define ACTOR_EN_TANRON1 0x23d
#undef GAMEPLAY_KEEP
#define GAMEPLAY_KEEP 0x1001
#undef NA_SE_EN_MB_MOTH_FLY
#define NA_SE_EN_MB_MOTH_FLY 0x399b
#undef NA_SE_EN_MB_MOTH_DEAD
#define NA_SE_EN_MB_MOTH_DEAD 0x399c
#undef Math_Atan2S
#define Math_Atan2S ((s16(*)(f32 y, f32 x))(void*)FOREIGN_ENEMY_API_ATAN2S)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateXS
#define Matrix_RotateXS ((void (*)(s16 x, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XS)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f * dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32(*)(f32 scale))0x80026d64u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32(*)(void))0x800cdcccu)
#undef Math_SinS
#define Math_SinS ((f32(*)(s16 angle))0x800636c4u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32(*)(f32 scale))0x80026d90u)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16 * pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32 * pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32 * pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32(*)(struct PlayState * play, CollisionContext * colCtx, f32 x, f32 z, f32 * outWaterSurface, WaterBox * *outWaterBox))0x80034908u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext * gfxCtx))0x8007e298u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx * (*)(struct GraphicsContext * gfxCtx))0x800ab900u)
#endif

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED)

void EnTanron1_Init(Actor* thisx, PlayState* play);
void EnTanron1_Destroy(Actor* thisx, PlayState* play);
void EnTanron1_Update(Actor* thisx, PlayState* play);
void EnTanron1_Draw(Actor* thisx, PlayState* play);

void func_80BB5318(EnTanron1* this, PlayState* play);
void func_80BB5AAC(EnTanron1* this, PlayState* play);

ActorProfile En_Tanron1_Profile = {
#if defined(GAME_MM)
    0x23d,
#else
    0xe3d,
#endif
    ACTORCAT_ENEMY,
    FLAGS,
#if defined(GAME_MM)
    1,
#else
    GAMEPLAY_KEEP,
#endif
    sizeof(EnTanron1),
    EnTanron1_Init,
    EnTanron1_Destroy,
    EnTanron1_Update,
    EnTanron1_Draw,
};

#define ENEMY_MOTH_STANDALONE 0x7f00
#if defined(GAME_MM)
#define ENEMY_MOTH_TORCH_ID 0x39
void comboEnemyNativePlacementBlock(s32);
#define EnemyMothBurning(player) ((player)->unk_B28)
#define EnemyMothMelee(player)   ((player)->unk_ADC)
#define EnemyMothDeku(player)    ((player)->transformation == MM_PLAYER_FORM_DEKU)
#define EnemyMothKnockback       ((void (*)(PlayState*, Actor*, f32, s16, f32, s32, u32))0x800b8d10)
#define EnemyMothRaycast         ((f32(*)(CollisionContext*, CollisionPoly**, Vec3f*))0x800c3fa0)
#define EnemyMothAudio           Audio_PlaySfx_AtPos
#define EnemyMothStopAudio       ((void (*)(Vec3f*, u16))0x801a7328)
#define EnemyMothSoundThreshold  (KREG(39) + 20)
#define EnemyMothAlloc           ((void* (*)(u32))0x80102c60)
#define EnemyMothFree            ((void (*)(void*))0x80102ce0)
#else
#define ENEMY_MOTH_TORCH_ID      0x5e
#define EnemyMothBurning(player) ((player)->unk_860)
#define EnemyMothMelee(player)   ((player)->meleeWeaponState)
#define EnemyMothDeku(player)    0
void FOREIGN_ENEMY_API_MM_KNOCKBACK_SMALL(PlayState*, Actor*, f32, s16, f32, s32, u32);
#define EnemyMothKnockback       FOREIGN_ENEMY_API_MM_KNOCKBACK_SMALL
#define EnemyMothRaycast         BgCheck_EntityRaycastDown1
#define EnemyMothAudio           comboEnemySfxPosition
#define EnemyMothStopAudio       comboEnemySfxStopPosition
#define EnemyMothSoundThreshold  20
#define EnemyMothAlloc           ((void* (*)(u32))0x80066c10)
#define EnemyMothFree            ((void (*)(void*))0x80066c90)
#endif
static void enemyMothAllocationFailed(Actor* actor, PlayState* play) {
    g.roomEnemyLackSoul = 1;
#if defined(GAME_MM)
    comboEnemyNativePlacementBlock(play->roomCtx.curRoom.num);
#endif
    Actor_Kill(actor);
}

static ColliderCylinderInit sEnemyMothCollider = {
    { COL_MATERIAL_NONE, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_TYPE_1, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_NONE },
    { 55, 80, -40, { 0, 0, 0 } }
};
void func_80BB4E50(EnTanron1Struct*, Vec3f*, s16, s16);

static void enemyMothKnockDown(EnTanron1* this) {
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.colChkInfo.health = 0;
    this->mothDeathTimer = 120;
    for (s32 i = 0; i < this->mothCapacity; ++i) {
        EnTanron1Struct* moth = &this->unk_160[i];
        if (moth->unk_24 == 1) {
            moth->unk_24 = 2;
            moth->unk_28 = 6;
            moth->unk_0C.y = -2.f;
            moth->unk_3C = this->actor.home.pos.y - 1000.f;
        }
    }
}

static void enemyMothCombat(EnTanron1* this, PlayState* play) {
    Vec3f center = { 0 };
    s32 live = 0, moving = 0;
    for (s32 i = 0; i < this->mothCapacity; ++i) {
        EnTanron1Struct* moth = &this->unk_160[i];
        if (moth->unk_24)
            ++moving;
        if (moth->unk_24 == 1) {
            ++live;
            center.x += moth->unk_00.x;
            center.y += moth->unk_00.y;
            center.z += moth->unk_00.z;
        }
    }
    if (!moving) {
        this->actor.colChkInfo.health = 0;
        Actor_Kill(&this->actor);
        return;
    }
    if (!live && this->actor.colChkInfo.health) {
        this->actor.colChkInfo.health = 0;
        this->mothDeathTimer = 120;
    }
    if (!this->actor.colChkInfo.health) {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->mothDeathTimer && !--this->mothDeathTimer)
            Actor_Kill(&this->actor);
        return;
    }
    if (live) {
        center.x /= live;
        center.y /= live;
        center.z /= live;
        this->actor.world.pos = center;
        this->actor.focus.pos = center;
        this->actor.focus.rot = this->actor.world.rot;
    }
    if (this->standaloneCollider.base.acFlags & AC_HIT) {
        this->standaloneCollider.base.acFlags &= ~AC_HIT;
        if (this->actor.colChkInfo.damage) {
            enemyMothKnockDown(this);
            return;
        }
    }
    Collider_UpdateCylinder(&this->actor, &this->standaloneCollider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->standaloneCollider.base);
}

u64 ovl_En_Tanron1Tex_001028[] = {
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x000000000000524F,
    0xAC9DAC5B520D0000,
    0x0000000000000000,
    0x0000000000000000,
    0x00000000B49FBD21,
    0xBD23BCE1AC9DA45B,
    0x520D000000000000,
    0x0000000000000000,
    0x0000B4DFC563CDA7,
    0x7B998399BD23B49F,
    0x93D9520D00000000,
    0x0000000000000000,
    0xB4DFC563CDE96B17,
    0x2909318B9C5FBD23,
    0x8397AC5B00000000,
    0x000000000000AC5D,
    0xBD21CDA76B170001,
    0x000100016B17C5A5,
    0x5A51AC5B520D0000,
    0x00000000520DB49D,
    0xC565D62B0001C807,
    0xC80760030001CDE9,
    0x00017B0FAC5D0000,
    0x00000000A4595A4F,
    0x00016B170001C807,
    0xFF01C8070001D62B,
    0x0001990FAC5D49CB,
    0x0000520DAC5B0001,
    0x0001000100016003,
    0xC807C8070001DE6F,
    0x0001990F7B0F0001,
    0x0000A419AC5B0001,
    0x4D07000173590001,
    0x000100010001E6B1,
    0x00010001990F49CB,
    0x0000520DA4590001,
    0x00010001E6F3EF35,
    0x739B000100017359,
    0xD62BAC5B000151CB,
    0x00000000A4195A4F,
    0x0001000100010001,
    0xB569000100010001,
    0x0001AC5B7B0F0001,
    0x000000009BD7AC5B,
    0x5A9100014D070001,
    0xEEF50001C807C807,
    0x60030001000151CB,
    0x000000000000A41B,
    0xBCE1000122830001,
    0xE6F30001C807FF01,
    0xC807000172CF0001,
    0x000000000000520D,
    0xB4DF000100010001,
    0xE6B100016003C807,
    0xC8070001000151CB,
    0x0000000000000000,
    0x5A4FC56362D50001,
    0xDE6F735900010001,
    0x0001BD216ACF0001,
    0x0000000000000000,
    0x0000BD21FFFFCDE9,
    0x6B5700010001CDA7,
    0xBD23B49F0001520D,
    0x0000000000000000,
    0xAC9DB49FBCE1C565,
    0x0001FF0183810001,
    0x0001AC5B51CB7315,
    0x000000000000AC5B,
    0xAC5B520D0000BD23,
    0x0001838100010001,
    0x520DA41920C50001,
    0x000000000000AC5B,
    0x520D00000000C565,
    0x7BDF00010001520D,
    0xA419000049CB7315,
    0x00000000520D0000,
    0x000000000000D62D,
    0xF7BBFFFF520DA419,
    0x0000000018C50001,
    0x0000000000000000,
    0x0000000000000000,
    0xE6F3520DA4190000,
    0x0000000039897315,
    0x0000000000000000,
    0x0000000000000000,
    0xD62DAC5B00000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xCDA7520D00000000,
    0x0000000000006AD3,
    0x0000000000000000,
    0x0000000000000000,
    0xC565000000000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xBD21000000000000,
    0x000000000000520F,
    0x0000000000000000,
    0x0000000000000000,
    0xB4DF000000000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xB4DF520D00000000,
    0x0000000000004189,
    0x0000000000000000,
    0x0000000000000000,
    0xB4DFAC5B520D0000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x5A4FAC5BA4590000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x00005A4F00000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,

};

u64 ovl_En_Tanron1_DL_001428[] = {
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x000000000000524F,
    0x0000AC5B00000000,
    0x0000000000000000,
    0x0000000000000000,
    0x00000000B49F0000,
    0xBD230000AC9D0000,
    0x520D000000000000,
    0x0000000000000000,
    0x0000B4DFC563CDA7,
    0x000000000000B49F,
    0x0000520D00000000,
    0x0000000000000000,
    0xB4DF0000CDE96B17,
    0x2909318B00000000,
    0x0000AC5B00000000,
    0x000000000000AC5D,
    0x0000CDA76B170001,
    0x0001000100000000,
    0x0000AC5B520D0000,
    0x00000000520DB49D,
    0xC565D62B00010000,
    0x000060030001CDE9,
    0x00010000AC5D0000,
    0x0000000000000000,
    0x000100000001C807,
    0x0000C8070001D62B,
    0x0001000000000000,
    0x0000520DAC5B0001,
    0x0001000100010000,
    0x0000C80700010000,
    0x0001990F00000001,
    0x0000000000000001,
    0x0000000173590001,
    0x0001000100010000,
    0x00010001000049CB,
    0x0000000000000001,
    0x00010001E6F30000,
    0x739B000100010000,
    0x0000000000010000,
    0x00000000A4190000,
    0x0001000100010001,
    0x0000000100010001,
    0x0001000000000001,
    0x000000009BD7AC5B,
    0x000000014D070001,
    0x00000001C8070000,
    0x60030001000151CB,
    0x0000000000000000,
    0xBCE1000100000001,
    0x000000010000FF01,
    0x0000000172CF0001,
    0x0000000000000000,
    0xB4DF000100010001,
    0x0000000100000000,
    0x0000000100010000,
    0x0000000000000000,
    0x0000C56300000001,
    0xDE6F000000010001,
    0x0001BD2100000001,
    0x0000000000000000,
    0x00000000FFFFCDE9,
    0x6B5700010001CDA7,
    0xBD23000000010000,
    0x0000000000000000,
    0x0000B49F0000C565,
    0x0001FF0183810001,
    0x0001000051CB7315,
    0x0000000000000000,
    0xAC5B000000000000,
    0x0001838100010001,
    0x0000A41900000001,
    0x000000000000AC5B,
    0x000000000000C565,
    0x0000000100010000,
    0x0000000000007315,
    0x0000000000000000,
    0x0000000000000000,
    0xF7BB000000000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000039890000,
    0x0000000000000000,
    0x0000000000000000,
    0xD62DAC5B00000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xCDA7000000000000,
    0x0000000000006AD3,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xBD21000000000000,
    0x000000000000520F,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000001,
    0x0000000000000000,
    0x0000000000000000,
    0xB4DF520D00000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x00000000520D0000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x00000000A4590000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x00005A4F00000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,
    0x0000000000000000,

};

Vtx ovl_En_Tanron1Vtx_001828[] = {
    VTX(0, -3, -7, 538, 1338, 84, 171, 0, 255),
    VTX(7, 5, 0, -346, 666, 84, 171, 0, 255),
    VTX(0, -3, 7, 510, -397, 84, 171, 0, 255),
    VTX(0, -3, 7, 510, -397, 172, 171, 0, 255),
    VTX(-7, 5, 0, -346, 666, 172, 171, 0, 255),
    VTX(0, -3, -7, 538, 1338, 172, 171, 0, 255),

};

Gfx ovl_En_Tanron1_DL_001888[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(ovl_En_Tanron1Tex_001028, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

Gfx ovl_En_Tanron1_DL_001900[] = {
    gsSPVertex(ovl_En_Tanron1Vtx_001828, 6, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSPEndDisplayList(),
};

void EnTanron1_Init(Actor* thisx, PlayState* play) {
    EnTanron1* this = (EnTanron1*)thisx;

    this->standalone = this->actor.params == 0x14 || this->actor.params == ENEMY_MOTH_STANDALONE;
    this->mothCapacity = this->standalone ? 1 + (s16)Rand_ZeroFloat(10.f) : 200;
    this->unk_160 = EnemyMothAlloc(sizeof(*this->unk_160) * this->mothCapacity);
    if (!this->unk_160) {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->standalone)
            enemyMothAllocationFailed(thisx, play);
        return;
    }
    memset(this->unk_160, 0, sizeof(*this->unk_160) * this->mothCapacity);
    if (this->standalone) {
        this->actor.params = this->mothCapacity;
        this->actor.colChkInfo.health = 1;
        Collider_InitCylinder(play, &this->standaloneCollider);
        Collider_SetCylinder(play, &this->standaloneCollider, thisx, &sEnemyMothCollider);
        this->unk_144 = 0;
        return;
    }
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (!ENTANRON1_GET_100(&this->actor)) {
        this->unk_144 = 0;
    } else {
        this->actor.params = 200;
        this->unk_144 = 1;
    }
}

void EnTanron1_Destroy(Actor* thisx, PlayState* play) {
    EnTanron1* this = (EnTanron1*)thisx;
    if (this->standalone && this->unk_160)
        Collider_DestroyCylinder(play, &this->standaloneCollider);
    EnemyMothStopAudio(&this->unk_3360, NA_SE_EN_MB_MOTH_FLY - SFX_FLAG);
    EnemyMothStopAudio(&this->unk_3360, NA_SE_EN_MB_MOTH_DEAD);
    if (this->unk_160)
        EnemyMothFree(this->unk_160);
    this->unk_160 = NULL;
}

void func_80BB4E50(EnTanron1Struct* arg0, Vec3f* arg1, s16 arg2, s16 capacity) {
    s16 i;

    for (i = 0; i < capacity; i++, arg0++) {
        if (arg0->unk_24 == 0) {
            arg0->unk_24 = 1;
            arg0->unk_2C = 0.0f;
            arg0->unk_00 = *arg1;
            arg0->unk_18 = arg0->unk_1C = 0;
            arg0->unk_1A = arg2;
            arg0->unk_26 = Rand_ZeroFloat(100.0f);
            arg0->unk_28 = 0;
            if (Rand_ZeroOne() < 0.5f) {
                arg0->unk_2A = 0xC00;
            } else {
                arg0->unk_2A = -0xC00;
            }
            break;
        }
    }
}

void EnTanron1_Update(Actor* thisx, PlayState* play) {
    EnTanron1* this = (EnTanron1*)thisx;
    Actor* temp_a0;
    Player* player = GET_PLAYER(play);
    s16 i;
    EnTanron1Struct* ptr = &this->unk_160[0];
    Vec3f temp;
    f32 phi_f18;

    if (!this->unk_160) {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->standalone)
            enemyMothAllocationFailed(thisx, play);
        return;
    }
    if (this->standalone && this->unk_144 != 0) {
        f32 dx = player->actor.world.pos.x - this->actor.home.pos.x;
        f32 dz = player->actor.world.pos.z - this->actor.home.pos.z;
        this->unk_144 = dx * dx + dz * dz < 122500.f ? 1 : 200;
        if (this->unk_144 == 200)
            this->actor.world.pos = this->actor.home.pos;
    }
    if (this->unk_148 != 0) {
        this->unk_148--;
    }

    switch (this->unk_144) {
        case 0:
            for (i = 0; i < this->actor.params && i < this->mothCapacity; i++) {
                func_80BB4E50(this->unk_160, &this->actor.world.pos, Rand_ZeroFloat(0x10000), this->mothCapacity);
            }
            this->unk_144 = 200;
            break;

        case 1:
            this->unk_14C.x = player->actor.world.pos.x;
            this->unk_14C.y = player->actor.world.pos.y + 60.0f;
            this->unk_14C.z = player->actor.world.pos.z;
            if (this->unk_148 != 0) {
                func_80BB4E50(this->unk_160, &this->actor.world.pos, this->actor.world.rot.y, this->mothCapacity);
            }
            this->unk_158 = 0x1000;
            this->unk_15C = 150.0f;
            break;

        case 100:
            phi_f18 = 10.0f;

            while (phi_f18 < 1000.0f) {
                temp_a0 = play->actorCtx.actors[ACTORCAT_PROP].first;
                while (temp_a0 != NULL) {
                    if (temp_a0->id != ENEMY_MOTH_TORCH_ID) {
                        temp_a0 = temp_a0->next;
                        continue;
                    }

                    temp.x = this->unk_14C.x - temp_a0->world.pos.x;
                    temp.y = this->unk_14C.y - temp_a0->world.pos.y + 70.0f;
                    temp.z = this->unk_14C.z - temp_a0->world.pos.z;

                    if (sqrtf(SQXYZ(temp)) < phi_f18) {
                        this->unk_14C.x = temp_a0->world.pos.x;
                        this->unk_14C.y = temp_a0->world.pos.y + 70.0f;
                        this->unk_14C.z = temp_a0->world.pos.z;
                        goto end;
                    }
                    temp_a0 = temp_a0->next;
                }
                phi_f18 += 20.0f;
            }
        end:
            break;

        case 200:
            this->unk_14C = this->actor.world.pos;
            this->unk_158 = 0x5000;
            this->unk_15C = 50.0f;
            break;

        case 250:
            for (i = 0; i < this->mothCapacity; i++, ptr++) {
                if ((ptr->unk_24 != 0) && (ptr->unk_28 < 8)) {
                    ptr->unk_28 = 8;
                    ptr->unk_24 = 2;
                }
            }
            this->unk_144 = 1;
            break;

        default:
            break;
    }

    if ((player->heldItemAction == PLAYER_IA_DEKU_STICK) && (EnemyMothBurning(player) != 0)) {
        this->unk_14C.x = player->meleeWeaponInfo[0].tip.x;
        this->unk_14C.y = player->meleeWeaponInfo[0].tip.y;
        this->unk_14C.z = player->meleeWeaponInfo[0].tip.z;

        this->unk_158 = 0x5000;
        this->unk_15C = 50.0f;
        if (this->unk_144 != 1) {
            this->unk_144 = 100;
        }
    } else {
        temp_a0 = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
        while (temp_a0 != NULL) {
            if (temp_a0->params == 1) {
                temp_a0 = temp_a0->next;
                continue;
            }

            this->unk_14C.x = temp_a0->world.pos.x;
            this->unk_14C.y = temp_a0->world.pos.y + 50.0f;
            this->unk_14C.z = temp_a0->world.pos.z;

            this->unk_15C = 150.0f;
            this->unk_158 = 0x1000;
            if (this->unk_144 != 1) {
                this->unk_144 = 100;
            }
            break;
        }
    }
    if (this->standalone && this->unk_144 == 200) {
        f32 nearest = 300.f * 300.f;
        for (Actor* torch = play->actorCtx.actors[ACTORCAT_PROP].first; torch; torch = torch->next) {
            if (torch->id != ENEMY_MOTH_TORCH_ID || !torch->update || !((EnemyNativeTorch*)torch)->litTimer)
                continue;
            f32 dx = torch->world.pos.x - this->actor.home.pos.x;
            f32 dy = torch->world.pos.y - this->actor.home.pos.y;
            f32 dz = torch->world.pos.z - this->actor.home.pos.z;
            f32 distance = dx * dx + dy * dy + dz * dz;
            if (distance >= nearest)
                continue;
            nearest = distance;
            this->unk_14C = torch->world.pos;
            this->unk_14C.y += 52.f;
        }
    }
    func_80BB5318(this, play);
    if (this->standalone)
        enemyMothCombat(this, play);
}

void EnTanron1_Draw(Actor* thisx, PlayState* play) {
    EnTanron1* this = (EnTanron1*)thisx;

    func_80BB5AAC(this, play);
}

void func_80BB5318(EnTanron1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnTanron1Struct* ptr = NULL;
    f32 phi_f28 = 0.0f;
    Vec3f* phi_s2 = NULL;
    Vec3f temp;
    s16 spBA = 0;
    s16 spB8 = 0;
    Vec3f* spB4 = NULL;
    f32 spB0;
    Vec3f spA4;
    WaterBox* waterBox;
    f32 sp9C;
    CollisionPoly* sp98;
    Actor* temp_v0;
    s16 i;
    f32 temp_f30 = this->unk_15C;

    if (EnemyMothMelee(player) != 0) {
        phi_s2 = &player->meleeWeaponInfo[0].tip;
        if (player->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) {
            phi_f28 = 2500.0f;
        } else {
            phi_f28 = 400.0f;
        }
    }

    temp_v0 = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
    while (temp_v0 != NULL) {
        if (temp_v0->params != 1) {
            temp_v0 = temp_v0->next;
            continue;
        }
        phi_s2 = &temp_v0->world.pos;
        phi_f28 = 40000.0f;
        break;
    }

    ptr = &this->unk_160[0];
    for (i = 0; i < this->actor.params && i < this->mothCapacity; i++, ptr++) {
        if (ptr->unk_24 != 0) {
            ptr->unk_26++;
            ptr->unk_00.x += ptr->unk_0C.x;
            ptr->unk_00.y += ptr->unk_0C.y;
            ptr->unk_00.z += ptr->unk_0C.z;

            if (ptr->unk_24 == 0) {
            } else {
                spB4 = &ptr->unk_00;
                if (ptr->unk_28 == 0) {
                    spBA++;
                    ptr->unk_2C = Math_SinS(ptr->unk_26 * 0x5000) * 1.2f;
                    if ((ptr->unk_26 & 3) == 0) {
                        temp.x = ptr->unk_30 + (this->unk_14C.x - ptr->unk_00.x);
                        temp.y = ptr->unk_34 + (this->unk_14C.y - ptr->unk_00.y);
                        temp.z = ptr->unk_38 + (this->unk_14C.z - ptr->unk_00.z);

                        ptr->unk_20 = Math_Atan2S(temp.x, temp.z);
                        ptr->unk_1E = Math_Atan2S(temp.y, sqrtf(SQXZ(temp)));
                        if ((ptr->unk_26 & 0xF) == 0) {
                            ptr->unk_30 = Rand_CenteredFloat(temp_f30);
                            ptr->unk_34 = Rand_CenteredFloat(temp_f30 * 0.5f);
                            ptr->unk_38 = Rand_CenteredFloat(temp_f30);
                        }

                        temp.x = player->actor.world.pos.x - ptr->unk_00.x;
                        temp.y = (player->actor.world.pos.y + 40.0f) - ptr->unk_00.y;
                        temp.z = player->actor.world.pos.z - ptr->unk_00.z;

                        if ((SQXYZ(temp) < 400.0f) && (!EnemyMothDeku(player))) {
                            EnemyMothKnockback(play, &this->actor, 0.0f, 0, 0.0f, 1, 1);
                        }
                    }

                    Math_ApproachS(&ptr->unk_1A, ptr->unk_20, 2, this->unk_158);
                    Math_ApproachS(&ptr->unk_18, ptr->unk_1E, 2, this->unk_158);
                    Matrix_RotateYS(ptr->unk_1A, MTXMODE_NEW);
                    Matrix_RotateXS(-ptr->unk_18, MTXMODE_APPLY);
                    Matrix_MultVecZ(6.0f, &ptr->unk_0C);

                    if (phi_s2 != NULL) {
                        temp.x = phi_s2->x - ptr->unk_00.x;
                        temp.y = phi_s2->y - ptr->unk_00.y;
                        temp.z = phi_s2->z - ptr->unk_00.z;

                        if (SQXYZ(temp) < phi_f28) {
                            ptr->unk_20 = Math_Atan2S(temp.x, temp.z);
                            ptr->unk_1E = Math_Atan2S(temp.y, sqrtf(SQXZ(temp)));

                            Matrix_RotateYS(ptr->unk_20, MTXMODE_NEW);
                            Matrix_RotateXS(-ptr->unk_1E, MTXMODE_APPLY);
                            Matrix_MultVecZ(-20.0f, &ptr->unk_0C);

                            if (phi_f28 >= 100000.0f) {
                                ptr->unk_28 = 1;
                            } else {
                                ptr->unk_28 = 6;
                            }
                            ptr->unk_24 = 2;
                            spB8++;
                        }
                    }
                } else if (ptr->unk_28 < 9) {
                    ptr->unk_18 += 0x3000;
                    ptr->unk_1A += 0x5000;
                    ptr->unk_30 = 0.0f;
                    ptr->unk_34 = 0.0f;
                    ptr->unk_28++;
                } else {
                    ptr->unk_1A += ptr->unk_2A;
                    Math_ApproachS(&ptr->unk_18, 0, 0xA, 0x1000);
                    Matrix_RotateYS(ptr->unk_1A, MTXMODE_NEW);
                    Matrix_MultVecZ(ptr->unk_30, &spA4);

                    ptr->unk_0C.x = spA4.x;
                    ptr->unk_0C.z = spA4.z;
                    ptr->unk_0C.y = -2.0f;

                    if (phi_s2 != NULL) {
                        temp.x = phi_s2->x - ptr->unk_00.x;
                        temp.y = phi_s2->y - ptr->unk_00.y;
                        temp.z = phi_s2->z - ptr->unk_00.z;

                        if (SQXYZ(temp) < phi_f28) {
                            ptr->unk_20 = Math_Atan2S(temp.x, temp.z);
                            ptr->unk_1E = Math_Atan2S(temp.y, sqrtf(SQXZ(temp)));

                            Matrix_RotateYS(ptr->unk_20, MTXMODE_NEW);
                            Matrix_RotateXS(-ptr->unk_1E, MTXMODE_APPLY);
                            Matrix_MultVecZ(-20.0f, &ptr->unk_0C);

                            ptr->unk_3C = ptr->unk_00.y - 1000.0f;
                            ptr->unk_30 = 5.0f;
                        }
                    }

                    if (ptr->unk_00.y <= (ptr->unk_3C + 5.0f)) {
                        ptr->unk_00.y = (ptr->unk_3C + 5.0f);
                        Math_ApproachZeroF(&ptr->unk_30, 1.0f, 0.3f);
                        Math_ApproachS(&ptr->unk_2A, 0, 1, 0x100);
                        ptr->unk_28++;
                        if (ptr->unk_28 > 50) {
                            ptr->unk_24 = 0;
                        }
                    } else {
                        Math_ApproachF(&ptr->unk_30, ptr->unk_34, 1.0f, 0.5f);
                        if ((ptr->unk_26 & 0xF) == 0) {
                            if (Rand_ZeroOne() < 0.5f) {
                                ptr->unk_34 = Rand_CenteredFloat(12.0f);
                            }
                            ptr->unk_3C = EnemyMothRaycast(&play->colCtx, &sp98, &ptr->unk_00);
                            if (this->standalone && !(ptr->unk_3C > -30000.f && ptr->unk_3C < 30000.f))
                                ptr->unk_3C = this->actor.home.pos.y - 40.f;
                            sp9C = ptr->unk_00.y;
                            BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, ptr->unk_00.x, ptr->unk_00.z, &sp9C,
                                                             &waterBox);
                            if ((sp9C < ptr->unk_00.y) && (ptr->unk_3C < sp9C)) {
                                ptr->unk_3C = sp9C;
                            }
                        }
                    }
                }
            }
        }
    }

    if (spB4 != NULL) {
        SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, spB4, &this->unk_3360, &spB0);
        if (spB8 >= (s16)EnemyMothSoundThreshold) {
            EnemyMothAudio(&this->unk_3360, NA_SE_EN_MB_MOTH_DEAD);
        } else if (spBA >= 20) {
            EnemyMothAudio(&this->unk_3360, NA_SE_EN_MB_MOTH_FLY - SFX_FLAG);
        }
    }
}

void func_80BB5AAC(EnTanron1* this, PlayState* play) {
    EnTanron1Struct* ptrBase = &this->unk_160[0];
    s16 i;
    u8 flag = 0;
    EnTanron1Struct* ptr = ptrBase;
    if (!ptrBase)
        return;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    for (i = 0; i < this->actor.params && i < this->mothCapacity; i++, ptr++) {
        if (ptr->unk_24 == 1) {
            if (!flag) {
                gSPDisplayList(POLY_OPA_DISP++, ovl_En_Tanron1_DL_001888);
                flag++;
            }
            Matrix_Translate(ptr->unk_00.x, ptr->unk_00.y, ptr->unk_00.z, MTXMODE_NEW);
            Matrix_RotateYS(ptr->unk_1A, MTXMODE_APPLY);
            Matrix_RotateXS(ptr->unk_18 * -1, MTXMODE_APPLY);
            Matrix_Scale(1.2f, ptr->unk_2C, 1.2f, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, ovl_En_Tanron1_DL_001900);
        }
    }

    flag = 0;
    ptr = ptrBase;
    for (i = 0; i < this->actor.params && i < this->mothCapacity; i++, ptr++) {
        if (ptr->unk_24 == 2) {
            if (!flag) {
                gSPDisplayList(POLY_OPA_DISP++, ovl_En_Tanron1_DL_001888);
                gDPLoadTextureBlock(POLY_OPA_DISP++, ovl_En_Tanron1_DL_001428, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0,
                                    G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD,
                                    G_TX_NOLOD);
                flag++;
            }

            Matrix_Translate(ptr->unk_00.x, ptr->unk_00.y, ptr->unk_00.z, MTXMODE_NEW);
            Matrix_RotateYS(ptr->unk_1A, MTXMODE_APPLY);
            Matrix_RotateXS(ptr->unk_18 * -1, MTXMODE_APPLY);
            Matrix_Scale(1.0f, ptr->unk_2C, 1.0f, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, ovl_En_Tanron1_DL_001900);
        }
    }

    CLOSE_DISPS();
}

#if defined(GAME_MM)
_Static_assert(__builtin_offsetof(EnTanron1, unk_144) == 0x144, "Odolwa swarm phase ABI");
_Static_assert(__builtin_offsetof(EnTanron1, unk_148) == 0x148, "Odolwa swarm timer ABI");
_Static_assert(sizeof(EnTanron1Struct) == 0x40, "Native moth particle ABI");
OVL_INFO_ACTOR(0x23d, En_Tanron1_Profile);
EXPORT_SYMBOL(ENEMY_NATIVE_MOTH_MM_V1, EnTanron1_Init);
#else
OVL_INFO_NATIVE_ENEMY(0x23d, En_Tanron1_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_TANRON1, En_Tanron1_Profile, EnTanron1_Init, EnTanron1_Destroy,
                     EnTanron1_Update, EnTanron1_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x23du, 0x1u, 0xa3dbd493u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_TANRON1_ORIGIN, generatedNativeOrigin);
#endif
