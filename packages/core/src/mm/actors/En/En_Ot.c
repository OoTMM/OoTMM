#include <combo.h>

void EnOt_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
        gi = comboOverride(OV_NPC, 0, NPC_MM_SEAHORSE_HEART_PIECE, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b5c1a4, EnOt_GiveItem);
PATCH_CALL(0x80b5c228, EnOt_GiveItem);
