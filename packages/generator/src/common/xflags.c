#include <combo.h>
#include <combo/xflags.h>
#include <combo/item.h>
#include <combo/custom.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/io.h>

static u16 bitPosLookup(const Xflag* xf, u32 paddrTableScenes, u32 paddrTableSetups, u32 paddrTableRooms)
{
    u16 setupIndex;
    u16 roomIndex;

    setupIndex = IO_ReadPhysU16(paddrTableScenes + xf->sceneId * 2) + xf->setupId;
    roomIndex = IO_ReadPhysU16(paddrTableSetups + setupIndex * 2) + (xf->roomId * 12) + xf->sliceId;
    return IO_ReadPhysI16(paddrTableRooms + roomIndex * 2) + xf->id;
}

static u16 bitPosLookupOot(const Xflag* xf)
{
    static u32 paddrTableScenes;
    static u32 paddrTableSetups;
    static u32 paddrTableRooms;
    DmaEntry dmaEntry;

    /* Init */
    if (paddrTableScenes == 0)
    {
        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_OOT_SCENES_ADDR);
        paddrTableScenes = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_OOT_SETUPS_ADDR);
        paddrTableSetups = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_OOT_ROOMS_ADDR);
        paddrTableRooms = dmaEntry.pstart;
    }

    return bitPosLookup(xf, paddrTableScenes, paddrTableSetups, paddrTableRooms);
}

static u16 bitPosLookupMm(const Xflag* xf)
{
    static u32 paddrTableScenes;
    static u32 paddrTableSetups;
    static u32 paddrTableRooms;
    DmaEntry dmaEntry;

    /* Init */
    if (paddrTableScenes == 0)
    {
        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_MM_SCENES_ADDR);
        paddrTableScenes = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_MM_SETUPS_ADDR);
        paddrTableSetups = dmaEntry.pstart;

        comboDmaLookup(&dmaEntry, CUSTOM_XFLAG_TABLE_MM_ROOMS_ADDR);
        paddrTableRooms = dmaEntry.pstart;
    }

    return bitPosLookup(xf, paddrTableScenes, paddrTableSetups, paddrTableRooms);
}

int comboXflagsGetOot(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return 1;
    bitPos = bitPosLookupOot(xf);
    return BITMAP8_GET(gSharedCustomSave.oot.xflags, bitPos);
}

int comboXflagsGetMm(const Xflag* xf)
{
    u16 bitPos;

    if (xf->sceneId == 0xff)
        return 1;
    bitPos = bitPosLookupMm(xf);
    return BITMAP8_GET(gSharedCustomSave.mm.xflags, bitPos);
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

int comboXflagInit(Xflag* xf, Actor* actor, PlayState* play)
{
    if (g.xflagOverride)
    {
        memcpy(xf, &g.xflag, sizeof(*xf));
        return FALSE;
    }

    /* Check for an actor index of 0xff, meaning NO XFLAG */
    if (actor->actorIndex == 0xff)
    {
        Xflag_Clear(xf);
        return FALSE;
    }

    xf->sceneId = play->sceneId;
    xf->setupId = g.sceneSetupId;
    xf->roomId = actor->room;
    xf->sliceId = 0;
    xf->id = actor->actorIndex;

#if defined(GAME_OOT)
    if (xf->sceneId == SCE_OOT_GROTTOS)
    {
        switch (xf->roomId)
        {
        case 0x00:
            /* Generic grottos */
            xf->roomId = 0x20 | (gGrottoData & 0x1f);
            break;
        }
    }
#endif

#if defined(GAME_MM)
    if (xf->sceneId == SCE_MM_GROTTOS)
    {
        switch (xf->roomId)
        {
        case 0x04:
            /* Generic Grottos */
            xf->roomId = 0x20 | (gGrottoData & 0x1f);
            break;
        case 0x0a:
            /* Cow grottos */
            if (gLastScene == SCE_MM_GREAT_BAY_COAST)
                xf->roomId = 0x0f;
            break;
        }
    }
#endif

    return TRUE;
}

int Xflag_IsValid(Xflag* xf)
{
    ComboItemOverride o;

    /* Fast check */
    if (xf->sceneId == 0xff)
        return FALSE;

    comboXflagItemOverride(&o, xf, GI_NONE);
    if (o.gi == GI_NONE)
    {
        xf->sceneId = 0xff;
        return FALSE;
    }

    return TRUE;
}

int Xflag_IsShuffled(Xflag* xf)
{
    return Xflag_IsValid(xf) && !comboXflagsGet(xf);
}

void Xflag_Clear(Xflag* xf)
{
    xf->sceneId = 0xff;
    xf->setupId = 0;
    xf->roomId = 0;
    xf->sliceId = 0;
    xf->id = 0;
}
