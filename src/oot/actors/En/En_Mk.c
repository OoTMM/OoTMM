#include <combo.h>

int EnMk_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(LINK.state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    switch (gi)
    {
    case GI_OOT_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_LABORATORY_DIVE, gi);
        break;
    case GI_OOT_EYE_DROPS:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_EYE_DROPS, gi);
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_EYEBALL_FROG);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80aac7e4, EnMk_GiveItem);
PATCH_CALL(0x80aac840, EnMk_GiveItem);
PATCH_CALL(0x80aacce8, EnMk_GiveItem);
PATCH_CALL(0x80aacd48, EnMk_GiveItem);
