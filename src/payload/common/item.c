#include <combo.h>

#if defined(GAME_OOT)
# define ITEM_BASE_MM MASK_FOREIGN_ITEM
#else
# define ITEM_BASE_MM 0x0
#endif

const u8 kMaxSticks[] = { 0, 10, 20, 30 };
const u8 kMaxNuts[] = { 0, 20, 30, 40 };
const u8 kMaxBombs[] = { 0, 20, 30, 40 };
const u8 kMaxArrows[] = { 0, 30, 40, 50 };
const u8 kMaxSeeds[] = { 0, 30, 40, 50 };
const u16 kMaxRupees[] = { 99, 200, 500 };

void comboAddItem(u16 itemId)
{
    if ((itemId & MASK_FOREIGN_ITEM) == ITEM_BASE_MM)
        comboAddItemMm(itemId & 0xff);
    else
        comboAddItemOot(itemId & 0xff);
}

int comboAddItemFromChest(void* play, s16 chestId)
{
    u16 itemId;

    chestId--;
    itemId = kExtendedGetItems[chestId].itemId;
    if (chestId & MASK_FOREIGN_GI)
        itemId |= MASK_FOREIGN_ITEM;

    comboTextHijackItem(play, itemId);
    comboAddItem(itemId);
    return -1;
}
