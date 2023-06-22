#include <combo.h>
#include <combo/item.h>

void EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_TOUR_GUIDE_HEART_PIECE;
        break;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}
