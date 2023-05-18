#include <combo.h>

int EnGuruguru_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.maskBremen = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80bc7460, EnGuruguru_HasGivenItem);

void EnGuruguru_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags2.maskBremen)
        gi = GI_MM_RECOVERY_HEART;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_BREMEN, gi);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bc7504, EnGuruguru_GiveItem);
