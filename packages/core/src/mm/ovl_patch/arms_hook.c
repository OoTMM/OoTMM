#include <combo.h>
#include <combo/player.h>

void ArmsHook_SetTimerAndParent(Actor_ArmsHook* this, GameState_Play* play)
{
    Actor_Player* player = GET_PLAYER(play);
    this->actor.parent = &player->actor;

    /* should use the ItemId enum */
    /* 0x0F - ITEM_HOOKSHOT */
    /* 0x11 - ITEM_OOT_HOOKSHOT */
    this->timer = player->heldItemId == 0xF ? 26 : 13;
}
