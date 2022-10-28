#include <combo.h>

int EnSth_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
        gi = comboOverride(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_OCEAN, gi);
        break;
    case GI_MM_MASK_TRUTH:
        gi = comboOverride(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_SWAMP, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x801311f4, EnSth_GiveItem);
PATCH_CALL(0x80131224, EnSth_GiveItem);
