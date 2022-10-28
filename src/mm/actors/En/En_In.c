#include <combo.h>

int EnIn_HasActorGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
        gMmExtraFlags.maskGaro = 1;
    return ret;
}

PATCH_CALL(0x808f3d50, EnIn_HasActorGivenItem);

int EnIn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gMmExtraFlags.maskGaro)
        gi = GI_MM_MILK;
    else
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_GARO, GI_MM_MASK_GARO);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x808f3dbc, EnIn_GiveItem);
PATCH_CALL(0x808f4da0, EnIn_GiveItem);
PATCH_CALL(0x808f5390, EnIn_GiveItem);
