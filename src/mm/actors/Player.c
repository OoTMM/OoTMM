#include <combo.h>

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    Player_Update(this, play);
    comboDpadUpdate(play);
    Ocarina_HandleWarp(this, play);
}

int Player_DpadHook(Actor_Player* this, GameState_Play* play)
{
    if (Player_UsingItem(this))
        return 1;
    return comboDpadUse(play, DPF_ITEMS);
}

PATCH_CALL(0x8082fe58, Player_DpadHook);
