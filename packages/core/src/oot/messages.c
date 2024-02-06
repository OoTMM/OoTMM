#include <combo.h>
#include <combo/entrance.h>

u8 gInCustomSong;

const char* soarNames[10] = {
    "Great Bay Coast",
    "Zora Cape",
    "Snowhead",
    "Mountain Village",
    "Clock Town",
    "Milk Road",
    "Woodfall",
    "Southern Swamp",
    "Ikana Canyon",
    "Stone Tower"
};

static void soaringMessage(GameState_Play* play, u8 soaringIndex)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Soar to " TEXT_COLOR_RED);
    comboTextAppendStr(&b, soarNames[soaringIndex]);
    comboTextAppendStr(&b, TEXT_CZ "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "OK" TEXT_NL "No" TEXT_END);
}

static void soaringNoStatuesMessage(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You have yet to leave your " TEXT_COLOR_RED "mark" TEXT_CZ TEXT_NL);
    comboTextAppendStr(&b, "on any of the " TEXT_COLOR_RED "statues" TEXT_CZ " you have" TEXT_NL);
    comboTextAppendStr(&b, "come across. Hurry up and find" TEXT_NL);
    comboTextAppendStr(&b, "some." TEXT_END);
}

static void PrepareSoaringScreen(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;

    // Rumble_StateReset();

    pauseCtx->switchPageTimer = 0;
    pauseCtx->changing = 1; // PAUSE_MAIN_STATE_SWITCHING_PAGE;

    // Set eye position and pageIndex such that scrolling left brings to the desired page
    pauseCtx->eye.x = -64.0f; // sKaleidoSetupEyeX1[pauseCtx->pageIndex];
    pauseCtx->eye.z = 0.0f; // sKaleidoSetupEyeZ1[pauseCtx->pageIndex];
    pauseCtx->screen_idx = 1 ; // sKaleidoSetupKscpPos1[pauseCtx->pageIndex];
    pauseCtx->infoPanelOffsetY = -40;

    for (i = 0; i < ARRAY_SIZE(pauseCtx->worldMapPoints); i++) {
        pauseCtx->worldMapPoints[i] = 0;
    }

    for (i = OWL_WARP_STONE_TOWER; i >= OWL_WARP_GREAT_BAY_COAST; i--) {
        if ((gMmOwlFlags >> i) & 1) {
            pauseCtx->worldMapPoints[i] = 1;
            pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = i;
        }
    }

    if ((gMmOwlFlags >> 4) & 1) {
        pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = 4;
    }
}

static void SetupSoaring(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;

    if (play->interfaceCtx.restrictions.warpSongs)
    {
        PlayerDisplayTextBox(play, 0x88c, NULL);
    }
    else
    {
        // if in a dungeon or boss scene
        // {
        //      "Soar to Dungeon Entrance?"
        // }
        // else
        if (!gMmOwlFlags)
        {
            PlayerDisplayTextBox(play, 0x88c, NULL);
            soaringNoStatuesMessage(play);
        }
        else
        {
            gPrevPageIndex = pauseCtx->screen_idx;
            gPrevCursorPoint = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
            pauseCtx->screen_idx = 0;
            pauseCtx->state = PAUSE_STATE_OWLWARP_0;
            PrepareSoaringScreen(play);
            pauseCtx->screen_idx = 1;
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
            gSaveContext.prevHudVisibilityMode = gSaveContext.hudVisibilityMode;
            gSaveContext.nextHudVisibilityMode = 0xC; // HUD_VISIBILITY_A | HUD_VISIBILITY_B;
            R_UPDATE_RATE = 2;
            if (Letterbox_GetSizeTarget() != 0) {
                Letterbox_SetSizeTarget(0);
            }
            func_800C7200(1);
            play->msgCtx.msgLength = 0;
            play->msgCtx.msgMode = 0; // MSGMODE_NONE
        }
    }
}

static void HandleSoaring(GameState_Play* play)
{
    int msgState;
    int songId;
    if (play->pauseCtx.state == 0)
    {
        if (gSoaringIndexSelected >= 0 && gSoaringIndexSelected < 10)
        {
            msgState = Message_GetState(&play->msgCtx);
            if (msgState)
            {
                if (msgState == 2)
                {
                    songId = gSoaringIndexSelected;

                    /* Stop ocarina */
                    gInCustomSong = 0;
                    play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
                    gSoaringIndexSelected = -1;

                    if (play->msgCtx.choice == 0)
                    {
                        u32 entrance = gComboData.entrancesOwl[songId] ^ MASK_FOREIGN_ENTRANCE;
                        comboTransition(play, entrance);
                    }
                }
            }
            else
            {
                PlayerDisplayTextBox(play, 0x88d, NULL);
                soaringMessage(play, gSoaringIndexSelected);
            }
        }
        else
        {
            /* Stop ocarina */
            gInCustomSong = 0;
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
        }
    }
}

static void HandleSongOfTime(GameState_Play* play)
{
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState == 2)
    {
        /* Stop ocarina */
        gInCustomSong = 0;

        if (play->msgCtx.choice == 0)
        {
            /* Age swap */
            play->linkAgeOnLoad = !gSaveContext.save.age;
            Play_SetupRespawnPoint(play, 1, 0xDFF);
            gSaveContext.respawnFlag = 2;
            play->transitionTrigger = TRANS_TYPE_NORMAL;
            play->nextEntranceIndex = gSaveContext.save.entrance;
            play->transitionType = TRANS_GFX_SHORTCUT;

            /* Set the correct farore */
            swapFarore();
        }
        else
        {
            PlaySound(0x480A);
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
        }
    }
}

void HandleMessagesWrapper(GameState_Play* play, void* unk)
{
    if (gCustomOcarinaSong)
    {
        PlaySound(0x4807);
        gInCustomSong = gCustomOcarinaSong;
        gCustomOcarinaSong = 0;
        gPlayedOcarinaSong = 0;

        switch (gInCustomSong)
        {
        case 0x01:
            SetupSoaring(play);
            break;
        }
    }
    else if (gInCustomSong)
    {
        switch (gInCustomSong)
        {
        case 0x01:
            HandleSoaring(play);
            break;
        case 0x02:
            HandleSongOfTime(play);
            break;
        }
    }

    HandleMessages(play, unk);
}

PATCH_CALL(0x800e0a60, HandleMessagesWrapper);
