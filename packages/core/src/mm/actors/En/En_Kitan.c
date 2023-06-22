#include <combo.h>
#include <combo/item.h>

/* TODO: Double check this for repeats */
void EnKitan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_KEATON_HEART_PIECE, a, b);
}

PATCH_CALL(0x80c096ec, EnKitan_GiveItem);
