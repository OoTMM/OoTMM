#include <combo.h>
#include <combo/player.h>
#include <combo/global.h>

void comboCreditWarp(GameState_Play* play)
{
    Actor_Player* link;

    g.isCreditWarp = 1;
    g.isCredits = 1;

    /* Save */
    Save_DoSave(play, 0);

    /* Credit warp */
    link = GET_PLAYER(play);
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_GFX_BLACK;
    play->nextEntranceIndex = 0x06b;
    gSaveContext.nextCutscene = 0xfff2;
}
