#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

int GetItemCollectBehavior(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
        if (gOotSave.upgrades.dekuStick)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_NUT:
    case ITEM_OOT_NUTS_5:
        if (gOotSave.upgrades.dekuNut)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_BOMB:
    case ITEM_OOT_ARROWS_5:
    case ITEM_OOT_SEEDS_5:
        return CB_PASSIVE;
    }
    return CB_GETITEM;
}
