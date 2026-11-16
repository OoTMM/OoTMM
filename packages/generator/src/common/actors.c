#include <combo.h>
#include <combo/enemy.h>
#include <combo/enemy_gold.h>
#include <combo/global.h>
#include <combo/actors.h>

void SetRoomClearHook(PlayState* play, u32 flag)
{
    if (!g.roomEnemyLackSoul && !comboEnemyEncounterPending(flag))
        Flags_SetClear(play, flag);
}

void Actor_Hyper(Actor* this, PlayState* play, ActorFunc update)
{
#if defined(GAME_OOT)
    if (this->id == ACTOR_EN_DOG || (this->id == ACTOR_BOSS_VA && (this->params >= 11 || this->params <= 15)) || this->id == ACTOR_BOSS_GANON)
#elif defined(GAME_MM)
    if (this->id == ACTOR_EN_DG)
#endif
        return;

    if (this->category != ACTORCAT_BOSS && this->category != ACTORCAT_ENEMY &&
        !(this->category == ACTORCAT_NPC && comboEnemyGoldIsTarget(this)))
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
