#include <combo.h>

void EnGb2_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GHOST_HUT_HEART_PIECE, gi);
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b109c4, EnGb2_GiveItem);
