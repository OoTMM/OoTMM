#include <combo.h>
#include <combo/item.h>

void EnHs_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_COJIRO);
    comboGiveItemNpc(actor, play, gi, NPC_OOT_TRADE_ODD_MUSHROOM, a, b);
}

PATCH_CALL(0x80aa1104, EnHs_GiveItem);
PATCH_CALL(0x80aa1058, EnHs_GiveItem);

void EnHs_InitHook(Actor* actor)
{
    static const u32 maskCojiro = 1 << XITEM_OOT_ADULT_COJIRO;
    static const u32 maskOddPotion = 1 << XITEM_OOT_ADULT_ODD_POTION;
    u32 adultBits;

    adultBits = gOotExtraTrade.adult;
    if (((adultBits & maskOddPotion) || !(adultBits & maskCojiro)) && gSave.inventory.items[ITS_OOT_TRADE_ADULT] != ITEM_OOT_COJIRO)
        ActorDestroy(actor);
}
