#include <combo.h>
#include <combo/item.h>

void EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
        npc = NPC_MM_POSTBOX;
    comboGiveItemNpc(actor, play, gi, npc, a, b);
}
