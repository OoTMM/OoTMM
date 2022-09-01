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

int comboGiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    s16 absItemId;
    s32 override;

    override = -1;
    absItemId = itemId > 0 ? itemId : -itemId;

    switch (actor->id)
    {
    case AC_EN_BOX:
        override = comboGetChestOverride(play->sceneId, actor->variable & 0x1f);
        break;
    case AC_EN_MA1:
        absItemId = GI_OOT_CHICKEN;
        override = comboGetNpcOverride(0x00);
        break;
    case AC_EN_NIW_LADY:
        if (absItemId == GI_OOT_EMPTY_BOTTLE)
            override = comboGetNpcOverride(0x01);
        break;
    case AC_EN_DIVING_GAME:
        override = comboGetNpcOverride(0x03);
        break;
    case AC_EN_DU:
        if (absItemId == GI_OOT_GORON_BRACELET)
        {
            override = comboGetNpcOverride(0x02);
            gSave.eventsItem[(EV_OOT_ITEM_GORON_BRACELET & 0xf0) >> 4] |= (1 << (EV_OOT_ITEM_GORON_BRACELET & 0xf));
        }
        break;
    }

    if (override >= 0)
        absItemId = (s16)(override);
    absItemId = comboProgressiveChestItem(absItemId);
    itemId = itemId > 0 ? absItemId : -absItemId;
    return GiveItem(actor, play, itemId, a, b);
}
