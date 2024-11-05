#include <combo.h>
#include <combo/player.h>

void Player_Freeze(PlayState* play)
{
    Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 100;
    link->stateFlags1 |= PLAYER_ACTOR_STATE_FROZEN;
}

void Player_Unfreeze(PlayState* play)
{
    Player* link;
    link = GET_PLAYER(play);
    link->actor.freezeTimer = 0;
    link->stateFlags1 &= ~PLAYER_ACTOR_STATE_FROZEN;
}
