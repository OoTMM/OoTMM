#include <combo.h>

int EnGuruguru_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_BREMEN, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bc7504, EnGuruguru_GiveItem);
