#include <combo.h>

int EnZot_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
        gMmExtraFlags.zoraHallLights = 1;
    return ret;
}

PATCH_CALL(0x80b98f40, EnZot_HasGivenItem);

int EnZot_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.zoraHallLights)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_ZORA_HALL_LIGHTS, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b98f7c, EnZot_GiveItem);
