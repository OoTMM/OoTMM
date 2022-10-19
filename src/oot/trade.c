#include <combo.h>

static void toggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize)
{
    int bitPos;
    /* We need to get the bit index of the current item */

    bitPos = -1;
    for (u32 i = 0; i < tableSize; ++i)
    {
        if (*slot == table[i])
        {
            bitPos = i;
            break;
        }
    }
    if (bitPos == -1)
    {
        *slot = ITEM_NONE;
        return;
    }
    for (;;)
    {
        bitPos++;
        if (bitPos >= tableSize)
            bitPos = 0;
        if (flags & (1 << bitPos))
        {
            *slot = table[bitPos];
            break;
        }
    }
}

void comboToggleTradeAdult(void)
{
    toggleTrade(gSave.inventory + ITS_OOT_TRADE_ADULT, gOotExtraTrade.adult, kOotTradeAdult, 10);
}

void comboToggleTradeChild(void)
{
    toggleTrade(gSave.inventory + ITS_OOT_TRADE_CHILD, gOotExtraTrade.child, kOotTradeChild, 11);
}

void comboToggleOcarina(void)
{
    u8* slot;

    slot = gOotSave.inventory + ITS_OOT_OCARINA;
    if (*slot == ITEM_OOT_OCARINA_FAIRY)
    {
        *slot = ITEM_OOT_OCARINA_TIME;
    }
    else
    {
        *slot = ITEM_OOT_OCARINA_FAIRY;
    }
}

void comboToggleHookshot(void)
{
    u8* slot;

    slot = gOotSave.inventory + ITS_OOT_HOOKSHOT;
    if (*slot == ITEM_OOT_HOOKSHOT)
    {
        *slot = ITEM_OOT_LONGSHOT;
    }
    else
    {
        *slot = ITEM_OOT_HOOKSHOT;
    }
}

static void removeButtonItem(u16 itemId)
{
    for (int i = 1; i < 4; ++i)
    {
        if (gSave.buttons[i] == itemId)
        {
            gSave.buttons[i] = ITEM_NONE;
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
            gSave.inventory[ITS_OOT_TRADE_ADULT] = ITEM_NONE;
        removeButtonItem(kOotTradeAdult[xitemId]);
    }
}
