#include <combo.h>
#include <combo/item.h>

void EnAni_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_OOT_KAKARIKO_ROOF_MAN, a, b);
}

PATCH_CALL(0x80ad8940, EnAni_GiveItem);
PATCH_CALL(0x80ad89a4, EnAni_GiveItem);
