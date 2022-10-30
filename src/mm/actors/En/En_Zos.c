#include <combo.h>

int EnZos_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_ZORA_EVAN, gi);
    }
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bbb3fc, EnZos_GiveItem);
