#include <combo.h>
#include <combo/item.h>

void EnOt_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_SEAHORSE_HEART_PIECE;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80b5c1a4, EnOt_GiveItem);
PATCH_CALL(0x80b5c228, EnOt_GiveItem);
