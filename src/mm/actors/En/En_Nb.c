#include <combo.h>

static u8 sIsHeartPiece;

void EnNb_AfterGivingItem(Actor* this)
{
    if (sIsHeartPiece)
    {
        sIsHeartPiece = 0;
        gMmExtraFlags2.grandma = 1;
    }
}

int EnNb_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        sIsHeartPiece = 1;
        if (!gMmExtraFlags2.grandma)
            gi = comboOverride(OV_NPC, 0, NPC_MM_GRANDMA_HEART_PIECE_1, gi);
        else
            gi = comboOverride(OV_NPC, 0, NPC_MM_GRANDMA_HEART_PIECE_2, gi);
    }
    return GiveItem(this, play, gi, a, b);
}
