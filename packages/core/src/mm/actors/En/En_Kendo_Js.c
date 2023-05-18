#include <combo.h>

void EnKendoJs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_SWORDSMAN_HEART_PIECE, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b27730, EnKendoJs_GiveItem);
