#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

s16 EnExItem_Reward(Actor* actor);
s16 EnExItem_RewardByIndex(int index);

int GetItemCollectBehavior(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_STICK:
        if (gOotSave.upgrades.dekuStick)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_NUT:
    case ITEM_OOT_NUTS_5:
        if (gOotSave.upgrades.dekuNut)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_BOMB:
    case ITEM_OOT_ARROWS_5:
    case ITEM_OOT_SEEDS_5:
        return CB_PASSIVE;
    }
    return CB_GETITEM;
}

int comboGiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    switch (actor->id)
    {
    case AC_EN_BOX:
        itemId = comboOverride(OV_CHEST, play->sceneId, actor->variable & 0x1f, (actor->variable >> 5) & 0xff);
        break;
    case AC_EN_MA1:
        itemId = comboOverride(OV_NPC, 0, 0x00, GI_OOT_CHICKEN);
        break;
    case AC_EN_NIW_LADY:
        if (itemId == GI_OOT_EMPTY_BOTTLE)
            itemId = comboOverride(OV_NPC, 0, 0x01, GI_OOT_EMPTY_BOTTLE);
        break;
    case AC_EN_DIVING_GAME:
        itemId = comboOverride(OV_NPC, 0, 0x03, GI_OOT_SCALE_SILVER);
        break;
    case AC_EN_DU:
        if (itemId == GI_OOT_GORON_BRACELET)
        {
            itemId = comboOverride(OV_NPC, 0, 0x02, GI_OOT_GORON_BRACELET);
            gSave.eventsItem[(EV_OOT_ITEM_GORON_BRACELET & 0xf0) >> 4] |= (1 << (EV_OOT_ITEM_GORON_BRACELET & 0xf));
        }
        break;
    case AC_EN_BOM_BOWL_PIT:
        itemId = EnExItem_RewardByIndex(*(u16*)((char*)actor + 0x14a));
        break;
    case AC_EN_EX_ITEM:
        itemId = EnExItem_Reward(actor);
        break;
    case AC_EN_GO2:
        switch (itemId)
        {
        case GI_OOT_TUNIC_GORON:
            break;
        }
        break;
    }

    return GiveItem(actor, play, itemId, a, b);
}
