#include <combo.h>

void EnDnh_AfterGivingItem(Actor* actor)
{
    gMmExtraFlags.pictobox = 1;
}

int EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_PICTOGRAPH_BOX:
        if (gMmExtraFlags.pictobox)
            gi = GI_MM_RECOVERY_HEART;
        else
            gi = comboOverride(OV_NPC, 0, NPC_MM_KOUME_PICTOGRAPH_BOX, gi);
        break;
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_MM_KOUME_HEART_PIECE, gi);
        break;
    }
    return GiveItem(actor, play, gi, a, b);
}
