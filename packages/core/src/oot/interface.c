#include <combo.h>
#include <combo/dma.h>

ALIGNED(16) static u32 sIconHeader[2];

static void lookupMmIcon(u32 pbase, int index, u32* outAddr, u32* outSize)
{
    u32 unk;

    DMARomToRam(pbase | PI_DOM1_ADDR2, sIconHeader, 4);
    unk = sIconHeader[0];
    if (index == 0)
    {
        DMARomToRam((pbase + 4) | PI_DOM1_ADDR2, sIconHeader, 4);
        *outAddr = pbase + unk;
        *outSize = sIconHeader[0];
    }
    else
    {
        DMARomToRam((pbase + index * 4) | PI_DOM1_ADDR2, sIconHeader, 8);
        *outAddr = pbase + sIconHeader[0] + unk;
        *outSize = sIconHeader[1] - sIconHeader[0];
    }
}

static void LoadMmIcon(void* dst, int iconId)
{
    DmaEntry entry;
    u32 iconAddr;
    u32 iconSize;

    if (!comboDmaLookup(&entry, 0xa36c10 | VROM_FOREIGN_OFFSET))
        return;
    lookupMmIcon(entry.pstart, iconId, &iconAddr, &iconSize);
    if (iconSize)
        DmaCompressed(iconAddr, dst, iconSize);
}

static void LoadCustomIcon(void* dst, int itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_MASK_BLAST:
        LoadMmIcon(dst, 0x47);
        break;
    default:
        LoadFile(dst, 0x7bd000 + itemId * 0x1000, 0x1000);
        break;
    }
}

static void LoadCustomItemIconSlot(GameState_Play* play, int slot)
{
    void* dst;

    dst = (*(char**)((char*)&play->interfaceCtx + 0x138)) + 0x1000 * slot;
    LoadCustomIcon(dst, gSave.equips.buttonItems[slot]);
}

PATCH_FUNC(0x8006fb50, LoadCustomItemIconSlot);

static void LoadCustomItemIcon_C_Left(void)
{
    LoadCustomItemIconSlot(gPlay, 1);
}

PATCH_CALL(0x800e1e20, LoadCustomItemIcon_C_Left);

static void LoadCustomItemIcon_C_Down(void)
{
    LoadCustomItemIconSlot(gPlay, 2);
}

PATCH_CALL(0x800e1e54, LoadCustomItemIcon_C_Down);

static void LoadCustomItemIcon_C_Right(void)
{
    LoadCustomItemIconSlot(gPlay, 3);
}

PATCH_CALL(0x800e1e88, LoadCustomItemIcon_C_Right);
