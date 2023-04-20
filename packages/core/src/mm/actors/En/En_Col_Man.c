#include <combo.h>

int EnColMan_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_LAB_FISH_HEART_PIECE, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80afdecc, EnColMan_GiveItem);

void EnColMan_DrawHeartPiece(Actor* this, GameState_Play* play)
{
    static const float scale = 12.5f;
    s16 gi;

    gi = comboOverride(OV_NPC, 0, NPC_MM_LAB_FISH_HEART_PIECE, GI_MM_HEART_PIECE);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x80afe414, EnColMan_DrawHeartPiece);
