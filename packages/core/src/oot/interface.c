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

void comboLoadMmIcon(void* dst, u32 iconBank, int iconId)
{
    DmaEntry entry;
    u32 iconAddr;
    u32 iconSize;

    if (!comboDmaLookup(&entry, iconBank | VROM_FOREIGN_OFFSET))
        return;
    lookupMmIcon(entry.pstart, iconId, &iconAddr, &iconSize);
    if (iconSize)
        DmaCompressed(iconAddr, dst, iconSize);
}

static void LoadMmItemIcon(void* dst, int iconId)
{
    comboLoadMmIcon(dst, 0xa36c10, iconId);
}

void comboItemIcon(void* dst, int itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_MASK_BLAST:
        LoadMmItemIcon(dst, 0x47);
        break;
    default:
        LoadFile(dst, 0x7bd000 + itemId * 0x1000, 0x1000);
        break;
    }
}

static void grayscale(void* dst, unsigned size)
{
    static const float rev = 1.f / 255.f;
    Color_RGBA8* pixels;
    Color_RGBA8* p;
    float acc;
    u8 v;

    pixels = dst;
    size /= 4;

    for (int i = 0; i < size; i++)
    {
        p = &pixels[i];
        acc = 0.f;
        acc += (p->r * rev) * 0.33f;
        acc += (p->g * rev) * 0.33f;
        acc += (p->b * rev) * 0.33f;
        if (acc > 1.f)
            acc = 1.f;
        v = (u8)(acc * 255.f);
        p->r = v;
        p->g = v;
        p->b = v;
    }
}

static void LoadCustomItemIconSlot(GameState_Play* play, int slot)
{
    Actor_Player* link;
    void* dst;
    u8 itemId;

    dst = (*(char**)((char*)&play->interfaceCtx + 0x138)) + 0x1000 * slot;
    itemId = gSave.equips.buttonItems[slot];

    if (slot == 0)
    {
        /* Masks can overwrite the B icon */
        link = GET_LINK(play);
        switch (link->mask)
        {
        case 9:
            comboItemIcon(dst, ITEM_OOT_MASK_BLAST);
            if (gBlastMaskDelayAcc)
                grayscale(dst, 0x1000);
            return;
        }
    }

    comboItemIcon(dst, itemId);
}

PATCH_FUNC(0x8006fb50, LoadCustomItemIconSlot);
PATCH_FUNC(0x8006fc00, LoadCustomItemIconSlot);

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
