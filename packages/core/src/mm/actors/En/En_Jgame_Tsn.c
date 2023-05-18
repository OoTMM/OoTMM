#include <combo.h>

void EnJgameTsn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
        gi = comboOverride(OV_NPC, 0, NPC_MM_FISHERMAN_HEART_PIECE, gi);
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80c145cc, EnJgameTsn_GiveItem);
PATCH_CALL(0x80c145e4, EnJgameTsn_GiveItem);
