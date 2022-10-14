#include <combo.h>

void EnBomBowlMan_AfterHandler(void)
{
    if (gSave.inventory[ITS_OOT_BOMBCHU] != ITEM_NONE)
        SetEventChk(EV_OOT_CHK_BOMBCHU_BOWLING);
}
