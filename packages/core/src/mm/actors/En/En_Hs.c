#include <combo.h>

int EnHs_HasGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        gMmExtraFlags.maskBunny = 1;
    }
    return ret;
}

PATCH_CALL(0x809530a4, EnHs_HasGivenItem);

void EnHs_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags.maskBunny)
    {
        gi = GI_MM_RUPEE_RED;
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_BUNNY, GI_MM_MASK_BUNNY);
    }

    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x8095314c, EnHs_GiveItem);
PATCH_CALL(0x80953168, EnHs_GiveItem);
