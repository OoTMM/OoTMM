#include <combo.h>
#include <combo/item.h>

void EnSth_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;
    int flags;

    npc = -1;
    flags = 0;
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
    case GI_OOT_RUPEE_HUGE:
        npc = NPC_OOT_GS_100;
        break;
    }

    comboGiveItemNpcEx(actor, play, gi, npc, flags, a, b);
}

PATCH_CALL(0x80b28268, EnSth_GiveItem);
