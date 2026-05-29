#include <combo.h>
#include <combo/mm/pause_menu.h>
#include <combo/mm/sequence.h>
#include <combo/common/kaleido_setup.h>
#include <combo/play.h>

void KaleidoSetup_Update(PlayState *play)
{
    Input *input = &play->state.input[0];
    MessageContext *msgCtx = &play->msgCtx;
    Player *player = GET_PLAYER(play);
    PauseContext *pauseCtx = &play->pauseCtx;

    if (CHECK_BTN_ALL(input->cur.button, BTN_R))
    {
        if (msgCtx && msgCtx)
        {
        }
    }

    if (IS_PAUSED(pauseCtx) || (play->gameOverCtx.state != GAMEOVER_INACTIVE))
    {
        return;
    }

    if ((play->transitionTrigger != TRANS_TRIGGER_OFF) || (play->transitionMode != TRANS_MODE_OFF))
    {
        return;
    }

    if ((gSaveContext.save.cutscene >= 0xFFF0) || (gSaveContext.nextCutscene >= 0xFFF0))
    {
        return;
    }

    if (!Play_InCsMode(play) || ((msgCtx->msgMode != MSGMODE_NONE) && (msgCtx->currentTextId == 0xFF)))
    {
        if (play->bButtonAmmoPlusOne >= 2)
        {
            return;
        }

        if ((gSaveContext.magicState == MAGIC_STATE_STEP_CAPACITY) || (gSaveContext.magicState == MAGIC_STATE_FILL))
        {
            return;
        }

        if (MM_CHECK_EVENT_INF(EVENTINF_17) || (player->stateFlags1 & PLAYER_STATE1_MM_20))
        {
            return;
        }

        if ((play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON) ||
            (play->actorCtx.flags & ACTORCTX_FLAG_PICTO_BOX_ON))
        {
            return;
        }

        // if (!play->actorCtx.isOverrideInputOn && CHECK_BTN_ALL(input->press.button, BTN_START))
        if(!play->actorCtx.isOverrideInputOn && KaleidoSetup_FrameAdvance(input))
        {
            gSaveContext.prevHudVisibilityMode = gSaveContext.hudVisibility;
            pauseCtx->itemDescriptionOn = false;
            pauseCtx->state = PAUSE_STATE_OPENING_0;
            func_800F4A10(play);
            // Set next page mode to scroll left
            pauseCtx->nextPageMode = pauseCtx->pageIndex * 2 + 1;
            Audio_SetPauseState(true);
        }

        if (pauseCtx->state == PAUSE_STATE_OPENING_0)
        {
            GameState_SetFramerateDivisor(&play->state, 2);
            if (ShrinkWindow_Letterbox_GetSizeTarget() != 0)
            {
                ShrinkWindow_Letterbox_SetSizeTarget(0);
            }
            Audio_PlaySfx_PauseMenuOpenOrClose(SFX_PAUSE_MENU_OPEN);
        }
    }
}

PATCH_FUNC(0x800f4c0c, KaleidoSetup_Update);
