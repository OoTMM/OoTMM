#include <combo.h>

int EnAz_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_BOTTLE_EMPTY:
        gi = comboOverride(OV_NPC, 0, NPC_MM_BEAVER_1, gi);
        break;
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_MM_BEAVER_2, gi);
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a976a8, EnAz_GiveItem);
