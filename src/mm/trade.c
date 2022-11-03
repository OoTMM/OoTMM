#include <combo.h>

void comboToggleTrade1(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE1, gMmExtraTrade.trade1, kMmTrade1, 5);
}

void comboToggleTrade2(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE2, gMmExtraTrade.trade2, kMmTrade2, 2);
}

void comboToggleTrade3(void)
{
    comboToggleTrade(gSave.inventory.items + ITS_MM_TRADE3, gMmExtraTrade.trade3, kMmTrade3, 2);
}
