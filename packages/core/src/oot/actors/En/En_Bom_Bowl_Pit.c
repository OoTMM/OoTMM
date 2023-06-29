#include <combo.h>
#include <combo/item.h>

void EnExItem_RewardByIndex(ComboItemQuery* q, int index);

void EnBomBowlPit_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnExItem_RewardByIndex(&q, *(u16*)((char*)actor + 0x14a));
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80aaf0ec, EnBomBowlPit_GiveItem);
PATCH_CALL(0x80aaf168, EnBomBowlPit_GiveItem);
