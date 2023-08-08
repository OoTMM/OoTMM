#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>

static const CsmcDisplayList kGrassDlist[] = {
    { 0x0500b140,           0xffffffff, CTF_COLOR | CTF_CLAMP,                       G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32 },
    { CUSTOM_GRASS_ADDR,    0xffff00ff, CTF_CUSTOM_TEXTURE | CTF_COLOR | CTF_CLAMP,  G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32 },
};

static int csmcGrassId(s16 gi, int def)
{
    int csmcId;

    if (gi == 0)
        return def;
    if (!csmcEnabled())
        return CSMC_GRASS_MAJOR;

    /* DEBUG */
    return CSMC_GRASS_MAJOR;

    csmcId = csmcFromItem(gi);
    switch (csmcId)
    {
    case CSMC_NORMAL:       return def;
    case CSMC_MAJOR:        return CSMC_GRASS_MAJOR;
    default:                return CSMC_GRASS_MAJOR;
    }
}

void csmcGrassPreDraw(GameState_Play* play, s16 gi, int def)
{
    int id;
    const void* list;

    id = csmcGrassId(gi, def);
    list = csmcLoadTextureEx(&kGrassDlist[id]);

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x0a, list);
    CLOSE_DISPS();
}
