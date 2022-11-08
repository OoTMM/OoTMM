#include <combo.h>

int EnGs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GOSSIP_HEART_PIECE, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x809989cc, EnGs_GiveItem);
PATCH_CALL(0x80998a30, EnGs_GiveItem);
