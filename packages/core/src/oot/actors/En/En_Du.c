#include <combo.h>

int EnDu_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_DARUNIA_BRACELET, gi);
    gSave.eventsItem[(EV_OOT_ITEM_GORON_BRACELET & 0xf0) >> 4] |= (1 << (EV_OOT_ITEM_GORON_BRACELET & 0xf));
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x8095457c, &EnDu_GiveItem);
