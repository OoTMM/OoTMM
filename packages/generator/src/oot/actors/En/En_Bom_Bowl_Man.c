#include <combo.h>
#include <combo/config.h>

void EnBomBowlMan_AfterHandler(void)
{
    if (gSave.info.inventory.upgrades.bombBag || gSharedCustomSave.bombchuBagOot)
        SetEventChk(EV_OOT_CHK_BOMBCHU_BOWLING);
}
