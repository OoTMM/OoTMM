#include <combo.h>

int EnSkj_GiveItem_TreeHP(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_LOST_WOODS_SKULL_KID, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a6e550, EnSkj_GiveItem_TreeHP);
PATCH_CALL(0x80a6e5e8, EnSkj_GiveItem_TreeHP);
