#include <combo.h>

int EnTa_GetActiveItem_Chicken(Actor* this, GameState_Play* play)
{
    int item;

    item = GetActiveItem(play);
    if (item == 3)
        comboRemoveTradeItemChild(XITEM_OOT_CHILD_CHICKEN);
    return item;
}

PATCH_CALL(0x809223d4, EnTa_GetActiveItem_Chicken);

void EnTa_GiveItem_MilkBottle(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_TALON_BOTTLE, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80923cd8, EnTa_GiveItem_MilkBottle);
PATCH_CALL(0x80923d7c, EnTa_GiveItem_MilkBottle);
