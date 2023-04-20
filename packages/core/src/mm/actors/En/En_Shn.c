#include <combo.h>

int EnShn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_MM_TOUR_GUIDE_HEART_PIECE, gi);
        break;
    }
    return GiveItem(actor, play, gi, a, b);
}
