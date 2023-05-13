#include <combo.h>

void EnGb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_POE_COLLECTOR, gi);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80b6b364, EnGb_GiveItem);
PATCH_CALL(0x80b6b3d4, EnGb_GiveItem);
