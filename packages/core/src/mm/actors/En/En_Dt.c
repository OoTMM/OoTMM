#include <combo.h>

void EnDt_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_MAYOR_HEART_PIECE, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80beab14, EnDt_GiveItem);
PATCH_CALL(0x80beabe0, EnDt_GiveItem);
