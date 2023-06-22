#include <combo.h>
#include <combo/item.h>

void EnGb_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(actor, play, gi, NPC_OOT_POE_COLLECTOR, a, b);
}

PATCH_CALL(0x80b6b364, EnGb_GiveItem);
PATCH_CALL(0x80b6b3d4, EnGb_GiveItem);
