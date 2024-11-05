#include <combo.h>
#include <combo/player.h>
#include <combo/global.h>

void comboCreditWarp(PlayState* play)
{
    Player* link;

    g.isCreditWarp = 1;
    g.isCredits = 1;

    /* Save */
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
    gSave.day = 0;
    gSave.time = 0x3fff;
    Sram_SaveNewDay(play);

    /* Credit warp */
    link = GET_PLAYER(play);
    link->stateFlags1 |= PLAYER_ACTOR_STATE_FROZEN;
    play->nextEntrance = ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_WEST;
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    gSaveContext.nextCutscene = 0xfff7;
}
