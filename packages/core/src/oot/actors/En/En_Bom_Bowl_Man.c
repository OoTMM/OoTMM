#include <combo.h>

void EnBomBowlMan_AfterHandler(void)
{
    if (gSave.inventory.upgrades.bombBag || (comboConfig(CFG_OOT_BOMBCHU_BAG) && gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10))
        SetEventChk(EV_OOT_CHK_BOMBCHU_BOWLING);
}
