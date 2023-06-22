#include <combo.h>
#include <combo/item.h>

static u8 sIsHeartPiece;

void EnNb_AfterGivingItem(Actor* this)
{
    if (sIsHeartPiece)
    {
        sIsHeartPiece = 0;
        gMmExtraFlags2.grandma = 1;
    }
}

void EnNb_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
    {
        sIsHeartPiece = 1;
        if (!gMmExtraFlags2.grandma)
            npc = NPC_MM_GRANDMA_HEART_PIECE_1;
        else
            npc = NPC_MM_GRANDMA_HEART_PIECE_2;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}
