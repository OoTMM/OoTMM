#include <combo.h>
#include <combo/item.h>

static u8 sIsPictobox;

void EnDnh_AfterGivingItem(Actor* actor)
{
    if (sIsPictobox)
    {
        gMmExtraFlags.pictobox = 1;
        sIsPictobox = 0;
    }
}

void EnDnh_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    sIsPictobox = 0;
    switch (gi)
    {
    case GI_MM_PICTOGRAPH_BOX:
        sIsPictobox = 1;
        if (gMmExtraFlags.pictobox)
            gi = GI_MM_RECOVERY_HEART;
        else
            npc = NPC_MM_KOUME_PICTOGRAPH_BOX;
        break;
    case GI_MM_HEART_PIECE:
        npc = NPC_MM_KOUME_HEART_PIECE;
        break;
    }

    comboGiveItemNpc(actor, play, gi, npc, a, b);
}
