#include <combo.h>
#include <combo/item.h>

void EnRz_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_ROSA_HEART_PIECE, a, b);
}

PATCH_CALL(0x80bfc2d8, EnRz_GiveItem);
PATCH_CALL(0x80bfc344, EnRz_GiveItem);
