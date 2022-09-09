#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

int GetItemCollectBehavior(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_STICK:
        if (gMmSave.inventory.items[ITS_MM_STICKS] != ITEM_NONE)
            return CB_PASSIVE;
        break;
    case ITEM_MM_NUT:
    case ITEM_MM_NUTS_5:
        if (gMmSave.inventory.items[ITS_MM_NUTS] != ITEM_NONE)
            return CB_PASSIVE;
        break;
    case ITEM_MM_BOMB:
    case ITEM_MM_ARROWS_10:
        return CB_PASSIVE;
    }
    return CB_GETITEM;
}


int comboGiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    s32 override;
    u16 absItemId;

    override = -1;
    absItemId = itemId > 0 ? itemId : -itemId;

    switch (itemId)
    {
    case GI_MM_BOTTLED_POTION_RED:
        override = comboGetNpcOverride(0x00);
        break;
    case GI_MM_PICTOGRAPH_BOX:
        override = comboGetNpcOverride(0x01);
        break;
    case GI_MM_SONG_AWAKENING:
        override = comboGetNpcOverride(0x02);
        break;
    }

    switch (actor->id)
    {
    case AC_EN_BOX:
        override = comboGetChestOverride(play->sceneId, actor->variable & 0x1f);
        break;
    }

    if (override >= 0)
        absItemId = (u16)override;
    absItemId = comboProgressive(absItemId);

    itemId = itemId > 0 ? absItemId : -absItemId;

    return GiveItem(actor, play, itemId, a, b);
}
