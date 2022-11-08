#include <combo.h>

int EnJs_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskFierceDeity = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x8096a2fc, EnJs_HasGivenItem);

int EnJs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskFierceDeity)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_FIERCE_DEITY, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x8096a370, EnJs_GiveItem);
