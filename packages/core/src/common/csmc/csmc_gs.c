#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>

static const Gfx kCsmcGsNormal[] = {
    gsDPSetPrimColor(0x00, 0x00, 117, 87, 38, 255),
    gsDPSetEnvColor(12, 9, 4, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsBossKey[] = {
    gsDPSetPrimColor(0x00, 0x00, 0, 0, 255, 255),
    gsDPSetEnvColor(0, 0, 25, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsMajor[] = {
    gsDPSetPrimColor(0x00, 0x00, 240, 220, 30, 255),
    gsDPSetEnvColor(20, 20, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsKey[] = {
    gsDPSetPrimColor(0x00, 0x00, 100, 100, 100, 255),
    gsDPSetEnvColor(10, 10, 10, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsSpider[] = {
    gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 255),
    gsDPSetEnvColor(25, 25, 25, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsFairy[] = {
    gsDPSetPrimColor(0x00, 0x00, 230, 157, 242, 255),
    gsDPSetEnvColor(23, 15, 24, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsHeart[] = {
    gsDPSetPrimColor(0x00, 0x00, 255, 0, 0, 255),
    gsDPSetEnvColor(25, 0, 0, 255),
    gsSPEndDisplayList(),
};

static const Gfx kCsmcGsSoul[] = {
    gsDPSetPrimColor(0x00, 0x00, 52, 11, 156, 255),
    gsDPSetEnvColor(5, 1, 15, 255),
    gsSPEndDisplayList(),
};

static const Gfx* const kCsmcGs[] = {
    kCsmcGsNormal,
    kCsmcGsBossKey,
    kCsmcGsMajor,
    kCsmcGsKey,
    kCsmcGsSpider,
    kCsmcGsFairy,
    kCsmcGsHeart,
    kCsmcGsSoul,
};

static int csmcGsId(s16 gi)
{
    int csmcId;

    if (gi == 0 || !csmcEnabledExtra())
        return CSMC_GS_MAJOR;

    csmcId = csmcFromItem(gi);
    switch (csmcId)
    {
    case CSMC_NORMAL:       return CSMC_GS_NORMAL;
    case CSMC_BOSS_KEY:     return CSMC_GS_BOSS_KEY;
    case CSMC_MAJOR:        return CSMC_GS_MAJOR;
    case CSMC_KEY:          return CSMC_GS_KEY;
    case CSMC_SPIDER:       return CSMC_GS_SPIDER;
    case CSMC_FAIRY:        return CSMC_GS_FAIRY;
    case CSMC_HEART:        return CSMC_GS_HEART;
    case CSMC_SOUL:         return CSMC_GS_SOUL;
    default:                return CSMC_GS_MAJOR;
    }
}

void csmcGsPreDraw(GameState_Play* play, s16 gi)
{
    int id;

    id = csmcGsId(gi);
    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, ((u32)kCsmcGs[id]) & 0xffffff);
    CLOSE_DISPS();
}
