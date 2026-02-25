#include <combo.h>
#include <combo/item.h>

void EnNb_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    switch (gi)
    {
    case 0x0001: npc = NPC_MM_GRANDMA_HEART_PIECE_1; break;
    case 0x0002: npc = NPC_MM_GRANDMA_HEART_PIECE_2; break;
    default: UNREACHABLE();
    }

    comboGiveItemNpc(this, play, GI_MM_HEART_PIECE, npc, a, b);
}
