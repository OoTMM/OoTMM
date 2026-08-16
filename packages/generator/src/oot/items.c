#include <combo.h>

void Item_AddSticksOot(int count);
void Item_AddNutsOot(int count);

u8 _Item_CheckObtainability(s16 itemId);
u8 _Item_Give(PlayState* play, u8 itemId);

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

u8 Item_GiveHook(PlayState* play, u8 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
        Item_AddSticksOot(1);
        return 0xff;
    case ITEM_OOT_STICKS_5:
        Item_AddSticksOot(5);
        return 0xff;
    case ITEM_OOT_STICKS_10:
        Item_AddSticksOot(10);
        return 0xff;
    case ITEM_OOT_NUT:
        Item_AddNutsOot(1);
        return 0xff;
    case ITEM_OOT_NUTS_5:
        Item_AddNutsOot(5);
        return 0xff;
    case ITEM_OOT_NUTS_10:
        Item_AddNutsOot(10);
        return 0xff;
    }

    return _Item_Give(play, itemId);
}
