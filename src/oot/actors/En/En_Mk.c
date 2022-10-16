#include <combo.h>

int EnMk_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_LABORATORY_DIVE, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80aac7e4, EnMk_GiveItem);
PATCH_CALL(0x80aac840, EnMk_GiveItem);
PATCH_CALL(0x80aacce8, EnMk_GiveItem);
PATCH_CALL(0x80aacd48, EnMk_GiveItem);
