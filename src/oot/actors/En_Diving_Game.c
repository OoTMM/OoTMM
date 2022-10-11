#include <combo.h>

int EnDivingGame_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_ZORA_DIVING_GAME, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a7f0f0, &EnDivingGame_GiveItem);
PATCH_CALL(0x80a7f16c, &EnDivingGame_GiveItem);
