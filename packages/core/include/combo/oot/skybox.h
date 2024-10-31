#ifndef INCLUDED_COMBO_OOT_SKYBOX_H
#define INCLUDED_COMBO_OOT_SKYBOX_H

#include <PR/gbi.h>
#include <combo/math.h>
#include <combo/util.h>
#include <combo/misc.h>

struct GameState;
struct GraphicsContext;

typedef enum SkyboxId
{
    SKYBOX_NONE,
    SKYBOX_NORMAL_SKY,
    SKYBOX_BAZAAR,
    SKYBOX_OVERCAST_SUNSET,
    SKYBOX_MARKET_ADULT,
    SKYBOX_CUTSCENE_MAP,
    SKYBOX_HOUSE_LINK = 7,
    SKYBOX_MARKET_CHILD_DAY = 9,
    SKYBOX_MARKET_CHILD_NIGHT,
    SKYBOX_HAPPY_MASK_SHOP,
    SKYBOX_HOUSE_KNOW_IT_ALL_BROTHERS,
    SKYBOX_HOUSE_OF_TWINS = 14,
    SKYBOX_STABLES,
    SKYBOX_HOUSE_KAKARIKO,
    SKYBOX_KOKIRI_SHOP,
    SKYBOX_GORON_SHOP = 19,
    SKYBOX_ZORA_SHOP,
    SKYBOX_POTION_SHOP_KAKARIKO = 22,
    SKYBOX_POTION_SHOP_MARKET,
    SKYBOX_BOMBCHU_SHOP,
    SKYBOX_HOUSE_RICHARD = 26,
    SKYBOX_HOUSE_IMPA,
    SKYBOX_TENT,
    SKYBOX_UNSET_1D,
    SKYBOX_HOUSE_MIDO = 32,
    SKYBOX_HOUSE_SARIA,
    SKYBOX_HOUSE_ALLEY,
    SKYBOX_UNSET_27 = 39
}
SkyboxId;

typedef enum SkyboxDrawType
{
    SKYBOX_DRAW_128,       // 128x128 top/bottom faces, 128x64 side faces
    SKYBOX_DRAW_256_4FACE, // 256x256 all side faces with per-face palettes
    SKYBOX_DRAW_256_3FACE  // 256x256 3/4 side faces with per-face palettes
}
SkyboxDrawType;

typedef struct SkyboxContext {
    /* 0x000 */ char unk_00[0x128];
    /* 0x128 */ void* staticSegments[2];
    /* 0x130 */ u16 (*palettes)[256];
    /* 0x134 */ Gfx (*dListBuf)[150];
    /* 0x138 */ Gfx* gfx;
    /* 0x13C */ Vtx* roomVtx;
    /* 0x140 */ s16 drawType;
    /* 0x144 */ Vec3f rot;
    /* 0x150 */ char unk_150[0x10];
} SkyboxContext; // size = 0x160

typedef struct SkyboxFile {
    /* 0x00 */ RomFile file;
    /* 0x08 */ RomFile palette;
} SkyboxFile; // size = 0x10

extern SkyboxFile gNormalSkyFiles[];

void Skybox_Init(struct GameState* state, SkyboxContext* skyboxCtx, s16 skyboxId);
Mtx* Skybox_UpdateMatrix(SkyboxContext* skyboxCtx, f32 x, f32 y, f32 z);
void Skybox_Draw(SkyboxContext* skyboxCtx, struct GraphicsContext* gfxCtx, s16 skyboxId, s16 blend, f32 x, f32 y, f32 z);
void Skybox_Update(SkyboxContext* skyboxCtx);

#endif
