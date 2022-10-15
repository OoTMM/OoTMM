#include <combo.h>

int EnZk_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_TUNIC_ZORA:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ZORA_KING_TUNIC, gi);
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80ad6cc8, EnZk_GiveItem);
