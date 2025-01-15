#include <combo.h>
#include <combo/inventory.h>

void Inventory_ReobtainProgressiveShields(void)
{
    if (gOotExtraItems.shield & EQ_OOT_SHIELD_DEKU)
        gOotSave.info.inventory.equipment.shields |= EQ_OOT_SHIELD_DEKU;

    if (gOotExtraItems.shield & EQ_OOT_SHIELD_HYLIAN)
        gOotSave.info.inventory.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;

    if (gSharedCustomSave.mmProgressiveShields && gMmSave.info.itemEquips.shield == 0)
        gMmSave.info.itemEquips.shield = 1;
}
