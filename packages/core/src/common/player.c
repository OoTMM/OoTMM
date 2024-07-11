#include <combo.h>
#include <combo/player.h>

void Player_Freeze(GameState_Play* play)
{
    Actor_Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 100;
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
}

void Player_Unfreeze(GameState_Play* play)
{
    Actor_Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 0;
    link->state &= ~PLAYER_ACTOR_STATE_FROZEN;
}
