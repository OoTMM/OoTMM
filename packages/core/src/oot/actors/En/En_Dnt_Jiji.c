#include <combo.h>

void EnDntJiji_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_THEATER_STICKS, gi);
        break;
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_THEATER_NUTS, gi);
        break;
    }
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80b4e7d8, EnDntJiji_GiveItem);
PATCH_CALL(0x80b4e854, EnDntJiji_GiveItem);
