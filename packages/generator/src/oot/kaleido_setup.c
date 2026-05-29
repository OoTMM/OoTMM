#include <combo.h>
#include <combo/common/kaleido_setup.h>
#include <combo/play.h>

extern f32 sKaleidoSetupRightPageEyeX[];
extern f32 sKaleidoSetupRightPageEyeZ[];
extern s16 sKaleidoSetupRightPageIndex[];

void KaleidoSetup_Update(PlayState *play)
{
    PauseContext *pauseCtx = &play->pauseCtx;
    Input *input = &play->state.input[0];

    if (!IS_PAUSED(pauseCtx) && play->gameOverCtx.state == GAMEOVER_INACTIVE &&
        play->transitionTrigger == TRANS_TRIGGER_OFF && play->transitionMode == TRANS_MODE_OFF &&
        gSaveContext.save.cutscene < CS_INDEX_0 && gSaveContext.nextCutscene < CS_INDEX_0 &&
        !Play_InCsMode(play) && play->shootingGalleryStatus <= 1 &&
        gSaveContext.magicState != MAGIC_STATE_STEP_CAPACITY && gSaveContext.magicState != MAGIC_STATE_FILL &&
        (play->sceneId != SCE_OOT_BOMBCHU_BOWLING_ALLEY || !Flags_GetSwitch(play, 0x38)))
    {

        if (KaleidoSetup_FrameAdvance(input))
        {
            gSaveContext.prevHudVisibilityMode = gSaveContext.hudVisibilityMode;

            R_PAUSE_BUTTON_LEFT_X = -175;
            R_PAUSE_BUTTON_RIGHT_X = 155;

            pauseCtx->pageSwitchTimer = 0;

            pauseCtx->mainState = PAUSE_MAIN_STATE_SWITCHING_PAGE;

            pauseCtx->eye.x = sKaleidoSetupRightPageEyeX[pauseCtx->pageIndex];
            pauseCtx->eye.z = sKaleidoSetupRightPageEyeZ[pauseCtx->pageIndex];
            pauseCtx->pageIndex = sKaleidoSetupRightPageIndex[pauseCtx->pageIndex];

            pauseCtx->nextPageMode = (u16)(pauseCtx->pageIndex * 2) + 1;
            pauseCtx->state = PAUSE_STATE_WAIT_LETTERBOX;
        }

        if (pauseCtx->state == PAUSE_STATE_WAIT_LETTERBOX)
        {
            R_PAUSE_PAGES_Y_ORIGIN_2 = PAUSE_PAGES_Y_ORIGIN_2_LOWER;
            R_UPDATE_RATE = 2;

            if (Letterbox_GetSizeTarget() != 0)
            {
                Letterbox_SetSizeTarget(0);
            }

            Audio_PlaySfx_PauseMenuOpenOrClose(1); // func_800F64E0
        }
    }
}

PATCH_FUNC(0x8005b860, KaleidoSetup_Update);
