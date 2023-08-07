#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>

# define POT_TEX_NORMAL_SIDE    0x06000000
# define POT_TEX_NORMAL_TOP     0x06001000
# define POT_TEX_MAGIC_SIDE     0x060003e0
# define POT_TEX_MAGIC_TOP      0x060013e0

#if defined(GAME_OOT)
# define POT_TEX_NORMAL_DANGEON_SIDE    0x050108a0
# define POT_TEX_NORMAL_DANGEON_TOP     0x050118a0
#else
# define POT_TEX_NORMAL_DANGEON_SIDE    0x05010ec0
# define POT_TEX_NORMAL_DANGEON_TOP     0x05011ec0
#endif

typedef struct
{
    u32 custom:1;
    u32 segSide:31;
    u32 segTop;
}
CsmcPotData;

static const CsmcPotData kCsmcData[] = {
    { 0, POT_TEX_NORMAL_SIDE, POT_TEX_NORMAL_TOP },
    { 0, POT_TEX_NORMAL_DANGEON_SIDE, POT_TEX_NORMAL_DANGEON_TOP },
    { 0, POT_TEX_MAGIC_SIDE, POT_TEX_MAGIC_TOP },
    { 1, CUSTOM_POT_MAJOR_SIDE_ADDR, CUSTOM_POT_MAJOR_TOP_ADDR },
    { 1, CUSTOM_POT_SPIDER_SIDE_ADDR, CUSTOM_POT_SPIDER_TOP_ADDR },
    { 1, CUSTOM_POT_FAIRY_SIDE_ADDR, CUSTOM_POT_FAIRY_TOP_ADDR },
    { 1, CUSTOM_POT_KEY_SIDE_ADDR, CUSTOM_POT_SPIDER_TOP_ADDR },
    { 1, CUSTOM_POT_HEART_SIDE_ADDR, CUSTOM_POT_HEART_TOP_ADDR },
    { 1, CUSTOM_POT_BOSSKEY_SIDE_ADDR, CUSTOM_POT_BOSSKEY_TOP_ADDR },
};

static int csmcPotId(s16 gi, int def)
{
    int csmcId;

    if (gi == 0)
        return def;
    if (!csmcEnabled())
        return CSMC_POT_MAJOR;

    csmcId = csmcFromItem(gi);
    switch (csmcId)
    {
    case CSMC_NORMAL:       return def;
    case CSMC_MAJOR:        return CSMC_POT_MAJOR;
    case CSMC_KEY:          return CSMC_POT_KEY;
    case CSMC_SPIDER:       return CSMC_POT_SPIDER;
    case CSMC_FAIRY:        return CSMC_POT_FAIRY;
    case CSMC_HEART:        return CSMC_POT_HEART;
    case CSMC_BOSS_KEY:     return CSMC_POT_BOSSKEY;
    case CSMC_SOUL:         return CSMC_POT_BOSSKEY;
    default:                return CSMC_POT_MAJOR;
    }
}

void csmcPotPreDraw(Actor* this, GameState_Play* play, s16 gi, int def)
{
    const void* listSide;
    const void* listTop;
    int type;

    type = csmcPotId(gi, def);
    listSide = csmcLoadTexture(kCsmcData[type].custom, kCsmcData[type].segSide, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0);
    listTop = csmcLoadTexture(kCsmcData[type].custom, kCsmcData[type].segTop, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0);

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, listSide);
    gSPSegment(POLY_OPA_DISP++, 0x0b, listTop);
    CLOSE_DISPS();
}
