#include <combo.h>

int EnMa1_GiveItem_Egg(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    itemId = comboOverride(OV_NPC, 0, NPC_OOT_MALON_EGG, itemId);
    return GiveItem(actor, play, itemId, a, b);
}

PATCH_CALL(0x809f1288, &EnMa1_GiveItem_Egg);
