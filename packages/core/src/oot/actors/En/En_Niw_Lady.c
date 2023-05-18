#include <combo.h>

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
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    switch (gi)
    {
    case GI_OOT_BOTTLE_EMPTY:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ANJU_BOTTLE, gi);
        break;
    case GI_OOT_POCKET_EGG:
    case GI_OOT_POCKET_CUCCO:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_POCKET_EGG, gi);
        break;
    case GI_OOT_COJIRO:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_COJIRO, gi);
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POCKET_CUCCO);
        break;
    }
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a9e95c, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9e9a0, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9ec70, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9edc4, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9eea8, EnNiwLady_GiveItem);
