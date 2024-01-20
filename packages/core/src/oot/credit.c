#include <combo.h>

void comboCreditWarp(GameState_Play* play)
{
    Actor_Player* link;

    g.isCreditWarp = 1;

    /* Save */
    comboSave(play, 0);

    /* Credit warp */
    link = GET_LINK(play);
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
    play->transitionTrigger = TRANS_TYPE_NORMAL;
    play->transitionType = TRANS_GFX_BLACK;
    play->nextEntranceIndex = 0x06b;
    gSaveContext.nextCutscene = 0xfff2;
}
