#include <combo.h>
#include <combo/player.h>

void FreezePlayer(GameState_Play* play)
{
    Actor_Player* link;
    link = GET_LINK(play);
    link->base.freezeTimer = 100;
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
}

void UnfreezePlayer(GameState_Play* play)
{
    Actor_Player* link;
    link = GET_LINK(play);
    link->base.freezeTimer = 0;
    link->state &= ~PLAYER_ACTOR_STATE_FROZEN;
}
