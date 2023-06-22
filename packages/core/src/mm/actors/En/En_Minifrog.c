#include <combo.h>
#include <combo/item.h>

void EnMinifrog_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_FROG_HEART_PIECE, a, b);
}

PATCH_CALL(0x808a46cc, EnMinifrog_GiveItem);
