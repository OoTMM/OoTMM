#include <combo.h>

int EnYb_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskKamaro = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80bfa940, EnYb_HasGivenItem);

void EnYb_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskKamaro)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_KAMARO, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bfa9b0, EnYb_GiveItem);
