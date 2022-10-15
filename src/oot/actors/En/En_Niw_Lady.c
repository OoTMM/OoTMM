#include <combo.h>

int EnNiwLady_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_EMPTY_BOTTLE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ANJU_BOTTLE, gi);
        break;
    case GI_OOT_POCKET_EGG:
    case GI_OOT_POCKET_CUCCO:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ANJU_EGG, gi);
        break;
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a9e95c, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9e9a0, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9ec70, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9edc4, EnNiwLady_GiveItem);
PATCH_CALL(0x80a9eea8, EnNiwLady_GiveItem);
