#include <combo.h>

static u8 sIsPictobox;

void EnDnh_AfterGivingItem(Actor* actor)
{
    if (sIsPictobox)
    {
        gMmExtraFlags.pictobox = 1;
        sIsPictobox = 0;
    }
}

int EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    sIsPictobox = 0;
    switch (gi)
    {
    case GI_MM_PICTOGRAPH_BOX:
        sIsPictobox = 1;
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
