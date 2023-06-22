#include <combo.h>
#include <combo/item.h>

void EnAz_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_BOTTLE_EMPTY:
        npc = NPC_MM_BEAVER_1;
        break;
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_BEAVER_2;
        break;
    }

    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80a976a8, EnAz_GiveItem);
