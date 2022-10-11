#include <combo.h>

s16 EnExItem_Reward(Actor* actor);
s16 EnExItem_RewardByIndex(int index);

int EnBomBowlPit_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = EnExItem_RewardByIndex(*(u16*)((char*)actor + 0x14a));
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80aaf0ec, EnBomBowlPit_GiveItem);
PATCH_CALL(0x80aaf168, EnBomBowlPit_GiveItem);
