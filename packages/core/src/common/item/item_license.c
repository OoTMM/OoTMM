#include <combo.h>

int isItemLicensed(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_BOMBCHU_BAG_5:
    case GI_OOT_BOMBCHU_BAG_10:
    case GI_OOT_BOMBCHU_BAG_20:
        return ((!comboConfig(CFG_OOT_BOMBCHU_BAG) && gOotSave.inventory.upgrades.bombBag != 0) || gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_BOMBCHU_BAG:
    case GI_MM_BOMBCHU_BAG_5:
    case GI_MM_BOMBCHU_BAG_10:
    case GI_MM_BOMBCHU_BAG_20:
        return (comboConfig(CFG_MM_BOMBCHU_BAG) ? (gMmSave.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU) : (gMmSave.inventory.upgrades.bombBag != 0));
    case GI_MM_MAGIC_BEAN:
        return (gMmSave.inventory.items[ITS_MM_BEANS] == ITEM_MM_MAGIC_BEAN);
    default:
        return 1;
    }
}
