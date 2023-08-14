#include <combo.h>
#include <combo/xflags.h>
#include <combo/item.h>
#include <combo/custom.h>
#include <combo/dma.h>

#if defined(GAME_OOT)
# define VROM_TABLE_SCENES CUSTOM_XFLAG_TABLE_OOT_SCENES_ADDR
# define VROM_TABLE_SETUPS CUSTOM_XFLAG_TABLE_OOT_SETUPS_ADDR
# define VROM_TABLE_ROOMS CUSTOM_XFLAG_TABLE_OOT_ROOMS_ADDR
#else
# define VROM_TABLE_SCENES CUSTOM_XFLAG_TABLE_MM_SCENES_ADDR
# define VROM_TABLE_SETUPS CUSTOM_XFLAG_TABLE_MM_SETUPS_ADDR
# define VROM_TABLE_ROOMS CUSTOM_XFLAG_TABLE_MM_ROOMS_ADDR
#endif

u16 comboXflagsBitPosLookup(const Xflag* xf)
{
    static u32 paddrTableScenes;
    static u32 paddrTableSetups;
    static u32 paddrTableRooms;
    DmaEntry dmaEntry;
    u16 setupIndex;
    u16 roomIndex;

    /* Init */
    if (paddrTableScenes == 0)
    {
        comboDmaLookup(&dmaEntry, VROM_TABLE_SCENES);
        paddrTableScenes = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, VROM_TABLE_SETUPS);
        paddrTableSetups = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, VROM_TABLE_ROOMS);
        paddrTableRooms = dmaEntry.pstart;
    }

    setupIndex = comboReadPhysU16(paddrTableScenes + xf->sceneId * 2) + xf->setupId;
    roomIndex = comboReadPhysU16(paddrTableSetups + setupIndex * 2) + (xf->roomId * 12) + xf->sliceId;
    return comboReadPhysI16(paddrTableRooms + roomIndex * 2) + xf->id;
}

int comboXflagsGet(const Xflag* xf)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(xf);
    return !!BITMAP8_GET(gCustomSave.xflags, bitPos);
}

void comboXflagsSet(const Xflag* xf)
{
    u16 bitPos;

    bitPos = comboXflagsBitPosLookup(xf);
    BITMAP8_SET(gCustomSave.xflags, bitPos);
}

void comboXflagItemQuery(ComboItemQuery* q, const Xflag* xf, s16 gi)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_XFLAG0 + xf->sliceId;
    q->sceneId = xf->sceneId;
    q->roomId = (xf->roomId | ((xf->setupId & 3) << 6));
    q->id = xf->id;
    q->gi = gi;
}

void comboXflagItemOverride(ComboItemOverride* o, const Xflag* xf, s16 gi)
{
    ComboItemQuery q;

    comboXflagItemQuery(&q, xf, gi);
    comboItemOverride(o, &q);
}
