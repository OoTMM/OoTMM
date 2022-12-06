#include <combo.h>

int EnZk_HasGiveItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        this->attachedA = NULL;
        gOotExtraFlags.tunicZora = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80ad6c5c, EnZk_HasGiveItem);

int EnZk_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    if (!gOotExtraFlags.tunicZora)
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ZORA_KING_TUNIC, GI_OOT_TUNIC_ZORA);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_EYEBALL_FROG, GI_OOT_EYEBALL_FROG);
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_PRESCRIPTION);
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80ad6cc8, EnZk_GiveItem);
