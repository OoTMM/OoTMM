#include <combo.h>
#include <combo/item.h>

void EnGb2_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_GHOST_HUT_HEART_PIECE;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b109c4, EnGb2_GiveItem);
