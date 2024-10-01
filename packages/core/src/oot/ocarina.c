#include <combo.h>
#include <combo/entrance.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/actor.h>
#include <combo/math.h>

typedef enum {
    CUSTOM_SONG_NONE,
    CUSTOM_SONG_SOARING,
    CUSTOM_SONG_TIME,
    CUSTOM_SONG_ELEGY,
    CUSTOM_SONG_DOUBLE_TIME,
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

static OcarinaSongButtons sSongDoubleTime = {
    6,
    {
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_A,
        OCARINA_BTN_A,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_DOWN,
    }
};

void Ocarina_CheckCustomSongs(void)
{
    if (gMmSave.info.inventory.quest.songSoaring
        && Config_Flag(CFG_MM_CROSS_WARP)
        && (Config_Flag(CFG_MM_CROSS_WARP_ADULT) || gSave.age == AGE_CHILD))
    {
        comboCheckSong(&sSongSoaring, CUSTOM_SONG_SOARING);
    }

    if (gCustomSave.hasElegy)
    {
        comboCheckSong(&sSongElegy, CUSTOM_SONG_ELEGY);
    }

    if (gSave.info.inventory.quest.songTime && Config_Flag(CFG_OOT_SONG_OF_DOUBLE_TIME))
    {
        comboCheckSong(&sSongDoubleTime, CUSTOM_SONG_DOUBLE_TIME);
    }
}

void OcarinaMaskButtons(void)
{
    u32 mask;

    mask = ~(A_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS);
    mask |= gSharedCustomSave.ocarinaButtonMaskOot;
    gOcarinaPressedButtons &= mask;
}

static void changeAgeMessage(PlayState* play, s32 toChild)
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

void Ocarina_HandleLastPlayedSong(PlayState* play, Player* player, s16 lastPlayedSong)
{
    s32 canChangeAge;

    switch (lastPlayedSong)
    {
    /* Displaced code: */
    case OCARINA_SONG_SARIAS:
        player->naviTextId = -0xE0;
        player->naviActor->flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        break;
    /* End displaced code. */
    case OCARINA_SONG_TIME:
        canChangeAge = Config_Flag(CFG_OOT_AGE_CHANGE) && GetEventChk(EV_OOT_CHK_MASTER_SWORD_CHAMBER) && GetEventChk(EV_OOT_CHK_MASTER_SWORD_PULLED);
        if (canChangeAge && Config_Flag(CFG_OOT_AGE_CHANGE_NEEDS_OOT) && gSave.info.inventory.items[ITS_OOT_OCARINA] != ITEM_OOT_OCARINA_TIME)
            canChangeAge = 0;
#if defined(DEBUG)
        canChangeAge = 1;
#endif
        if (play->msgCtx.ocarinaAction != 0x29) /* OCARINA_ACTION_FREE_PLAY_DONE */
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
                if (actor->id == ACTOR_OBJ_TIMEBLOCK || actor->id == ACTOR_OBJ_WARP2BLOCK)
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

static void soaringNoStatuesMessage(PlayState* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You have yet to leave your " TEXT_COLOR_RED "mark" TEXT_CZ TEXT_NL);
    comboTextAppendStr(&b, "on any of the " TEXT_COLOR_RED "statues" TEXT_CZ " you have" TEXT_NL);
    comboTextAppendStr(&b, "come across. Hurry up and find" TEXT_NL);
    comboTextAppendStr(&b, "some." TEXT_END);
}

static void HandleSoaring(PlayState* play)
{
    Player* link;
    int songId;
    if (play->pauseCtx.state == 0)
    {
        if (gSoaringIndexSelected >= 0 && gSoaringIndexSelected < 10)
        {
            songId = gSoaringIndexSelected;

            /* Stop ocarina */
            sInCustomSong = CUSTOM_SONG_NONE;
            play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
            gSoaringIndexSelected = -1;

            link = GET_PLAYER(play);
            link->stateFlags1 |= (PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_FROZEN);
            link->actor.freezeTimer = 10000;
            u32 entrance = gComboConfig.entrancesOwl[songId] ^ MASK_FOREIGN_ENTRANCE;
            comboTransition(play, entrance);
        }
        else
        {
            /* Stop ocarina */
            sInCustomSong = CUSTOM_SONG_NONE;
            play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
        }
    }
}

void ageSwap(PlayState* play)
{
    /* Age swap */
    play->linkAgeOnLoad = !gSaveContext.save.age;
    Play_SetupRespawnPoint(play, 1, 0xdff);
    gSaveContext.respawnFlag = 2;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->nextEntranceIndex = gSaveContext.save.entrance;
    play->transitionType = TRANS_GFX_SHORTCUT;

    /* Set the correct farore */
    swapFarore();
}

static void HandleSongOfTime(PlayState* play)
{
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState == 2)
    {
        /* Stop ocarina */
        sInCustomSong = CUSTOM_SONG_NONE;

        if (play->msgCtx.choiceIndex == 0)
        {
            ageSwap(play);
        }
        else
        {
            PlaySound(0x480A);
            play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
        }
    }
}

static void PrepareSoaringScreen(PlayState* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;

    /* Rumble_StateReset(); */

    pauseCtx->switchPageTimer = 0;
    pauseCtx->mainState = 1; /* PAUSE_MAIN_STATE_SWITCHING_PAGE; */

    /* Set eye position and pageIndex such that scrolling left brings to the desired page */
    pauseCtx->eye.x = -64.0f; /* sKaleidoSetupEyeX1[pauseCtx->pageIndex]; */
    pauseCtx->eye.z = 0.0f; /* sKaleidoSetupEyeZ1[pauseCtx->pageIndex]; */
    pauseCtx->screen_idx = 1 ; /* sKaleidoSetupKscpPos1[pauseCtx->pageIndex]; */
    pauseCtx->infoPanelOffsetY = -40;

    for (i = 0; i < ARRAY_COUNT(pauseCtx->worldMapPoints); i++) {
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

static void SetupSoaring(PlayState* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;

    if (play->interfaceCtx.restrictions.warpSongs)
    {
        PlayerDisplayTextBox(play, 0x88c, NULL);
    }
    else
    {
        /* if in a dungeon or boss scene */
        /* { */
        /*      "Soar to Dungeon Entrance?" */
        /* } */
        /* else */
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
            gSaveContext.nextHudVisibilityMode = 0x1; /* HUD_VISIBILITY_NOTHING; */
            R_UPDATE_RATE = 2;
            if (Letterbox_GetSizeTarget() != 0) {
                Letterbox_SetSizeTarget(0);
            }
            func_800C7200(1);
            play->msgCtx.msgLength = 0;
            play->msgCtx.msgMode = 0; /* MSGMODE_NONE */
        }
    }
}

Actor_CustomEnTorch2* gElegyShell;

static void HandleElegy(PlayState* play)
{
    sInCustomSong = CUSTOM_SONG_NONE;
    Player* player = GET_PLAYER(play);

    if (gElegyShell != NULL)
    {
        Math_Vec3f_Copy(&gElegyShell->base.home.pos, &player->actor.world.pos);
        gElegyShell->base.home.rot.y = player->actor.shape.rot.y;
        gElegyShell->state = 0;
        gElegyShell->framesUntilNextState = 20;
    }
    else
    {
        gElegyShell = (Actor_CustomEnTorch2*)Actor_Spawn(&play->actorCtx, play, ACTOR_CUSTOM_TORCH2, player->actor.world.pos.x,
                                        player->actor.world.pos.y, player->actor.world.pos.z, 0, player->actor.shape.rot.y, 0, 0);
    }

    Actor_DemoEffect* effect = (Actor_DemoEffect*)Actor_Spawn(&play->actorCtx, play, ACTOR_DEMO_EFFECT, player->actor.world.pos.x, player->actor.world.pos.y,
                                player->actor.world.pos.z, 0, player->actor.shape.rot.y, 0, DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL);

    if (effect != NULL)
    {
        effect->envXluColor[0] = 0;
        effect->envXluColor[1] = 100;
        effect->envXluColor[2] = 0;
    }

    play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
    Message_Close(play);
}

static void songOfDoubleTimeMessage(PlayState* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Proceed to " TEXT_COLOR_RED);
    if (gSave.isNight)
    {
        comboTextAppendStr(&b, "Dawn");
    }
    else
    {
        comboTextAppendStr(&b, "Dusk");
    }
    comboTextAppendStr(&b, TEXT_CZ "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
}

static void songOfDoubleTimeFailMessage(PlayState* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Your notes echoed far.... " TEXT_NL "but nothing happened." TEXT_FADE("\x28") TEXT_END);
}

static void SetupSongOfDoubleTime(PlayState* play)
{
    if (gSaveContext.sunSongState != 0 || (play->envCtx.sceneTimeSpeed == 0 && (play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_1 || play->interfaceCtx.restrictions.sunSong == 3)))
    {
        PlayerDisplayTextBox(play, 0x88c, NULL);
        songOfDoubleTimeFailMessage(play);
        play->msgCtx.ocarinaMode = 4;
        sInCustomSong = CUSTOM_SONG_NONE;
    }
    else
    {
        PlayerDisplayTextBox(play, 0xe0, NULL); /* You want to talk to Saria, right? */
        songOfDoubleTimeMessage(play);
    }
}

static void HandleSongOfDoubleTime(PlayState* play)
{
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState == 2)
    {
        /* Stop ocarina */
        play->msgCtx.ocarinaMode = 4;
        sInCustomSong = CUSTOM_SONG_NONE;

        /* Check the selected option */
        if (play->msgCtx.choiceIndex == 0)
        {
            if (play->envCtx.sceneTimeSpeed != 0)
            {
                if (gSave.isNight)
                {
                    gSaveContext.save.dayTime = CLOCK_TIME(6, 30);
                }
                else
                {
                    gSaveContext.save.dayTime = CLOCK_TIME(18, 0) + 1;
                }
                gSaveContext.skyboxTime = gSaveContext.save.dayTime;
                if (play->envCtx.timeSeqState == 4) {
                    play->envCtx.timeSeqState++;
                }
            }
            else if (play->roomCtx.curRoom.behaviorType1 != ROOM_BEHAVIOR_TYPE1_1 && play->interfaceCtx.restrictions.sunSong != 3)
            {
                if (gSave.isNight)
                {
                    gSaveContext.nextDayTime = CLOCK_TIME(6, 30);
                    play->transitionType = TRANS_GFX_FADE_BLACK_FAST;
                    gSaveContext.nextTransitionType = TRANS_GFX_BLACK;
                }
                else
                {
                    gSaveContext.nextDayTime = CLOCK_TIME(18, 0) + 1;
                    play->transitionType = TRANS_GFX_FADE_WHITE_FAST;
                    gSaveContext.nextTransitionType = TRANS_GFX_WHITE;
                }
                play->haltAllActors = 1;

                if (play->sceneId == SCE_OOT_HAUNTED_WASTELAND)
                {
                    play->transitionType = TRANS_GFX_SANDSTORM_SLOW;
                    gSaveContext.nextTransitionType = TRANS_GFX_SANDSTORM_SLOW;
                }

                gSaveContext.respawnFlag = -2;
                play->nextEntranceIndex = gSaveContext.save.entrance;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.sunSongState = 0; /* SUNSSONG_INACTIVE */
                gSaveContext.seqId = 0xff; /* NA_BGM_DISABLED */
                gSaveContext.natureAmbienceId = 0xff; /* NATURE_ID_DISABLED */
            }
        }
    }
}

u8 Ocarina_BeforeSongPlayingProcessed(PlayState* play)
{
    u8 songPlayed = play->msgCtx.ocarinaStaff->state;

    if (songPlayed >= 0x81 && songPlayed <= 0x84)
    {
        PlaySound(0x4807); /* NA_SE_SY_TRE_BOX_APPEAR */

        sInCustomSong = songPlayed - 0x80;

        switch (sInCustomSong)
        {
        case CUSTOM_SONG_SOARING:
            SetupSoaring(play);
            break;
        case CUSTOM_SONG_ELEGY:
            HandleElegy(play);
            break;
        case CUSTOM_SONG_DOUBLE_TIME:
            SetupSongOfDoubleTime(play);
            break;
        }

        return 0xfe;
    }

    return songPlayed;
}

void Ocarina_HandleCustomSongs(Player* player, PlayState* play)
{
    switch (sInCustomSong)
    {
    case CUSTOM_SONG_SOARING:
        HandleSoaring(play);
        break;
    case CUSTOM_SONG_TIME:
        HandleSongOfTime(play);
        break;
    case CUSTOM_SONG_DOUBLE_TIME:
        HandleSongOfDoubleTime(play);
        break;
    }
}
