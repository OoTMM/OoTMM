#include <combo.h>

void EnGo_AfterGivingItem(Actor* actor)
{
    gMmExtraFlags.powderKeg = 1;
}

int EnGo_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.powderKeg)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GORON_KEG, GI_MM_POWDER_KEG);
    }
    else if (gMmSave.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG)
    {
        gi = GI_MM_POWDER_KEG;
    }
    else
    {
        gi = GI_MM_RECOVERY_HEART;
    }
    return GiveItem(this, play, gi, a, b);
}
