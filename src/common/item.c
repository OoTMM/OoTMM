#include <combo.h>

#if defined(GAME_OOT)
# define ITEM_BASE_MM 0x100
#endif

#if defined(GAME_MM)
# define ITEM_BASE_MM 0x0
#endif

static void addItemOot(u16 itemId)
{
    gOotSave.inventory[0] = ITEM_OOT_OCARINA_OF_TIME;
}

static void addItemMm(u16 itemId)
{
    gOotSave.inventory[0] = ITEM_OOT_MASK_OF_TRUTH;
}

int comboAddItem(void* play, u16 itemId)
{
    if ((itemId & 0x100) == ITEM_BASE_MM)
        addItemMm(itemId & 0xff);
    else
        addItemOot(itemId & 0xff);

    return -1;
}

int comboAddItemFromChest(void* play, s16 chestId)
{
    u16 itemId;

    chestId--;
    itemId = kExtendedGetItems[chestId].item;
    itemId |= (chestId & 0x100);
    return comboAddItem(play, itemId);
}
