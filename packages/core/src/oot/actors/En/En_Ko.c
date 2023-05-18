#include <combo.h>

void EnKo_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_POACHER_SAW, gi);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_ODD_POTION);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80ad43ec, EnKo_GiveItem);

void EnKo_SpawnHook(Actor* this)
{
    /* Check for Lost Woods */
    switch (gSave.entrance)
    {
    case 0x11e:
    case 0x1a9:
    case 0x1b1:
    case 0x4d6:
    case 0x4da:
    case 0x4de:
    case 0x5e0:
        break;
    default:
        ActorDestroy(this);
        return;
    }

    /* Check for Odd Potion */
    if (!(gOotExtraTrade.adult & (1 << XITEM_OOT_ADULT_ODD_POTION)))
        ActorDestroy(this);
}

PATCH_CALL(0x80ad3ea0, EnKo_SpawnHook);
