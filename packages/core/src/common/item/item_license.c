#include <combo.h>
#include <combo/config.h>

int isItemLicensed(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_BOMBCHU_BAG_FIRST_5:
    case GI_OOT_BOMBCHU_BAG_FIRST_10:
    case GI_OOT_BOMBCHU_BAG_FIRST_20:
        if (gOotSave.info.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10)
            return 1;
        if ((gComboConfig.bombchuBehaviorOot == BOMBCHU_BEHAVIOR_FREE || gComboConfig.bombchuBehaviorOot == BOMBCHU_BEHAVIOR_BOMB_BAG) && gOotSave.info.inventory.upgrades.bombBag)
            return 1;
        return 0;
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_BOMBCHU_BAG_FIRST_1:
    case GI_MM_BOMBCHU_BAG_FIRST_5:
    case GI_MM_BOMBCHU_BAG_FIRST_10:
    case GI_MM_BOMBCHU_BAG_FIRST_20:
        if (gMmSave.info.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU)
            return 1;
        if ((gComboConfig.bombchuBehaviorMm == BOMBCHU_BEHAVIOR_FREE || gComboConfig.bombchuBehaviorMm == BOMBCHU_BEHAVIOR_BOMB_BAG) && gOotSave.info.inventory.upgrades.bombBag)
            return 1;
        return 0;
    case GI_MM_MAGIC_BEAN:
        return (gMmSave.info.inventory.items[ITS_MM_BEANS] == ITEM_MM_MAGIC_BEAN);
    default:
        return 1;
    }
}
