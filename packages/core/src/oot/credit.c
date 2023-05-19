#include <combo.h>

void comboCreditWarp(GameState_Play* play)
{
    Actor_Player* link;
    TransitionContext* t;

    g.isCreditWarp = 1;

    /* Save */
    PlayStoreFlags(play);
    gSave.sceneId = play->sceneId;
    comboWriteSave();

    /* Credit warp */
    link = GET_LINK(play);
    link->state |= PLAYER_ACTOR_STATE_FROZEN;
    t = &play->transition;
    t->type = TRANS_TYPE_NORMAL;
    t->gfx = TRANS_GFX_BLACK;
    t->entrance = 0x06b;
    gSaveContext.nextCutscene = 0xfff2;
}
