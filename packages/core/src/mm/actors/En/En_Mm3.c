#include <combo.h>

int EnMm3_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_POSTMAN_HEART_PIECE, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a6fec0, EnMm3_GiveItem);
