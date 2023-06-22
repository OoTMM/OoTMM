#include <combo.h>
#include <combo/item.h>

void EnDu_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gSave.eventsItem[(EV_OOT_ITEM_GORON_BRACELET & 0xf0) >> 4] |= (1 << (EV_OOT_ITEM_GORON_BRACELET & 0xf));
    comboGiveItemNpc(actor, play, gi, NPC_OOT_DARUNIA_BRACELET, a, b);
}

PATCH_CALL(0x8095457c, &EnDu_GiveItem);
