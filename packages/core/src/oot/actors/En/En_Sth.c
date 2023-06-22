#include <combo.h>
#include <combo/item.h>

void EnSth_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_OOT_WALLET2:
        npc = NPC_OOT_GS_10;
        break;
    case GI_OOT_STONE_OF_AGONY:
        npc = NPC_OOT_GS_20;
        break;
    case GI_OOT_WALLET3:
        npc = NPC_OOT_GS_30;
        break;
    case GI_OOT_BOMBCHU_10:
        npc = NPC_OOT_GS_40;
        break;
    case GI_OOT_HEART_PIECE:
        npc = NPC_OOT_GS_50;
        break;
    }

    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80b28268, EnSth_GiveItem);
