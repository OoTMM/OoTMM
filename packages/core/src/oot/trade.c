#include <combo.h>
#include <combo/item.h>

static void removeButtonItem(u16 itemId)
{
    for (int i = 1; i < 4; ++i)
    {
        if (gSave.equips.buttonItems[i] == itemId)
        {
            gSave.equips.buttonItems[i] = ITEM_NONE;
            gSave.equips.cButtonSlots[i - 1] = 0xff;
        }
    }
}

void comboRemoveTradeItemAdult(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gOotExtraTrade.adult & mask)
    {
        gOotExtraTrade.adult &= ~mask;
        if (gOotExtraTrade.adult)
            comboToggleSlot(ITS_OOT_TRADE_ADULT);
        else
            gSave.inventory.items[ITS_OOT_TRADE_ADULT] = ITEM_NONE;
        removeButtonItem(kOotTradeAdult[xitemId]);
    }
}

void comboRemoveTradeItemChild(u16 xitemId)
{
    u32 mask;

    mask = 1 << xitemId;
    if (gOotExtraTrade.child & mask)
    {
        gOotExtraTrade.child &= ~mask;
        if (gOotExtraTrade.child)
            comboToggleSlot(ITS_OOT_TRADE_CHILD);
        else
            gSave.inventory.items[ITS_OOT_TRADE_CHILD] = ITEM_NONE;
        removeButtonItem(kOotTradeChild[xitemId]);
    }
}
