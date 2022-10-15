#include <combo.h>

int EnGo2_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_TUNIC_GORON:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GORON_LINK_TUNIC, gi);
        break;
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GORON_BOMB_BAG, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b56ff4, EnGo2_GiveItem);
PATCH_CALL(0x80b5ad5c, EnGo2_GiveItem);
