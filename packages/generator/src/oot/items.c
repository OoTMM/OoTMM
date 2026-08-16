#include <combo.h>

u8 _Item_CheckObtainability(s16 itemId);

u8 Item_CheckObtainabilityHook(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
    case ITEM_OOT_STICKS_5:
    case ITEM_OOT_STICKS_10:
    case ITEM_OOT_NUT:
    case ITEM_OOT_NUTS_5:
    case ITEM_OOT_NUTS_10:
        return (u8)itemId;
    }

    return _Item_CheckObtainability(itemId);
}
