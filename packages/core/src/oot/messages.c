#include <combo.h>

static u8 sInCustomSong;

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
            pauseCtx->world_map_cursor = i;
        }
    }

    if ((gMmOwlFlags >> 4) & 1) {
        pauseCtx->world_map_cursor = 4;
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
            gPrevCursorPoint = pauseCtx->world_map_cursor;
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

static s32 soarTargets[] = {
    0x68B0, // Great Bay Coast
    0x6A60, // Zora Cape
    0xB230, // Snowhead
    0x9A80, // Mountain Village (Winter)
    0xD890, // South Clock Town
    0x3E40, // Milk Road
    0x8640, // Woodfall
    0x84A0, // Southern Swamp (Poisoned)
    0x2040, // Ikana Canyon
    0xAA30, // Stone Tower
};

static void HandleSoaring(GameState_Play* play)
{
    int msgState;
    if (play->pauseCtx.state == 0)
    {
        if (gSoaringIndexSelected >= 0 && gSoaringIndexSelected < ARRAY_SIZE(soarTargets))
        {
            msgState = Message_GetState(&play->msgCtx);
            if (msgState)
            {
                if (msgState == 2)
                {
                    if (play->msgCtx.choice == 0)
                    {
                        s32 entrance = soarTargets[gSoaringIndexSelected];
                        if (gSoaringIndexSelected == 3 && gMiscFlags.erSpring) // Mountain Village (Winter)
                        {
                            entrance = 0xAE80; // Mountain Village (Spring)
                        }
                        else if (gSoaringIndexSelected == 7 && gMiscFlags.erSwampClear) // Southern Swamp (Poisoned)
                        {
                            entrance = 0x84A0; // Southern Swamp (Cleared)
                        }
                        comboGameSwitch(play, entrance);
                    }
                    else
                    {
                        /* Stop ocarina */
                        sInCustomSong = 0;
                        play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
                        gSoaringIndexSelected = -1;
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
            sInCustomSong = 0;
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
        }
    }
}

void HandleMessagesWrapper(GameState_Play* play, void* unk)
{
    if (gCustomOcarinaSong)
    {
        PlaySound(0x4807);
        sInCustomSong = gCustomOcarinaSong;
        gCustomOcarinaSong = 0;
        gPlayedOcarinaSong = 0;

        switch (sInCustomSong)
        {
        case 0x01:
            SetupSoaring(play);
            break;
        }
    }
    else if (sInCustomSong)
    {
        switch (sInCustomSong)
        {
        case 0x01:
            HandleSoaring(play);
            break;
        }
    }

    HandleMessages(play, unk);
}

PATCH_CALL(0x800e0a60, HandleMessagesWrapper);
