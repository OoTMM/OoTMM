#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>

static void removeButtonItem(u16 itemId)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 1; i < 4; ++i)
        {
            if (gSave.info.itemEquips.buttonItems[j][i] == itemId)
            {
                gSave.info.itemEquips.buttonItems[j][i] = ITEM_NONE;
            }
        }
    }
}

static void removeCurrentCButtonsForSlot(u16 slot)
{
    for (int button = 1; button < 4; ++button)
    {
        if (gMmSave.info.itemEquips.cButtonSlots[0][button] == slot)
        {
            gMmSave.info.itemEquips.buttonItems[0][button] = ITEM_NONE;
            gMmSave.info.itemEquips.cButtonSlots[0][button] = 0xff;
        }
    }
}

static void checkTradeSlotAgeRequirement(u16 slot)
{
    u8 item;

    item = gSave.info.inventory.items[slot];

    if (!KaleidoScope_CheckMmItemAgeReq(item))
        removeCurrentCButtonsForSlot(slot);
}

void comboRemoveTradeItem1(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gMmExtraTrade.trade1 & mask)
    {
        gMmExtraTrade.trade1 &= ~mask;
        if (gMmExtraTrade.trade1)
        {
            comboToggleSlot(ITS_MM_TRADE1);
            checkTradeSlotAgeRequirement(ITS_MM_TRADE1);
        }
        else
            gSave.info.inventory.items[ITS_MM_TRADE1] = ITEM_NONE;
        removeButtonItem(kMmTrade1[xitemId]);
    }
}

void comboRemoveTradeItem2(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gMmExtraTrade.trade2 & mask)
    {
        gMmExtraTrade.trade2 &= ~mask;
        if (gMmExtraTrade.trade2)
        {
            comboToggleSlot(ITS_MM_TRADE2);
            checkTradeSlotAgeRequirement(ITS_MM_TRADE2);
        }
        else
            gSave.info.inventory.items[ITS_MM_TRADE2] = ITEM_NONE;
        removeButtonItem(kMmTrade2[xitemId]);
    }
}

void comboRemoveTradeItem3(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gMmExtraTrade.trade3 & mask)
    {
        gMmExtraTrade.trade3 &= ~mask;
        if (gMmExtraTrade.trade3)
        {
            comboToggleSlot(ITS_MM_TRADE3);
            checkTradeSlotAgeRequirement(ITS_MM_TRADE3);
        }
        else
            gSave.info.inventory.items[ITS_MM_TRADE3] = ITEM_NONE;
        removeButtonItem(kMmTrade3[xitemId]);
    }
}
