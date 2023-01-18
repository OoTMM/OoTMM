#include <combo.h>

#if defined(GAME_OOT)
# define ITEM_BASE_MM           MASK_FOREIGN_ITEM
# define comboAddItemNative     comboAddItemOot
# define comboAddItemForeign    comboAddItemMm
#else
# define ITEM_BASE_MM 0x0
# define comboAddItemNative     comboAddItemMm
# define comboAddItemForeign    comboAddItemOot
#endif

const u8 kMaxSticks[] = { 0, 10, 20, 30 };
const u8 kMaxNuts[] = { 0, 20, 30, 40 };
const u8 kMaxBombs[] = { 0, 20, 30, 40 };
const u8 kMaxArrows[] = { 0, 30, 40, 50 };
const u8 kMaxSeeds[] = { 0, 30, 40, 50 };
const u16 kMaxRupees[] = { 99, 200, 500 };

int comboAddItem(GameState_Play* play, s16 gi)
{
    u16 itemId;

    gi--;
    itemId = kExtendedGetItems[gi].itemId;
    if (gi & MASK_FOREIGN_GI)
    {
        itemId |= MASK_FOREIGN_ITEM;
        comboAddItemForeign(itemId & 0xff, 1);
    }
    else
    {
        comboAddItemNative(itemId, 0);
        comboAddItemEffect(play, itemId);
    }

    comboTextHijackItem(play, itemId);
    return -1;
}

int comboAddItemNoEffect(s16 gi)
{
    u16 itemId;

    gi--;
    itemId = kExtendedGetItems[gi].itemId;
    if (gi & MASK_FOREIGN_GI)
    {
        itemId |= MASK_FOREIGN_ITEM;
        comboAddItemForeign(itemId & 0xff, 1);
    }
    else
    {
        comboAddItemNative(itemId, 1);
    }
    return -1;
}
