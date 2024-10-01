#include <combo.h>
#include <combo/config.h>
#include <combo/entrance.h>

int KaleidoScope_CanSave(PlayState* play)
{
#if defined(GAME_MM)
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_EPONA)
        return 0;

    switch (play->sceneId)
    {
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_LINK:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_LAIR_MAJORA:
        return 0;
    case SCE_MM_CLOCK_TOWER_ROOFTOP:
        return !!Config_Flag(CFG_ER_MOON) || !!Config_Flag(CFG_MM_MOON_CRASH_CYCLE);
    case SCE_MM_SOUTHERN_SWAMP:
        return gSave.entrance != 0x8460 && !MM_CHECK_EVENT_INF(0x35) && !MM_CHECK_EVENT_INF(0x41);
    default:
        return 1;
    }
#else
    return 1;
#endif
}

static void saveMessage(PlayState* play)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Would you like to " TEXT_COLOR_RED);
    comboTextAppendStr(&b, "save");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
}

static void gameSavedMessage(PlayState* play)
{
    char* b;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Game saved." TEXT_NL);

    RespawnData* dungeonEntranceRespawn = &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE];
    if (dungeonEntranceRespawn->playerParams)
    {
        comboTextAppendStr(&b, TEXT_COLOR_GREEN TEXT_CHOICE3 "Continue" TEXT_NL "Return to Spawn" TEXT_NL "Return to Dungeon Entrance" TEXT_END);
    }
    else
    {
        comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Continue" TEXT_NL "Return to Spawn" TEXT_END);
    }
}

static void close(PlayState* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;

    Message_Close(play);
    pauseCtx->state = 6; /* PAUSE_STATE_MAIN */
    play->state.input[0].press.button |= START_BUTTON;
    gSaveContext.prevHudVisibilityMode = HUD_VISIBILITY_ALL;
}

s32 KaleidoScope_Update(PlayState* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    u16 buttons = play->state.input[0].press.button;

    switch (pauseCtx->state)
    {
    case 6: /* PAUSE_STATE_MAIN */
        switch (pauseCtx->mainState)
        {
        case 0: /* PAUSE_MAIN_STATE_IDLE */
        case 5: /* PAUSE_MAIN_STATE_SONG_PROMPT */
        case 8: /* PAUSE_MAIN_STATE_IDLE_CURSOR_ON_SONG */
#if defined(GAME_MM)
            u16 shouldSave = !pauseCtx->itemDescriptionOn && (buttons & B_BUTTON);
#else
            u16 shouldSave = (buttons & B_BUTTON);
#endif
            if (shouldSave && KaleidoScope_CanSave(play))
            {
                PlaySound(NA_SE_SY_DECIDE);
                PlayerDisplayTextBox(play, 0, NULL);
                saveMessage(play);
                pauseCtx->state = 7; /* PAUSE_STATE_SAVE_PROMPT */
                pauseCtx->savePromptState = 0;
                return 1;
            }
            break;
        }
        break;
    case 7: /* PAUSE_STATE_SAVE_PROMPT */
        switch (pauseCtx->savePromptState)
        {
        case 0:
            if (buttons & (A_BUTTON | U_CBUTTONS))
            {
                if (play->msgCtx.choiceIndex == 0)
                {
                    Save_DoSave(play, SF_OWL);
                    PlaySound(NA_SE_SY_PIECE_OF_HEART);
                    PlayerDisplayTextBox(play, 0, NULL);
                    gameSavedMessage(play);
                    pauseCtx->savePromptState = 1;
                }
                else
                {
                    close(play);
                    return 0;
                }
            }
            else if (buttons & (START_BUTTON | B_BUTTON))
            {
                play->state.input[0].press.button &= ~B_BUTTON;
                close(play);
                return 0;
            }
            break;
        case 1:
            if (buttons & (A_BUTTON | U_CBUTTONS))
            {
                if (play->msgCtx.choiceIndex == 0)
                {
                    close(play);
                    return 0;
                }
                else if (play->msgCtx.choiceIndex == 1)
                {
                    PlaySound(NA_SE_SY_DECIDE);
                    u32 entrance = gComboConfig.entrancesSpawns[gOotSave.age];
#if defined(GAME_MM)
                    /* Assume spawn is always an OoT spawn */
                    entrance |= MASK_FOREIGN_ENTRANCE;
#endif
                    comboTransition(play, entrance);
                    Player_Freeze(play);
                    close(play);
                    return 0;
                }
                else
                {
                    RespawnData* dungeonEntranceRespawn = &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE];
                    if (dungeonEntranceRespawn->playerParams)
                    {
                        PlaySound(NA_SE_SY_DECIDE);
#if defined(GAME_OOT)
                        if (dungeonEntranceRespawn->data & 0x80)
                        {
                            play->nextEntranceIndex = ENTR_CROSS_RESPAWN;
                        }
                        else
                        {
                            memcpy(&gSaveContext.respawn[RESPAWN_MODE_RETURN], dungeonEntranceRespawn, sizeof(RespawnData));
                            play->nextEntranceIndex = dungeonEntranceRespawn->entrance;
                            gSaveContext.respawnFlag = 2;
                        }
#else
                        if (!(dungeonEntranceRespawn->data & 0x80))
                        {
                            play->nextEntrance = ENTR_CROSS_RESPAWN;
                        }
                        else
                        {
                            memcpy(&gSaveContext.respawn[RESPAWN_MODE_TOP], dungeonEntranceRespawn, sizeof(RespawnData));
                            gSaveContext.respawn[RESPAWN_MODE_TOP].data &= 0x7f;
                            play->nextEntrance = dungeonEntranceRespawn->entrance;
                            gSaveContext.respawnFlag = -6;
                        }
#endif
                        play->transitionTrigger = TRANS_TRIGGER_START;
                        play->transitionType = TRANS_TYPE_FADE_BLACK;
                        Player_Freeze(play);
                        close(play);
                        return 0;
                    }
                    else
                    {
                        close(play);
                        return 0;
                    }
                }
            }
            else if (buttons & (START_BUTTON | B_BUTTON))
            {
                play->state.input[0].press.button &= ~B_BUTTON;
                close(play);
                return 0;
            }
            break;
        }
        return 1;
    }

    return 0;
}

typedef void (*KaleidoScopeUpdateFunc)(PlayState* play);

void KaleidoScope_UpdateWrapper(PlayState* play, KaleidoScopeUpdateFunc update)
{
    if (!KaleidoScope_Update(play))
    {
        update(play);
    }
}
