#include <combo.h>
#include <combo/item.h>

static void removeButtonItem(u16 itemId)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 1; i < 4; ++i)
        {
            if (gSave.itemEquips.buttonItems[j][i] == itemId)
            {
                gSave.itemEquips.buttonItems[j][i] = ITEM_NONE;
            }
        }
    }
}

void comboRemoveTradeItem1(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gMmExtraTrade.trade1 & mask)
    {
        gMmExtraTrade.trade1 &= ~mask;
        if (gMmExtraTrade.trade1)
            comboToggleSlot(ITS_MM_TRADE1);
        else
            gSave.inventory.items[ITS_MM_TRADE1] = ITEM_NONE;
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
            comboToggleSlot(ITS_MM_TRADE2);
        else
            gSave.inventory.items[ITS_MM_TRADE2] = ITEM_NONE;
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
            comboToggleSlot(ITS_MM_TRADE3);
        else
            gSave.inventory.items[ITS_MM_TRADE3] = ITEM_NONE;
        removeButtonItem(kMmTrade3[xitemId]);
    }
}
