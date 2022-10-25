#include <combo.h>

int EnAob01_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_DOG_LADY_HEART_PIECE, gi);
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x809c2ba8, EnAob01_GiveItem);
PATCH_CALL(0x809c2bc8, EnAob01_GiveItem);
