#include <combo.h>

int EnYb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_KAMARO, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bfa9b0, EnYb_GiveItem);
