#include <combo.h>
#include <combo/entrance.h>

typedef enum {
    CUSTOM_SONG_NONE,
    CUSTOM_SONG_SOARING,
    CUSTOM_SONG_TIME,
    CUSTOM_SONG_ELEGY,
} CustomOcarinaSong;

extern u32 gOcarinaPressedButtons;
static u8 sInCustomSong = CUSTOM_SONG_NONE;

static OcarinaSongButtons sSongSoaring = {
    6,
    {
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
    }
};

static OcarinaSongButtons sSongElegy = {
    7,
    {
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_LEFT,
    }
};

void Ocarina_CheckCustomSongs(void)
{
    if (gMmSave.inventory.quest.songSoaring
        && comboConfig(CFG_MM_CROSS_WARP)
        && (comboConfig(CFG_MM_CROSS_WARP_ADULT) || gSave.age == AGE_CHILD))
    {
        comboCheckSong(&sSongSoaring, CUSTOM_SONG_SOARING);
    }

    // if (comboConfig(CFG_SHARED_ELEGY))
    {
        comboCheckSong(&sSongElegy, CUSTOM_SONG_ELEGY);
    }
}

void OcarinaMaskButtons(void)
{
    u32 mask;

    mask = ~(A_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS);
    mask |= gSharedCustomSave.ocarinaButtonMaskOot;
    gOcarinaPressedButtons &= mask;
}

static void changeAgeMessage(GameState_Play* play, s32 toChild)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    if (toChild)
        comboTextAppendStr(&b, "Become a " TEXT_COLOR_RED "child");
    else
        comboTextAppendStr(&b, "Become an " TEXT_COLOR_RED "adult");
    comboTextAppendStr(&b, TEXT_CZ "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "OK" TEXT_NL "No" TEXT_END);
}

void Ocarina_HandleLastPlayedSong(GameState_Play* play, Actor_Player* player, s16 lastPlayedSong)
{
    s32 canChangeAge;

    switch (lastPlayedSong)
    {
    /* Displaced code: */
    case OCARINA_SONG_SARIAS:
        player->naviTextId = -0xE0;
        player->naviActor->flags |= (1 << 16); // ACTOR_FLAG_16
        break;
    /* End displaced code. */
    case OCARINA_SONG_TIME:
        canChangeAge = comboConfig(CFG_OOT_AGE_CHANGE) && GetEventChk(EV_OOT_CHK_MASTER_SWORD_CHAMBER) && GetEventChk(EV_OOT_CHK_MASTER_SWORD_PULLED);
        if (canChangeAge && comboConfig(CFG_OOT_AGE_CHANGE_NEEDS_OOT) && gSave.inventory.items[ITS_OOT_OCARINA] != ITEM_OOT_OCARINA_TIME)
            canChangeAge = 0;
#if defined(DEBUG)
        canChangeAge = 1;
#endif
        if (play->msgCtx.ocarinaAction != 0x29) // OCARINA_ACTION_FREE_PLAY_DONE
        {
            canChangeAge = 0;
        }
        if (canChangeAge)
        {
            switch (play->sceneId)
            {
            case SCE_OOT_GANON_TOWER_COLLAPSING:
            case SCE_OOT_INSIDE_GANON_CASTLE_COLLAPSING:
            case SCE_OOT_TREASURE_SHOP:
            case SCE_OOT_BOMBCHU_BOWLING_ALLEY:
            case SCE_OOT_LAIR_GOHMA:
            case SCE_OOT_LAIR_KING_DODONGO:
            case SCE_OOT_LAIR_BARINADE:
            case SCE_OOT_LAIR_PHANTOM_GANON:
            case SCE_OOT_LAIR_VOLVAGIA:
            case SCE_OOT_LAIR_MORPHA:
            case SCE_OOT_LAIR_TWINROVA:
            case SCE_OOT_LAIR_BONGO_BONGO:
            case SCE_OOT_LAIR_GANONDORF:
            case SCE_OOT_TOWER_COLLAPSE_EXTERIOR:
            case SCE_OOT_GROTTOS:
            case SCE_OOT_FAIRY_FOUNTAIN:
            case SCE_OOT_CASTLE_MAZE_DAY:
            case SCE_OOT_CASTLE_MAZE_NIGHT:
            case SCE_OOT_CASTLE_COURTYARD:
            case SCE_OOT_GANON_BATTLE_ARENA:
            case SCE_OOT_HYRULE_CASTLE:
            case SCE_OOT_GANON_CASTLE_EXTERIOR:
            case SCE_OOT_MARKET_ENTRANCE_CHILD_DAY:
            case SCE_OOT_MARKET_ENTRANCE_CHILD_NIGHT:
            case SCE_OOT_MARKET_ENTRANCE_ADULT:
            case SCE_OOT_BACK_ALLEY_DAY:
            case SCE_OOT_BACK_ALLEY_NIGHT:
            case SCE_OOT_MARKET_CHILD_DAY:
            case SCE_OOT_MARKET_CHILD_NIGHT:
            case SCE_OOT_MARKET_ADULT:
            case SCE_OOT_TEMPLE_OF_TIME_EXTERIOR_CHILD_DAY:
            case SCE_OOT_TEMPLE_OF_TIME_EXTERIOR_CHILD_NIGHT:
            case SCE_OOT_TEMPLE_OF_TIME_EXTERIOR_ADULT:
            case SCE_OOT_GRAVEKEEPER_HUT:
            case SCE_OOT_FISHING_POND:
                canChangeAge = 0;
                break;
            }
        }
        if (canChangeAge)
        {
            ActorContext* actorCtx;
            Actor* actor;

            actorCtx = &play->actorCtx;
            actor = actorCtx->actors[7].first;

            while (actor)
            {
                if (actor->id == AC_OBJ_TIMEBLOCK || actor->id == AC_OBJ_WARP2BLOCK)
                {
                    Actor_ObjTimeblockWarp2Block* timeblock;

                    timeblock = (Actor_ObjTimeblockWarp2Block*)actor;
                    if (timeblock->demoEffectTimer != 0)
                    {
                        canChangeAge = 0;
                        break;
                    }
                }
                actor = actor->next;
            }
        }
        if (canChangeAge)
        {
            PlayerDisplayTextBox(play, 0x88d, NULL);
            changeAgeMessage(play, !gSaveContext.save.age);
            sInCustomSong = CUSTOM_SONG_TIME;
        }
        break;
    }
}

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
                    sInCustomSong = CUSTOM_SONG_NONE;
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
            sInCustomSong = CUSTOM_SONG_NONE;
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
        }
    }
}

void ageSwap(GameState_Play* play)
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

static void HandleSongOfTime(GameState_Play* play)
{
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState == 2)
    {
        /* Stop ocarina */
        sInCustomSong = CUSTOM_SONG_NONE;

        if (play->msgCtx.choice == 0)
        {
            ageSwap(play);
        }
        else
        {
            PlaySound(0x480A);
            play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
        }
    }
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

EnTorch2* gElegyShells[1]; // need to increase this if other forms are added to OoT.

static void HandleElegy(GameState_Play* play)
{
    sInCustomSong = CUSTOM_SONG_NONE;
    Actor_Player* player = GET_LINK(play);
    u8 playerForm = 0;

    EnTorch2* torch2 = gElegyShells[playerForm];
    if (torch2 != NULL)
    {
        Math_Vec3f_Copy(&torch2->base.home.pos, &player->base.world.pos);
        torch2->base.home.rot.y = player->base.rot2.y;
        torch2->state = 0;
        torch2->framesUntilNextState = 20;
    }
    else
    {
        torch2 = (EnTorch2*)SpawnActor(&play->actorCtx, play, AC_CUSTOM_TORCH2, player->base.world.pos.x,
                                        player->base.world.pos.y, player->base.world.pos.z, 0, player->base.rot2.y, 0,
                                        playerForm);
    }

    if (torch2 != NULL)
    {
        gElegyShells[playerForm] = torch2;
    }

    s32 effectType = 0x19; // DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL
    SpawnActor(&play->actorCtx, play, AC_DEMO_EFFECT, player->base.world.pos.x, player->base.world.pos.y,
                                player->base.world.pos.z, 0, player->base.rot2.y, 0, effectType);

    play->msgCtx.ocarinaMode = 4; // OCARINA_MODE_END
    Message_Close(play);
}

u8 Ocarina_BeforeSongPlayingProcessed(GameState_Play* play)
{
    u8 songPlayed = play->msgCtx.ocarinaStaff->state;

    if (songPlayed >= 0x81 && songPlayed <= 0x83)
    {
        PlaySound(0x4807); // NA_SE_SY_TRE_BOX_APPEAR

        sInCustomSong = songPlayed - 0x80;

        switch (sInCustomSong)
        {
        case CUSTOM_SONG_SOARING:
            SetupSoaring(play);
            break;
        case CUSTOM_SONG_ELEGY:
            HandleElegy(play);
            break;
        }

        return 0xfe;
    }

    return songPlayed;
}

void Ocarina_HandleCustomSongs(Actor_Player* player, GameState_Play* play)
{
    switch (sInCustomSong)
    {
    case CUSTOM_SONG_SOARING:
        HandleSoaring(play);
        break;
    case CUSTOM_SONG_TIME:
        HandleSongOfTime(play);
        break;
    }
}
