#include <combo.h>
#include <combo/item.h>

void EnDivingGame_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(actor, play, gi, NPC_OOT_ZORA_DIVING_GAME, a, b);
}

PATCH_CALL(0x80a7f0f0, &EnDivingGame_GiveItem);
PATCH_CALL(0x80a7f16c, &EnDivingGame_GiveItem);
