#include <combo.h>

void Player_UpdateWrapper(Actor_Player* this, GameState_Play* play)
{
    Player_Update(this, play);
    comboDpadUpdate(play);
}

int Player_DpadHook(Actor_Player* this, GameState_Play* play)
{
    if (Player_UsingItem(this))
        return 1;
    return comboDpadUse(play);
}
