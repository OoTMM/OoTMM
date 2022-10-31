#include <combo.h>

int EnGeg_HasGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
        gMmExtraFlags.maskDonGero = 1;
    return ret;
}

PATCH_CALL(0x80bb3230, EnGeg_HasGivenItem);

int EnGeg_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.maskDonGero)
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_DON_GERO, GI_MM_MASK_DON_GERO);
    else
        gi = GI_MM_RUPEE_PURPLE;
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bb3290, EnGeg_GiveItem);
