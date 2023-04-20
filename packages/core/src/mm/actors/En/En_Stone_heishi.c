#include <combo.h>

int EnStoneheishi_HasGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        gMmExtraFlags.maskStone = 1;
    }
    return ret;
}

PATCH_CALL(0x80bc9d4c, EnStoneheishi_HasGivenItem);

int EnStoneheishi_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags.maskStone)
    {
        gi = GI_MM_RUPEE_BLUE;
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_STONE, GI_MM_MASK_STONE);
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bc9cd4, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9cf8, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9e10, EnStoneheishi_GiveItem);
PATCH_CALL(0x80bc9e34, EnStoneheishi_GiveItem);
