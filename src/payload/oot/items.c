#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

int GetItemCollectBehavior(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_OOT_DEKU_STICK:
        if (gOotSave.upgrades.dekuStick)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_DEKU_NUT:
    case ITEM_OOT_DEKU_NUTS_5:
        if (gOotSave.upgrades.dekuNut)
            return CB_PASSIVE;
        break;
    case ITEM_OOT_BOMB:
    case ITEM_OOT_ARROWS_5:
    case ITEM_OOT_DEKU_SEEDS_5:
        return CB_PASSIVE;
    }
    return CB_GETITEM;
}

int hook_SetChestItemInRange(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    s32 override;
    if (actor->id == 0x0a)
    {
        override = comboGetChestOverride(play->sceneId, actor->variable & 0x1f);
        if (override >= 0)
            itemId = (s16)(-override);
    }
    return SetChestItemInRange(actor, play, itemId, a, b);
}
