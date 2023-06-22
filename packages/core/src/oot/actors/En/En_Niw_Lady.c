#include <combo.h>
#include <combo/item.h>

int EnNiwLady_GetActiveItem(GameState_Play* play)
{
    int itemAction;

    itemAction = GetActiveItem(play);
    if (itemAction == 6 && !BITMAP16_GET(gSave.eventsItem, EV_OOT_ITEM_ANJU_POCKET_EGG))
    {
        itemAction = -1;
    }
    return itemAction;
}

PATCH_CALL(0x80a9eb04, EnNiwLady_GetActiveItem);

void EnNiwLady_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    switch (gi)
    {
    case GI_OOT_BOTTLE_EMPTY:
        npc = NPC_OOT_ANJU_BOTTLE;
        break;
    case GI_OOT_POCKET_EGG:
    case GI_OOT_POCKET_CUCCO:
        npc = NPC_OOT_TRADE_POCKET_EGG;
        break;
    case GI_OOT_COJIRO:
        npc = NPC_OOT_TRADE_COJIRO;
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POCKET_CUCCO);
        break;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80a9e95c, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9e9a0, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9ec70, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9edc4, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9eea8, EnNiwLady_GiveItem);
