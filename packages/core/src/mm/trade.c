#include <combo.h>
#include <combo/item.h>

void comboToggleTrade1(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE1, gMmExtraTrade.trade1, kMmTrade1, 6);
    reloadSlotMm(gPlay, ITS_MM_TRADE1);
}

void comboToggleTrade2(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE2, gMmExtraTrade.trade2, kMmTrade2, 5);
    reloadSlotMm(gPlay, ITS_MM_TRADE2);
}

void comboToggleTrade3(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE3, gMmExtraTrade.trade3, kMmTrade3, 5);
    reloadSlotMm(gPlay, ITS_MM_TRADE3);
}

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
            comboToggleTrade1();
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
            comboToggleTrade2();
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
            comboToggleTrade3();
        else
            gSave.inventory.items[ITS_MM_TRADE3] = ITEM_NONE;
        removeButtonItem(kMmTrade3[xitemId]);
    }
}

void comboToggleHookshot(void)
{
    u8* slot;

    slot = gSave.inventory.items + ITS_MM_HOOKSHOT;
    if (*slot == ITEM_MM_HOOKSHOT)
    {
        *slot = 0x11; // ITEM_MM_BOTTLE_POTION_RED but that enum is wrong
    }
    else
    {
        *slot = ITEM_MM_HOOKSHOT;
    }
    reloadSlotMm(gPlay, ITS_MM_HOOKSHOT);
}

void comboToggleOcarina(void)
{
    u8* slot;

    slot = gSave.inventory.items + ITS_MM_OCARINA;
    if (*slot == ITEM_MM_OCARINA_OF_TIME)
    {
        *slot = ITEM_MM_OCARINA_FAIRY;
    }
    else
    {
        *slot = ITEM_MM_OCARINA_OF_TIME;
    }
    reloadSlotMm(gPlay, ITS_MM_OCARINA);
}
