#include <combo.h>
#include <combo/item.h>

void EnDt_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_MM_MAYOR_HEART_PIECE, a, b);
}

PATCH_CALL(0x80beab14, EnDt_GiveItem);
PATCH_CALL(0x80beabe0, EnDt_GiveItem);
