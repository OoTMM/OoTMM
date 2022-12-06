#include <combo.h>

int EnSth_HasGivenItemMaskOfTruth(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskTruth = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80b67478, EnSth_HasGivenItemMaskOfTruth);

int EnSth_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
        gi = comboOverride(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_OCEAN, gi);
        break;
    case GI_MM_MASK_TRUTH:
        if (gMmExtraFlags2.maskTruth)
            gi = GI_MM_RECOVERY_HEART;
        else
            gi = comboOverride(OV_NPC, 0, NPC_MM_SPIDER_HOUSE_SWAMP, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b67524, EnSth_GiveItem);
PATCH_CALL(0x80b67c00, EnSth_GiveItem);
