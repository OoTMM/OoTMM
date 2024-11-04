#include <combo.h>
#include <combo/dma.h>
#include <combo/player.h>
#include <combo/mask.h>

ALIGNED(16) static u32 sIconHeader[2];

u32 GetItemTexture(u32 slotId, u8 item, u32 index);

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

void LoadIconItem24Oot(void* dst, int iconId)
{
    LoadFile(dst, 0x846000 + iconId * 0x900, 0x900);
}

void LoadIconItem24MM(void* dst, int iconId)
{
}

void LoadMmItemIcon(void* dst, int iconId)
{
    comboLoadMmIcon(dst, 0xa36c10, iconId);
}

void comboItemIcon(void* dst, int itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_MASK_BLAST:
        LoadMmItemIcon(dst, ITEM_MM_MASK_BLAST);
        break;
    case ITEM_OOT_MASK_STONE:
        LoadMmItemIcon(dst, ITEM_MM_MASK_STONE);
        break;
    default:
        LoadFile(dst, 0x7bd000 + itemId * 0x1000, 0x1000);
        break;
    }
}

static void LoadCustomItemIconSlot(PlayState* play, int slot, int isInit)
{
    Actor_Player* link;
    void* dst;
    u8 itemId;

    dst = (*(char**)((char*)&play->interfaceCtx + 0x138)) + 0x1000 * slot;
    itemId = gSave.equips.buttonItems[slot];

    if (slot == 0 && !isInit)
    {
        /* Masks can overwrite the B icon */
        link = GET_PLAYER(play);
        switch (link->mask)
        {
        case MASK_BLAST:
            comboItemIcon(dst, ITEM_OOT_MASK_BLAST);
            if (gBlastMaskDelayAcc)
                Grayscale(dst, 0x400);
            return;
        }
    }

    if (itemId == ITEM_OOT_SWORD_KOKIRI)
    {
        if (gSharedCustomSave.extraSwordsOot == 1)
        {
            LoadMmItemIcon(dst, ITEM_MM_SWORD_RAZOR);
            return;
        }

        if (gSharedCustomSave.extraSwordsOot == 2)
        {
            LoadMmItemIcon(dst, ITEM_MM_SWORD_GILDED);
            return;
        }
    }

    comboItemIcon(dst, itemId);
}

static void LoadCustomItemIcon(PlayState* play, int slot, int isInit)
{
    LoadCustomItemIconSlot(play, slot, 0);
}

PATCH_FUNC(0x8006fb50, LoadCustomItemIcon);
PATCH_FUNC(0x8006fc00, LoadCustomItemIcon);

static void LoadCustomItemIcon_B(void)
{
    LoadCustomItemIconSlot(gPlay, 0, 1);
}

PATCH_CALL(0x800e1dbc, LoadCustomItemIcon_B);

static void LoadCustomItemIcon_C_Left(void)
{
    LoadCustomItemIconSlot(gPlay, 1, 1);
}

PATCH_CALL(0x800e1e20, LoadCustomItemIcon_C_Left);

static void LoadCustomItemIcon_C_Down(void)
{
    LoadCustomItemIconSlot(gPlay, 2, 1);
}

PATCH_CALL(0x800e1e54, LoadCustomItemIcon_C_Down);

static void LoadCustomItemIcon_C_Right(void)
{
    LoadCustomItemIconSlot(gPlay, 3, 1);
}

PATCH_CALL(0x800e1e88, LoadCustomItemIcon_C_Right);

void LoadEquipItemTexture(void)
{
    u32 tex;
    PlayState* play;
    PauseContext* pauseCtx;

    play = gPlay;
    pauseCtx = &play->pauseCtx;
    tex = GetItemTexture(pauseCtx->equipTargetSlot, pauseCtx->equipTargetItem, 0);

    if (!tex) return;

    OPEN_DISPS(play->state.gfxCtx);
    gDPLoadTextureBlock(OVERLAY_DISP++, tex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    CLOSE_DISPS();
}

void Interface_UpdateButtonsPart2Wrapper(PlayState* play)
{
    u8* ptr;
    u8 itemId;
    u8 buttons[3];

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.equips.buttonItems[i + 1];
        itemId = *ptr;
        buttons[i] = itemId;

        switch (itemId)
        {
        case ITEM_OOT_MASK_BLAST:
        case ITEM_OOT_MASK_STONE:
            *ptr = ITEM_OOT_KEATON_MASK;
            break;
        }
    }

    Interface_UpdateButtonsPart2(play);

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.equips.buttonItems[i + 1];
        *ptr = buttons[i];
    }

    /* Fix for wrong tempB restores */
    if (EV_OOT_IS_SWORDLESS() && gSave.equips.buttonItems[0] == 0)
        gSave.equips.buttonItems[0] = ITEM_NONE;
}
