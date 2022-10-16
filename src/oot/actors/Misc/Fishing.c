#include <combo.h>

int Fishing_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_FISH_CHILD, gi);
        break;
    case GI_OOT_SCALE_SILVER:
    case GI_OOT_SCALE_GOLDEN:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_FISH_ADULT, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a42644, Fishing_GiveItem);
PATCH_CALL(0x80a427e0, Fishing_GiveItem);
