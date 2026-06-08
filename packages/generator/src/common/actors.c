#include <combo.h>
#include <combo/global.h>
#include <combo/actors.h>

void SetRoomClearHook(PlayState* play, u32 flag)
{
    if (!g.roomEnemyLackSoul)
        SetRoomClear(play, flag);
}

void Actor_Hyper(Actor* this, PlayState* play, ActorFunc update)
{
#if defined(GAME_OOT)
    if (this->id == ACTOR_EN_DOG || (this->id == ACTOR_BOSS_VA && (this->params >= 11 || this->params <= 15)) || this->id == ACTOR_BOSS_GANON)
#elif defined(GAME_MM)
    if (this->id == ACTOR_EN_DG)
#endif
        return;

    if (this->type != ACTORCAT_BOSS && this->type != ACTORCAT_ENEMY)
        return;

    if (this->update == NULL)
        return;

    Player* link = GET_PLAYER(play);

    if (Player_InBlockingCsMode(play, link))
        return;

    int prev = play->frameAdvCtx.enabled;
    play->frameAdvCtx.enabled = true;
    update(this, play);
    play->frameAdvCtx.enabled = prev;
}
