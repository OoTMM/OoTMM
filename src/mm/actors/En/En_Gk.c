#include <combo.h>

int EnGk_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_BOTTLED_GOLD_DUST)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GORON_RACE, gi);
    }
    else
        gi = GI_MM_RECOVERY_HEART;
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80b525c8, EnGk_GiveItem);
