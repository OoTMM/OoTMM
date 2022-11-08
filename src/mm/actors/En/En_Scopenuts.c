#include <combo.h>

int EnScopenuts_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_TELESCOPE, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bcb968, EnScopenuts_GiveItem);
