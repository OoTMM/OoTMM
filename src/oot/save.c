#include <combo.h>

#define ENTRANCE_MIDO         0x433
#define ENTRANCE_HCASTLE      0x138
#define ENTRANCE_BOSS1        0x40f
#define ENTRANCE_KAKARIKO     0x195
#define ENTRANCE_SMEADOW      0x215
#define ENTRANCE_GORON        0x14d
#define ENTRANCE_GF           0x315
#define ENTRANCE_ZWF          0x1dd
#define ENTRANCE_ZRI          0x0ea
#define ENTRANCE_HYLIA        0x560
#define ENTRANCE_ZD           0x108
#define ENTRANCE_BOSS3        0x301
#define ENTRANCE_HYRULE       0x17d
#define ENTRANCE_MARKET       0x1d1
#define ENTRANCE_DEKUTREE     0x000
#define ENTRANCE_TOT          0x053
#define ENTRANCE_LAKE         0x4e6
#define ENTRANCE_ZELDA        0x400
#define ENTRANCE_RANCH        0x157
#define ENTRANCE_RT           0x02d
#define ENTRANCE_WINDMILL     0x453
#define ENTRANCE_DMC          0x246
#define ENTRANCE_FISH         0x45f
#define ENTRANCE_COLOSSUS     0x1e1
#define ENTRANCE_FIRE         0x165
#define ENTRANCE_LW           0x11e
#define ENTRANCE_POTION_SHOP  0x072
#define ENTRANCE_VALLEY       0x3d0
#define ENTRANCE_BIGGORON     0x1bd
#define ENTRANCE_LAB          0x043
#define ENTRANCE_BOSS2        0x40b
#define ENTRANCE_BOSS4        0x00c
#define ENTRANCE_BOSS5        0x305
#define ENTRANCE_BOSS6        0x417
#define ENTRANCE_BOSS7        0x413

void Sram_AfterOpenSave(void)
{
#if defined(DEBUG)
    //gSave.entrance = ENTRANCE_MIDO;
    gSave.entrance = ENTRANCE_MARKET;
#endif

    if (gComboCtx.valid)
    {
        gSave.entrance = ENTRANCE_MARKET;
        gComboCtx.valid = 0;
    }
}
