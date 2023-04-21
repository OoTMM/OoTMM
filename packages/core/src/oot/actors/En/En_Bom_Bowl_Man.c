#include <combo.h>

void EnBomBowlMan_AfterHandler(void)
{
    if (gSave.inventory.upgrades.bombBag)
        SetEventChk(EV_OOT_CHK_BOMBCHU_BOWLING);
}
