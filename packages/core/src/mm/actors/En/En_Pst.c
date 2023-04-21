#include <combo.h>

int EnPst_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
        gi = comboOverride(OV_NPC, 0, NPC_MM_POSTBOX, gi);
    return GiveItem(actor, play, gi, a, b);
}
