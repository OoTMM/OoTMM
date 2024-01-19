#include <combo.h>
#include <combo/xflags.h>
#include <combo/item.h>
#include <combo/custom.h>
#include <combo/dma.h>

static u16 bitPosLookupTables(const Xflag* xf, u32 vromScenes, u32 vromSetups, u32 vromRooms)
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
        comboDmaLookup(&dmaEntry, vromScenes);
        paddrTableScenes = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, vromSetups);
        paddrTableSetups = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, vromRooms);
        paddrTableRooms = dmaEntry.pstart;
    }

    setupIndex = comboReadPhysU16(paddrTableScenes + xf->sceneId * 2) + xf->setupId;
    roomIndex = comboReadPhysU16(paddrTableSetups + setupIndex * 2) + (xf->roomId * 12) + xf->sliceId;
    return comboReadPhysI16(paddrTableRooms + roomIndex * 2) + xf->id;
}

static u16 bitPosLookupOot(const Xflag* xf)
{
    return bitPosLookupTables(xf, CUSTOM_XFLAG_TABLE_OOT_SCENES_ADDR, CUSTOM_XFLAG_TABLE_OOT_SETUPS_ADDR, CUSTOM_XFLAG_TABLE_OOT_ROOMS_ADDR);
}

static u16 bitPosLookupMm(const Xflag* xf)
{
    return bitPosLookupTables(xf, CUSTOM_XFLAG_TABLE_MM_SCENES_ADDR, CUSTOM_XFLAG_TABLE_MM_SETUPS_ADDR, CUSTOM_XFLAG_TABLE_MM_ROOMS_ADDR);
}

int comboXflagsGetOot(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return 1;
    bitPos = bitPosLookupOot(xf);
    return !!BITMAP8_GET(gSharedCustomSave.oot.xflags, bitPos);
}

int comboXflagsGetMm(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return 1;
    bitPos = bitPosLookupMm(xf);
    return !!BITMAP8_GET(gSharedCustomSave.mm.xflags, bitPos);
}

void comboXflagsSetOot(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return;
    bitPos = bitPosLookupOot(xf);
    BITMAP8_SET(gSharedCustomSave.oot.xflags, bitPos);
}

void comboXflagsSetMm(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return;
    bitPos = bitPosLookupMm(xf);
    BITMAP8_SET(gSharedCustomSave.mm.xflags, bitPos);
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
