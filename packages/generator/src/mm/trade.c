#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>
#include "combo/common/Kaleido_Scope.h"

enum
{
    MM_TRADE1_COUNT = 6,
    MM_TRADE2_COUNT = 5,
    MM_TRADE3_COUNT = 5,
};

static void clearTradeButtons(u16 slot, u8 itemId)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 1; i < 4; ++i)
        {
            if (gSave.info.itemEquips.cButtonSlots[j][i] == slot ||
                gSave.info.itemEquips.buttonItems[j][i] == itemId)
            {
                gSave.info.itemEquips.buttonItems[j][i] = ITEM_NONE;
                gSave.info.itemEquips.cButtonSlots[j][i] = 0xff;
            }
        }
    }
    for (int age = 0; age < 2; ++age)
    {
        MmHumanAgeLoadout* equips = &gSharedCustomSave.mm.humanAgeLoadouts[age];

        for (int i = 1; i < 4; ++i)
        {
            if (equips->cButtonSlots[i] == slot ||
                equips->buttonItems[i] == itemId)
            {
                equips->buttonItems[i] = ITEM_NONE;
                equips->cButtonSlots[i] = 0xff;
            }
        }
    }
}

static void finishTradeRemoval(u16 slot, u8 removedItem, u32 remainingFlags, const u8* table, u32 tableSize)
{
    clearTradeButtons(slot, removedItem);

    if (!remainingFlags)
    {
        gSave.info.inventory.items[slot] = ITEM_NONE;
    }
    else if (gSave.info.inventory.items[slot] == removedItem)
    {
        gSave.info.inventory.items[slot] = comboGetNextTrade(removedItem, remainingFlags, table, tableSize);
    }

    reloadSlotMm(gPlay, slot);
}

void comboRemoveTradeItem1(u16 xitemId)
{
    u32 mask;

    if (xitemId >= MM_TRADE1_COUNT)
        return;

    mask = 1u << xitemId;
    if (!(gMmExtraTrade.trade1 & mask))
        return;

    gMmExtraTrade.trade1 &= ~mask;
    finishTradeRemoval(ITS_MM_TRADE1, kMmTrade1[xitemId], gMmExtraTrade.trade1, kMmTrade1, MM_TRADE1_COUNT);
}

void comboRemoveTradeItem2(u16 xitemId)
{
    u32 mask;

    if (xitemId >= MM_TRADE2_COUNT)
        return;

    mask = 1u << xitemId;
    if (!(gMmExtraTrade.trade2 & mask))
        return;

    gMmExtraTrade.trade2 &= ~mask;
    finishTradeRemoval(ITS_MM_TRADE2, kMmTrade2[xitemId], gMmExtraTrade.trade2, kMmTrade2, MM_TRADE2_COUNT);
}

void comboRemoveTradeItem3(u16 xitemId)
{
    u32 mask;

    if (xitemId >= MM_TRADE3_COUNT)
        return;

    mask = 1u << xitemId;
    if (!(gMmExtraTrade.trade3 & mask))
        return;

    gMmExtraTrade.trade3 &= ~mask;
    finishTradeRemoval(ITS_MM_TRADE3, kMmTrade3[xitemId], gMmExtraTrade.trade3, kMmTrade3, MM_TRADE3_COUNT);
}
