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

void EnGeg_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (!gMmExtraFlags.maskDonGero)
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_DON_GERO, GI_MM_MASK_DON_GERO);
    else
        gi = GI_MM_RUPEE_PURPLE;
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bb3290, EnGeg_GiveItem);

/**
 * Add new points for Hungry Goron to roll on
 **/
static Vec3f rollTargets[6] = {
    {
        .x = -550,
        .y = 8,
        .z = 550
    },
    {
        .x = 220,
        .y = 43,
        .z = 525
    },
    {
        .x = 960,
        .y = 90,
        .z = 525
    },
    {
        .x = 2060,
        .y = 16,
        .z = 925
    },
    {
        .x = 2710,
        .y = -40,
        .z = 1980
    },
    {
        .x = 3510,
        .y = -90,
        .z = 2380
    }
};

/**
 * Hook function used to get the roll target position of the goron.
 **/
Vec3f* DonGero_GetRollTarget(s16 index) {
    if (index < 6) {
        return &rollTargets[index];
    }
    return NULL;
}

/**
 * Hook function used to override starting the cutscene.
 **/
void DonGero_StartCutscene(s16 index, Actor* actor) {
    if (index == 0x17) {
        u8* address = (u8*)actor;
        u16* rollingAddress = (u16*)(address + 0x4AA);
        *rollingAddress = 0x180;
    } else {
        ActorCutscene_StartAndSetFlag(index, actor);
    }
}
