#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

int GetItemCollectBehavior(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_STICK:
        if (gMmSave.inventory.items[ITS_MM_STICKS] != ITEM_NONE)
            return CB_PASSIVE;
        break;
    case ITEM_MM_NUT:
    case ITEM_MM_NUTS_5:
        if (gMmSave.inventory.items[ITS_MM_NUTS] != ITEM_NONE)
            return CB_PASSIVE;
        break;
    case ITEM_MM_BOMB:
    case ITEM_MM_ARROWS_10:
        return CB_PASSIVE;
    }
    return CB_GETITEM;
}
