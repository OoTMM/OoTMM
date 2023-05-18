#include <combo.h>

void EnGe3_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_GERUDO_CARD, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b7f27c, EnGe3_GiveItem);
PATCH_CALL(0x80b7f1e8, EnGe3_GiveItem);
