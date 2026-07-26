#include <combo.h>

void EnPoField_Item_Give(PlayState *play, u8 itemId)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
        {
            gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] = itemId;
            reloadSlotOot(play, ITS_OOT_BOTTLE + i);
            return;
        }
    }

    if (gOotExtraTrade.adult & (1 << XITEM_OOT_ADULT_BOTTLE) && gOotExtraItems.bottleAdultSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        gOotExtraItems.bottleAdultSlot = itemId;
        if (gOotSave.info.inventory.items[ITS_OOT_TRADE_ADULT] == ITEM_OOT_BOTTLE_EMPTY)
        {
            gOotSave.info.inventory.items[ITS_OOT_TRADE_ADULT] = gOotExtraItems.bottleAdultSlot;
            reloadSlotOot(play, ITS_OOT_TRADE_ADULT);
        }
        return;
    }

    if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BOTTLE) && gOotExtraItems.bottleChildSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        gOotExtraItems.bottleChildSlot = itemId;
        if (gOotSave.info.inventory.items[ITS_OOT_TRADE_CHILD] == ITEM_OOT_BOTTLE_EMPTY)
        {
            gOotSave.info.inventory.items[ITS_OOT_TRADE_CHILD] = gOotExtraItems.bottleChildSlot;
            reloadSlotOot(play, ITS_OOT_TRADE_CHILD);
        }
        return;
    }

    return;
}

PATCH_CALL(0x80af9e58, EnPoField_Item_Give);
PATCH_CALL(0x80af9e70, EnPoField_Item_Give);
