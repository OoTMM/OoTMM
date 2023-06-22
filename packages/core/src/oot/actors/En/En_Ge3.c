#include <combo.h>
#include <combo/item.h>

void EnGe3_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_OOT_GERUDO_CARD, a, b);
}

PATCH_CALL(0x80b7f27c, EnGe3_GiveItem);
PATCH_CALL(0x80b7f1e8, EnGe3_GiveItem);
