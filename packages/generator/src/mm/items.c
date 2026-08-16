#include <combo.h>

void Item_AddSticksMm(int count);
void Item_AddNutsMm(int count);

u8 _Item_CheckObtainability(s16 itemId);
u8 _Item_Give(PlayState* play, u8 itemId);

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


u8 Item_GiveHook(PlayState* play, u8 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_STICK:
        Item_AddSticksMm(1);
        return 0xff;
    case ITEM_MM_STICKS_20:
        Item_AddSticksMm(20);
        return 0xff;
    case ITEM_MM_STICKS_30:
        Item_AddSticksMm(30);
        return 0xff;
    case ITEM_MM_NUT:
        Item_AddNutsMm(1);
        return 0xff;
    case ITEM_MM_NUTS_5:
        Item_AddNutsMm(5);
        return 0xff;
    case ITEM_MM_NUTS_10:
        Item_AddNutsMm(10);
        return 0xff;
    case ITEM_MM_NUTS_30:
        Item_AddNutsMm(30);
        return 0xff;
    case ITEM_MM_NUTS_40:
        Item_AddNutsMm(40);
        return 0xff;
    }

    return _Item_Give(play, itemId);
}
