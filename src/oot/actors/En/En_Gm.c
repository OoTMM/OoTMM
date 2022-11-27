#include <combo.h>

int EnGm_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_MEDIGORON);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aa0030, EnGm_HasGivenItem);

int EnGm_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_MEDIGORON, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a9ffd8, EnGm_GiveItem);
PATCH_CALL(0x80aa006c, EnGm_GiveItem);

int EnGm_GetState(void)
{
    if (gSave.age == AGE_CHILD)
        return 0;

    if (BITMAP16_GET(gSave.eventsMisc, EV_OOT_INF_MEDIGORON))
        return 3;

    return 1;
}

PATCH_FUNC(0x80a9f8dc, EnGm_GetState);
