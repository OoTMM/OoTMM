#include <combo.h>
#include <combo/item.h>

void comboToggleTradeAdult(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_OOT_TRADE_ADULT, gOotExtraTrade.adult, kOotTradeAdult, 11);
    reloadSlotOot(gPlay, ITS_OOT_TRADE_ADULT);
}

void comboToggleTradeChild(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_OOT_TRADE_CHILD, gOotExtraTrade.child, kOotTradeChild, 12);
    reloadSlotOot(gPlay, ITS_OOT_TRADE_CHILD);
}

void comboToggleOcarina(void)
{
    u8* slot;

    slot = gSave.inventory.items + ITS_OOT_OCARINA;
    if (*slot == ITEM_OOT_OCARINA_FAIRY)
    {
        *slot = ITEM_OOT_OCARINA_TIME;
    }
    else
    {
        *slot = ITEM_OOT_OCARINA_FAIRY;
    }
    reloadSlotOot(gPlay, ITS_OOT_OCARINA);
}

void comboToggleHookshot(void)
{
    u8* slot;

    slot = gSave.inventory.items + ITS_OOT_HOOKSHOT;
    if (*slot == ITEM_OOT_HOOKSHOT)
    {
        *slot = ITEM_OOT_LONGSHOT;
    }
    else
    {
        *slot = ITEM_OOT_HOOKSHOT;
    }
    reloadSlotOot(gPlay, ITS_OOT_HOOKSHOT);
}

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
            comboToggleTradeAdult();
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
            comboToggleTradeChild();
        else
            gSave.inventory.items[ITS_OOT_TRADE_CHILD] = ITEM_NONE;
        removeButtonItem(kOotTradeChild[xitemId]);
    }
}
