#include <combo.h>
#include <combo/item.h>

void EnSth_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.gi = gi;
    q.ovType = OV_NPC;

    switch (gi)
    {
    case GI_OOT_WALLET2:
        q.id = NPC_OOT_GS_10;
        break;
    case GI_OOT_STONE_OF_AGONY:
        q.id = NPC_OOT_GS_20;
        break;
    case GI_OOT_WALLET3:
        q.id = NPC_OOT_GS_30;
        break;
    case GI_OOT_BOMBCHU_10:
        q.id = NPC_OOT_GS_40;
        break;
    case GI_OOT_HEART_PIECE:
        q.id = NPC_OOT_GS_50;
        break;
    case GI_OOT_RUPEE_HUGE:
        q.id = NPC_OOT_GS_100;
        if (BITMAP8_GET(gSharedCustomSave.oot.npc, NPC_OOT_GS_100))
            q.ovType = OV_NONE;
        break;
    }

    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80b28268, EnSth_GiveItem);
