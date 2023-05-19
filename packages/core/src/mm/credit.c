#include <combo.h>

void comboCreditWarp(GameState_Play* play)
{
    Actor_Player* link;

    g.isCreditWarp = 1;

    /* Save */
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
    gSave.day = 0;
    gSave.time = 0x3fff;
    Sram_SaveNewDay(play);

    /* Credit warp */
    link = GET_LINK(play);
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
    play->nextEntrance = 0x5400;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_BLACK;
    gSaveContext.nextCutscene = 0xfff7;
}
