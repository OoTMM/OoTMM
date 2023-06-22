#include <combo.h>
#include <combo/item.h>

/* TODO: Can this be repeated? */
void EnInvadepoh_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_ROMANI_BOTTLE, a, b);
}

PATCH_CALL(0x80b4af64, EnInvadepoh_GiveItem);
