#include <combo.h>

u8 _Item_CheckObtainability(s16 itemId);

u8 Item_CheckObtainability(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_STICK:
    case ITEM_MM_STICKS_20:
    case ITEM_MM_STICKS_30:
    case ITEM_MM_NUT:
    case ITEM_MM_NUTS_5:
    case ITEM_MM_NUTS_10:
    case ITEM_MM_NUTS_30:
    case ITEM_MM_NUTS_40:
        return (u8)itemId;
    }

    return _Item_CheckObtainability(itemId);
}

PATCH_FUNC(0x80114978, Item_CheckObtainability);
