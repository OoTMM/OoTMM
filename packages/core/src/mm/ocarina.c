#include <combo.h>
#include <combo/player.h>
#include <combo/config.h>

extern u32 gOcarinaPressedButtons;

const char* warpTexts[6] = {
    TEXT_COLOR_GREEN "Warp to the Lost Woods?",
    TEXT_COLOR_RED "Warp to the Death Mountain" TEXT_NL "Crater?",
    TEXT_COLOR_BLUE "Warp to Lake Hylia?",
    TEXT_COLOR_YELLOW "Warp to the Desert Colossus?",
    TEXT_COLOR_PINK "Warp to the graveyard?",
    TEXT_COLOR_TEAL "Warp to the Temple of Time?",
};

static OcarinaSongButtons sWarpSongs[6] = {
    /* MINUET_OF_FOREST */
    {
        6,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
        }
    },

    /* BOLERO_OF_FIRE */
    {
        8,
        {
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
        }
    },

    /* SERENADE_OF_WATER */
    {
        5,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
        }
    },

    /* REQUIEM_OF_SPIRIT */
    {
        6,
        {
            OCARINA_BTN_A,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_A,
        }
    },

    /* NOCTURNE_OF_SHADOW */
    {
        7,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_A,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_DOWN,
        }
    },

    /* PRELUDE_OF_LIGHT */
    {
        6,
        {
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
        }
    },
};

static u8 sWarpSongPlayed = 0xff;

u8 Ocarina_BeforeSongPlayingProcessed(PlayState* ctxt)
{
    u8 songPlayed = ctxt->msgCtx.songInfo->frameInfo[0].storedSong;

    if (songPlayed >= 0x80 && songPlayed <= 0x85)
    {
        PlaySound(0x4807); /* NA_SE_SY_TRE_BOX_APPEAR */

        u8 songIndex = songPlayed - 0x80;
        sWarpSongPlayed = songIndex;

        DisplayTextBox2(ctxt, 0x1b5b);
        ctxt->msgCtx.msgMode = 18; /* MSGMODE_SONG_PLAYED */
        ctxt->msgCtx.textBoxType = 3; /* TEXTBOX_TYPE_3 */
        ctxt->msgCtx.stateTimer = 10;

        return 0xfe;
    }

    return songPlayed;
}

void Ocarina_HandleWarp(Player* player, PlayState* play)
{
    int messageState;
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        messageState = Message_GetState((char*)play + 0x4908);
        if (messageState == 4) /* TEXT_STATE_CHOICE */
        {
            AudioOcarina_SetInstrument(0);
        }

        if (play->msgCtx.ocarinaMode == 0x27) /* OCARINA_MODE_PROCESS_RESTRICTED_SONG */
        {
            if (messageState == 6) /* TEXT_STATE_DONE */
            {
                AudioOcarina_SetInstrument(0);
                sWarpSongPlayed = 0xFF;
            }
        }

        if (play->msgCtx.ocarinaMode == 2) /* OCARINA_MODE_WARP */
        {
            play->interfaceCtx.bButtonInterfaceDoActionActive = 0;
            ActorCutscene_Stop(play->playerActorCsIds[0]);
            player->actor.flags &= ~ACTOR_FLAG_MM_20000000;
            Actor* actor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST7, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, 0x8000 | sWarpSongPlayed);
            if (actor)
            {
                player->stateFlags1 &= ~0x20000000; /* PLAYER_STATE1_TIME_STOP */
                player->csMode = 0; /* csMode = PLAYER_CSMODE_0; */

                void (*Player_func_8085B28C)(PlayState* play, Player* link, s32 csMode);
                Player_func_8085B28C = OverlayAddr(0x8085B28C);
                Player_func_8085B28C(play, NULL, 19);

                player->stateFlags1 |= 0x30000000; /* PLAYER_STATE1_SPECIAL_2 | PLAYER_STATE1_TIME_STOP */
            }
            else
            {
                /* TODO not sure what this is for in the vanilla code. perhaps to prevent the player softlocking in the case the soaring actor fails to spawn? */
                /*
                func_80836A5C(this, play);
                Player_AnimationPlayOnceReverse(play, this, D_8085D17C[this->transformation]);
                */
            }
            sWarpSongPlayed = 0xFF;
        }
        else if (play->msgCtx.ocarinaMode == 4) /* OCARINA_MODE_END */
        {
            sWarpSongPlayed = 0xFF;
        }
    }
}

void Ocarina_CheckCustomSongs(void)
{
    u32 enabledWarpSongs = (gOotSave.info.inventory.quest.value >> 6) & 0x3f;
    u8 songIndex;

    if (!Config_Flag(CFG_OOT_CROSS_WARP))
        return;

    for (songIndex = 0; songIndex < 6; songIndex++)
    {
        if (enabledWarpSongs & (1 << songIndex))
            comboCheckSong(&sWarpSongs[songIndex], songIndex);
    }
}

void OcarinaMaskButtons(void)
{
    u32 mask;

    mask = ~(A_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS);
    mask |= gSharedCustomSave.ocarinaButtonMaskMm;
    gOcarinaPressedButtons &= mask;
}

void Ocarina_SpawnSongEffect(PlayState* play)
{
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        Player* player = GET_PLAYER(play);
        play->msgCtx.ocarinaSongEffectActive = 1;
        if (sWarpSongPlayed < 5)
        {
            Actor_Spawn(&play->actorCtx, play, ACTOR_OCEFF_WIPE5, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, sWarpSongPlayed);
        }
        else
        {
            Actor_Spawn(&play->actorCtx, play, ACTOR_OCEFF_WIPE, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, 1);
        }
    }
    else
    {
        Message_SpawnSongEffect(play);
    }
}

PATCH_CALL(0x80155180, Ocarina_SpawnSongEffect)

OcarinaNote sOcarinaWarpSongNotes[6][20] = {
    // OCARINA_SONG_MINUET
    {
        { OCARINA_PITCH_D4, 18, 86, 0, 0, 0 },
        { OCARINA_PITCH_D5, 18, 92, 0, 0, 0 },
        { OCARINA_PITCH_B4, 72, 86, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 80, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 144, 86, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 86, 0, 0, 0 },
    },

    // OCARINA_SONG_BOLERO
    {
        { OCARINA_PITCH_F4, 15, 80, 0, 0, 0 },
        { OCARINA_PITCH_D4, 15, 72, 0, 0, 0 },
        { OCARINA_PITCH_F4, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 15, 76, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 15, 74, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 78, 0, 0, 0 },
        { OCARINA_PITCH_F4, 135, 66, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 66, 0, 0, 0 },
    },

    // OCARINA_SONG_SERENADE
    {
        { OCARINA_PITCH_D4, 36, 60, 0, 0, 0 },
        { OCARINA_PITCH_F4, 36, 78, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 82, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 82, 0, 0, 0 },
        { OCARINA_PITCH_A4, 36, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 144, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_REQUIEM
    {
        { OCARINA_PITCH_D4, 45, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 23, 86, 0, 0, 0 },
        { OCARINA_PITCH_D4, 22, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 45, 86, 0, 0, 0 },
        { OCARINA_PITCH_F4, 45, 94, 0, 0, 0 },
        { OCARINA_PITCH_D4, 180, 94, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 94, 0, 0, 0 },
    },

    // OCARINA_SONG_NOCTURNE
    {
        { OCARINA_PITCH_B4, 36, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 82, 0, 0, 0 },
        { OCARINA_PITCH_D4, 18, 60, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 90, 0, 0, 0 },
        { OCARINA_PITCH_A4, 18, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 144, 96, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 96, 0, 0, 0 },
    },

    // OCARINA_SONG_PRELUDE
    {
        { OCARINA_PITCH_D5, 15, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 45, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 15, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 15, 82, 0, 0, 0 },
        { OCARINA_PITCH_B4, 15, 86, 0, 0, 0 },
        { OCARINA_PITCH_D5, 60, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 75, 90, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },
};

void Ocarina_SetPlaybackSong(s8 songIndexPlusOne, u8 playbackState)
{
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        OcarinaNote** sPlaybackSong = (OcarinaNote**)0x801d84f0;
        AudioOcarina_SetPlaybackSong(1, playbackState);
        *sPlaybackSong = sOcarinaWarpSongNotes[sWarpSongPlayed];
    }
    else
    {
        AudioOcarina_SetPlaybackSong(songIndexPlusOne, playbackState);
    }
}

PATCH_CALL(0x80155248, Ocarina_SetPlaybackSong);

static u16 sOcarinaWarpSongFanfares[] = {
    FANFARE_SONG_TP_FOREST,
    FANFARE_SONG_TP_FIRE,
    FANFARE_SONG_TP_WATER,
    FANFARE_SONG_TP_SPIRIT,
    FANFARE_SONG_TP_SHADOW,
    FANFARE_SONG_TP_LIGHT,
};

void Ocarina_PlayFanfareWithPlayerIOPort7(u16 seqId, u8 ioData)
{
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        Audio_PlayFanfare(sOcarinaWarpSongFanfares[sWarpSongPlayed]);
    }
    else
    {
        Audio_PlayFanfareWithPlayerIOPort7(seqId, ioData);
    }
}

PATCH_CALL(0x80155294, Ocarina_PlayFanfareWithPlayerIOPort7)

const char* sWarpSongPlayedText[6] = {
    TEXT_COLOR_GREEN "Minuet of Forest",
    TEXT_COLOR_RED "Bolero of Fire",
    TEXT_COLOR_BLUE "Serenade of Water",
    TEXT_COLOR_YELLOW "Requiem of Spirit",
    TEXT_COLOR_PINK "Nocturne of Shadow",
    TEXT_COLOR_TEAL "Prelude of Light",
};

void Ocarina_DisplaySongPlayedTextbox(PlayState* play, u16 textId)
{
    char* b;
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        DisplayTextBox2(play, 0x1b78);
        b = play->msgCtx.font.textBuffer.schar;
        b += 11;
        comboTextAppendStr(&b, "You played the ");
        comboTextAppendStr(&b, sWarpSongPlayedText[sWarpSongPlayed]);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "!" TEXT_END);
    }
    else if (textId == 0x1b7d)
    {
        DisplayTextBox2(play, 0x1b78);
        b = play->msgCtx.font.textBuffer.schar;
        b += 11;
        comboTextAppendStr(&b, "You played the " TEXT_COLOR_YELLOW "Sun's Song");
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "!" TEXT_END);
    }
    else
    {
        DisplayTextBox2(play, textId);
    }
}

PATCH_CALL(0x80155334, Ocarina_DisplaySongPlayedTextbox)

s16 Ocarina_HandleLastPlayedSong(s16 lastPlayedSong)
{
    char* b;
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        if (gPlay->interfaceCtx.restrictions.songOfSoaring)
        {
            PlayerDisplayTextBox(gPlay, 0x1B95, NULL);
            gPlay->msgCtx.ocarinaMode = 0x27; /* OCARINA_MODE_PROCESS_RESTRICTED_SONG */
            return -1;
        }

        PlayerDisplayTextBox(gPlay, 0x1B93, NULL); /* Soar to X? */
        b = gPlay->msgCtx.font.textBuffer.schar;
        b += 11;
        comboTextAppendStr(&b, warpTexts[sWarpSongPlayed]);
        if (sWarpSongPlayed != 1)
        {
            comboTextAppendStr(&b, TEXT_NL " ");
        }
        comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "     OK" TEXT_NL "     No" TEXT_END);

        gPlay->msgCtx.ocarinaMode = 1; /* OCARINA_MODE_ACTIVE */
        return -1;
    }
    return lastPlayedSong;
}
