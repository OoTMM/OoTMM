#include <combo.h>

int EnDno_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskScents = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a72bb4, EnDno_HasGivenItem);

void EnDno_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskScents)
        gi = GI_MM_RUPEE_RED;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_SCENTS, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a729c4, EnDno_GiveItem);
PATCH_CALL(0x80a72bec, EnDno_GiveItem);
