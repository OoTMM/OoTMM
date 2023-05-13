#include <combo.h>

void EnAl_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_MASK_KAFEI:
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_KAFEI, gi);
        gMmExtraFlags2.maskKafei = 1;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        gi = comboOverride(OV_NPC, 0, NPC_MM_CHATEAU_ROMANI, gi);
        break;
    }
    GiveItem(this, play, gi, a, b);
}
