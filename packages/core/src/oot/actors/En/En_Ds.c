#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/inventory.h>

void EnDs_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    if (!(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_ODD_MUSHROOM);
    comboGiveItemNpc(actor, play, gi, NPC_OOT_TRADE_ODD_POTION, a, b);
}

PATCH_CALL(0x80aabcec, EnDs_GiveItem);
PATCH_CALL(0x80aabc6c, EnDs_GiveItem);
