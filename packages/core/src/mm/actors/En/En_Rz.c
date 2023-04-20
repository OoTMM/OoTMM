#include <combo.h>

int EnRz_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_ROSA_HEART_PIECE, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bfc2d8, EnRz_GiveItem);
PATCH_CALL(0x80bfc344, EnRz_GiveItem);
