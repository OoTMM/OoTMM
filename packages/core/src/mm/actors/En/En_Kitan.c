#include <combo.h>

void EnKitan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_KEATON_HEART_PIECE, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80c096ec, EnKitan_GiveItem);
