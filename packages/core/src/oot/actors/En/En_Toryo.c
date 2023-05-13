#include <combo.h>

void EnToryo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_BROKEN_GORON_SWORD, gi);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POACHER_SAW);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a8c31c, EnToryo_GiveItem);
