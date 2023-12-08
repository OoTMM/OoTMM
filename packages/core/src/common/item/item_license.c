#include <combo.h>

int isItemLicensed(s16 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
        return (gOotSave.inventory.upgrades.bombBag != 0 || gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    case GI_MM_MAGIC_BEAN:
        return (gMmSave.inventory.items[ITS_MM_BEANS] == ITEM_MM_MAGIC_BEAN);
    default:
        return 1;
    }
}
