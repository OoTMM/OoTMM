#include <combo.h>
#include <combo/item.h>

void EnKendoJs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_SWORDSMAN_HEART_PIECE, a, b);
}

PATCH_CALL(0x80b27730, EnKendoJs_GiveItem);
