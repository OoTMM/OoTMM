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

int comboHasFreeBottleOot(void)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
            return 1;
    }
    if ((gOotExtraTrade.adult & (1 << XITEM_OOT_ADULT_BOTTLE)) && gOotExtraItems.bottleAdultSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        return 1;
    }
    if ((gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BOTTLE)) && gOotExtraItems.bottleChildSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        return 1;
    }
    return 0;
}

#if defined(GAME_OOT)
PATCH_FUNC(0x80071A94, comboHasFreeBottleOot);
#endif

int comboHasFreeBottleMm(void)
{
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}
