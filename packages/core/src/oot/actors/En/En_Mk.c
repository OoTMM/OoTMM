#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x274) = (h); } while (0)

int EnMk_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
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

void EnMk_EyeballFrogHandler(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        this->attachedA = NULL;
        SET_HANDLER(this, actorAddr(0x14a, 0x80aacd6c));
        return;
    }

    EnMk_GiveItem(this, play, GI_OOT_EYE_DROPS, 9999.f, 9999.f);
}

PATCH_FUNC(0x80aacbd0, EnMk_EyeballFrogHandler);
