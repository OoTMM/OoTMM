#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/inventory.h>

int EnNiwLady_GetActiveItem(PlayState* play)
{
    int itemAction;

    itemAction = Actor_GetPlayerExchangeItemId(play);
    if (itemAction == 6 && !BITMAP16_GET(gSave.info.eventsItem, EV_OOT_ITEM_ANJU_POCKET_EGG))
    {
        itemAction = -1;
    }
    return itemAction;
}

PATCH_CALL(0x80a9eb04, EnNiwLady_GetActiveItem);

void EnNiwLady_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (!(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
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

PATCH_CALL(0x80a9ec70, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9edc4, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9eea8, EnNiwLady_GiveItem);

static void EnNiwLady_GiveItemInit(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    actor->parent = NULL;
    EnNiwLady_GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a9e95c, EnNiwLady_GiveItemInit);
PATCH_CALL(0x80a9e9a0, EnNiwLady_GiveItemInit);
