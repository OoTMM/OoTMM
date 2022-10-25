#include <combo.h>

int EnInvadepoh_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_ROMANI_BOTTLE, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b4af64, EnInvadepoh_GiveItem);
