#include <combo.h>

u8 gMaxBombchuOot;
u8 gMaxBombchuMm;

static u8 Inventory_GetMaxBombchuGame(int behavior, int bombBagLevel, int bombchuBagLevel)
{
    switch (behavior)
    {
    case BOMBCHU_BEHAVIOR_FREE:
        return 50;
    case BOMBCHU_BEHAVIOR_BOMB_BAG:
        return kMaxBombs[bombBagLevel];
    case BOMBCHU_BEHAVIOR_BAG_FIRST:
        return bombchuBagLevel ? 50 : 0;
    case BOMBCHU_BEHAVIOR_BAG_SEPARATE:
        return kMaxBombs[bombchuBagLevel];
    default:
        UNREACHABLE();
    }
}

void Inventory_UpdateMaxBombchu(void)
{
    gMaxBombchuOot = Inventory_GetMaxBombchuGame(gComboConfig.bombchuBehaviorOot, gOotSave.info.inventory.upgrades.bombBag, gSharedCustomSave.bombchuBagOot);
    gMaxBombchuMm = Inventory_GetMaxBombchuGame(gComboConfig.bombchuBehaviorMm, gMmSave.info.inventory.upgrades.bombBag, gSharedCustomSave.bombchuBagMm);
}
