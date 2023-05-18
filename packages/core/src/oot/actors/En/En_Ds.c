#include <combo.h>

void EnDs_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_ODD_POTION, gi);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_ODD_MUSHROOM);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80aabcec, EnDs_GiveItem);
PATCH_CALL(0x80aabc6c, EnDs_GiveItem);
