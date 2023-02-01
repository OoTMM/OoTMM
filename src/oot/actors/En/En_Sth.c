#include <combo.h>

int EnSth_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_WALLET2:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GS_10, gi);
        break;
    case GI_OOT_STONE_OF_AGONY:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GS_20, gi);
        break;
    case GI_OOT_WALLET3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GS_30, gi);
        break;
    case GI_OOT_BOMBCHU_10:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GS_40, gi);
        break;
    case GI_OOT_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GS_50, gi);
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80b28268, EnSth_GiveItem);
