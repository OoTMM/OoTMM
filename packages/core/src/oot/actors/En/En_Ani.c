#include <combo.h>

int EnAni_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_KAKARIKO_ROOF_MAN, gi);
    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80ad8940, EnAni_GiveItem);
PATCH_CALL(0x80ad89a4, EnAni_GiveItem);
