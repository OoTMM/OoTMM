#include <combo.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/data/song_events.h>
#include "ovl/actors/Custom_Song_Tag/Custom_Song_Tag.h"

extern u32 gOcarinaPressedButtons;

const char* warpTexts[6] = {
    TEXT_COLOR_GREEN "Warp to the Lost Woods?",
    TEXT_COLOR_RED "Warp to the Death Mountain" TEXT_NL "Crater?",
    TEXT_COLOR_BLUE "Warp to Lake Hylia?",
    TEXT_COLOR_YELLOW "Warp to the Desert Colossus?",
    TEXT_COLOR_PINK "Warp to the graveyard?",
    TEXT_COLOR_TEAL "Warp to the Temple of Time?",
};

typedef enum {
    CUSTOM_SONG_MINUET,
    CUSTOM_SONG_BOLERO,
    CUSTOM_SONG_SERENADE,
    CUSTOM_SONG_REQUIEM,
    CUSTOM_SONG_NOCTURNE,
    CUSTOM_SONG_PRELUDE,
    CUSTOM_SONG_ZELDAS_LULLABY,
    CUSTOM_SONG_SARIAS,
    CUSTOM_SONG_MAX,
    CUSTOM_SONG_NONE = 0xff,
} CustomOcarinaSong;


static OcarinaSongButtons sCustomSongs[8] = {
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

    /* OCARINA_SONG_ZELDA_LULLABY */
    {
        6,
        {
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_UP,
            OCARINA_BTN_C_RIGHT,
        }
    },

    /* OCARINA_SONG_SARIAS */
    {
        6,
        {
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
            OCARINA_BTN_C_DOWN,
            OCARINA_BTN_C_RIGHT,
            OCARINA_BTN_C_LEFT,
        }
    },
};

static u8 sCustomSongPlayed = 0xff;

u8 Ocarina_BeforeSongPlayingProcessed(PlayState* ctxt)
{
    u8 songPlayed = ctxt->msgCtx.songInfo->frameInfo[0].storedSong;

    if (songPlayed >= 0x80 && songPlayed < 0x80 + CUSTOM_SONG_MAX)
    {
        PlaySound(0x4807); /* NA_SE_SY_TRE_BOX_APPEAR */

        u8 songIndex = songPlayed - 0x80;
        sCustomSongPlayed = songIndex;

        DisplayTextBox2(ctxt, 0x1b5b);
        ctxt->msgCtx.msgMode = 18; /* MSGMODE_SONG_PLAYED */
        ctxt->msgCtx.textBoxType = 3; /* TEXTBOX_TYPE_3 */
        ctxt->msgCtx.stateTimer = 10;
        Interface_SetHudVisibility(1); /* HUD_VISIBILITY_NONE */

        switch (sCustomSongPlayed)
        {
        case CUSTOM_SONG_ZELDAS_LULLABY:
            ctxt->msgCtx.ocarinaSong = OCARINA_SONG_ZELDAS_LULLABY;
            break;
        case CUSTOM_SONG_SARIAS:
            ctxt->msgCtx.ocarinaSong = OCARINA_SONG_SARIAS;
            break;
        default:
            ctxt->msgCtx.ocarinaSong = OCARINA_SONG_CUSTOM_START + sCustomSongPlayed;
            break;
        }

        return 0xfe;
    }

    return songPlayed;
}

void Ocarina_HandleWarp(Player* player, PlayState* play)
{
    int messageState;
    if (sCustomSongPlayed >= CUSTOM_SONG_MINUET && sCustomSongPlayed <= CUSTOM_SONG_PRELUDE)
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
                sCustomSongPlayed = 0xFF;
            }
        }

        if (play->msgCtx.ocarinaMode == 2) /* OCARINA_MODE_WARP */
        {
            play->interfaceCtx.bButtonInterfaceDoActionActive = 0;
            ActorCutscene_Stop(play->playerActorCsIds[0]);
            player->actor.flags &= ~ACTOR_FLAG_MM_20000000;
            Actor* actor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TEST7, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, 0x8000 | sCustomSongPlayed);
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
            sCustomSongPlayed = 0xFF;
        }
        else if (play->msgCtx.ocarinaMode == 4) /* OCARINA_MODE_END */
        {
            sCustomSongPlayed = 0xFF;
        }
    }
}

void Ocarina_CheckCustomSongs(void)
{
    u8 enabledCustomSongs = gCustomSave.ootSongs.value;
    u8 songIndex;

    for (songIndex = 0; songIndex < ARRAY_COUNT(sCustomSongs); songIndex++)
    {
        if (enabledCustomSongs & (1 << songIndex))
            comboCheckSong(&sCustomSongs[songIndex], songIndex);
    }
}

void OcarinaMaskButtons(void)
{
    u32 mask;

    mask = ~(A_BUTTON | U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS);
    mask |= gSharedCustomSave.ocarinaButtonMaskMm;
    gOcarinaPressedButtons &= mask;
}

static s16 sCustomSongActorEffectIds[CUSTOM_SONG_MAX] = {
    ACTOR_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE,
    ACTOR_OCEFF_WIPE,
    ACTOR_OCEFF_WIPE3,
};

static u16 sCustomSongActorEffectParams[CUSTOM_SONG_MAX] = {
    0,
    1,
    2,
    3,
    4,
    1,
    0,
    0
};

void Ocarina_SpawnSongEffect(PlayState* play)
{
    if (sCustomSongPlayed >= CUSTOM_SONG_MINUET && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        Player* player = GET_PLAYER(play);
        s16 actorId = sCustomSongActorEffectIds[sCustomSongPlayed];
        if (actorId >= 0)
        {
            play->msgCtx.ocarinaSongEffectActive = 1;
            u16 actorParams = sCustomSongActorEffectParams[sCustomSongPlayed];
            Actor_Spawn(&play->actorCtx, play, actorId, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, actorParams);
        }
    }
    else
    {
        Message_SpawnSongEffect(play);
    }
}

PATCH_CALL(0x80155180, Ocarina_SpawnSongEffect)

static OcarinaNote sOcarinaCustomSongNotes[CUSTOM_SONG_MAX][20] = {
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

    // OCARINA_SONG_LULLABY
    {
        { OCARINA_PITCH_B4, 51, 84, 0, 0, 0 },
        { OCARINA_PITCH_D5, 25, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 78, 80, 0, 0, 0 },
        { OCARINA_PITCH_B4, 51, 84, 0, 0, 0 },
        { OCARINA_PITCH_D5, 25, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 100, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_SARIAS
    {
        { OCARINA_PITCH_F4, 17, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 17, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 34, 80, 0, 0, 0 },
        { OCARINA_PITCH_F4, 17, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 17, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 136, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },
};

void Ocarina_SetPlaybackSong(s8 songIndexPlusOne, u8 playbackState)
{
    if (sCustomSongPlayed >= 0 && sCustomSongPlayed <= ARRAY_COUNT(sOcarinaCustomSongNotes))
    {
        OcarinaNote** sPlaybackSong = (OcarinaNote**)0x801d84f0;
        AudioOcarina_SetPlaybackSong(1, playbackState);
        *sPlaybackSong = sOcarinaCustomSongNotes[sCustomSongPlayed];
    }
    else
    {
        AudioOcarina_SetPlaybackSong(songIndexPlusOne, playbackState);
    }
}

PATCH_CALL(0x80155248, Ocarina_SetPlaybackSong);

static u16 sOcarinaCustomSongFanfares[CUSTOM_SONG_MAX] = {
    FANFARE_SONG_TP_FOREST,
    FANFARE_SONG_TP_FIRE,
    FANFARE_SONG_TP_WATER,
    FANFARE_SONG_TP_SPIRIT,
    FANFARE_SONG_TP_SHADOW,
    FANFARE_SONG_TP_LIGHT,
    FANFARE_SONG_ZELDA,
    FANFARE_SONG_SARIA,
};

void Ocarina_PlayFanfareWithPlayerIOPort7(u16 seqId, u8 ioData)
{
    if (sCustomSongPlayed >= 0 && sCustomSongPlayed <= ARRAY_COUNT(sOcarinaCustomSongFanfares))
    {
        Audio_PlayFanfare(sOcarinaCustomSongFanfares[sCustomSongPlayed]);
    }
    else
    {
        Audio_PlayFanfareWithPlayerIOPort7(seqId, ioData);
    }
}

PATCH_CALL(0x80155294, Ocarina_PlayFanfareWithPlayerIOPort7)

const char* sCustomSongPlayedText[CUSTOM_SONG_MAX] = {
    "the " TEXT_COLOR_GREEN "Minuet of Forest",
    "the " TEXT_COLOR_RED "Bolero of Fire",
    "the " TEXT_COLOR_BLUE "Serenade of Water",
    "the " TEXT_COLOR_YELLOW "Requiem of Spirit",
    "the " TEXT_COLOR_PINK "Nocturne of Shadow",
    "the " TEXT_COLOR_TEAL "Prelude of Light",
    TEXT_COLOR_BLUE "Zelda's Lullaby",
    TEXT_COLOR_GREEN "Saria's Song",
};

void Ocarina_DisplaySongPlayedTextbox(PlayState* play, u16 textId)
{
    char* b;
    if (sCustomSongPlayed >= 0 && sCustomSongPlayed <= CUSTOM_SONG_MAX)
    {
        DisplayTextBox2(play, 0x1b78);
        b = play->msgCtx.font.textBuffer.schar;
        b += 11;
        comboTextAppendStr(&b, "You played ");
        comboTextAppendStr(&b, sCustomSongPlayedText[sCustomSongPlayed]);
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
    switch (sCustomSongPlayed)
    {
    case CUSTOM_SONG_MINUET:
    case CUSTOM_SONG_BOLERO:
    case CUSTOM_SONG_SERENADE:
    case CUSTOM_SONG_REQUIEM:
    case CUSTOM_SONG_NOCTURNE:
    case CUSTOM_SONG_PRELUDE:
        if (gPlay->interfaceCtx.restrictions.songOfSoaring || Config_Flag(CFG_ONLY_MM))
        {
            PlayerDisplayTextBox(gPlay, 0x1B95, NULL);
            gPlay->msgCtx.ocarinaMode = 0x27; /* OCARINA_MODE_PROCESS_RESTRICTED_SONG */
            return -1;
        }

        PlayerDisplayTextBox(gPlay, 0x1B93, NULL); /* Soar to X? */
        b = gPlay->msgCtx.font.textBuffer.schar;
        b += 11;
        comboTextAppendStr(&b, warpTexts[sCustomSongPlayed]);
        if (sCustomSongPlayed != 1)
        {
            comboTextAppendStr(&b, TEXT_NL " ");
        }
        comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "     OK" TEXT_NL "     No" TEXT_END);

        gPlay->msgCtx.ocarinaMode = 1; /* OCARINA_MODE_ACTIVE */
        return -1;
    case CUSTOM_SONG_ZELDAS_LULLABY:
        sCustomSongPlayed = 0xFF;
        return 0;
    case CUSTOM_SONG_SARIAS:
        Audio_PlaySubBgm(NA_BGM_SARIAS_SONG);
        sCustomSongPlayed = 0xFF;
        return 0;
    }
    return lastPlayedSong;
}

static u8 sSongEventSongs[] = {
    OCARINA_SONG_ZELDAS_LULLABY,
    OCARINA_SONG_EPONAS,
    OCARINA_SONG_SARIAS,
    OCARINA_SONG_STORMS,
    OCARINA_SONG_SUNS,
    OCARINA_SONG_TIME,
    OCARINA_SONG_CUSTOM_MINUET,
    OCARINA_SONG_CUSTOM_BOLERO,
    OCARINA_SONG_CUSTOM_SERENADE,
    OCARINA_SONG_CUSTOM_REQUIEM,
    OCARINA_SONG_CUSTOM_NOCTURNE,
    OCARINA_SONG_CUSTOM_PRELUDE,
    OCARINA_SONG_HEALING,
    OCARINA_SONG_SOARING,
    OCARINA_SONG_SONATA,
    OCARINA_SONG_GORON_LULLABY,
    OCARINA_SONG_GORON_LULLABY_INTRO,
    OCARINA_SONG_NEW_WAVE,
    OCARINA_SONG_ELEGY,
    OCARINA_SONG_OATH,
};

s32 Ocarina_CheckSongEventSong(u16 ocarinaSong, int songEventId)
{
    u8 expectedSong = sSongEventSongs[gComboConfig.songEventsMm[songEventId]];
    if (ocarinaSong == expectedSong || (expectedSong == OCARINA_SONG_GORON_LULLABY_INTRO && ocarinaSong == OCARINA_SONG_GORON_LULLABY)) {
        return true;
    }
    return false;
}

void Ocarina_ClearLastPlayedSong(PlayState* play)
{
    s16* sLastPlayedSong = (s16*)0x801c6a7c;
    *sLastPlayedSong = 0xff;
    sCustomSongPlayed = CUSTOM_SONG_NONE;
    play->msgCtx.blockSunsSong = true;
}

void comboSpawnCustomSongTags(PlayState* play)
{
    if (!Config_Flag(CFG_MM_SONG_EVENTS_SHUFFLE)) {
        return;
    }
    int variable;
    float x;
    float y;
    float z;
    s16 ry = 0;
    u8 room = 0xff;

    variable = -1;

#if defined(GAME_MM)
    if (play->sceneId == SCE_MM_WOODFALL)
    {
        variable = SONG_EVENT_TEMPLE_WOODFALL;
        x = 0.f;
        y = 212.f;
        z = 1040.f;
    }

    if (play->sceneId == SCE_MM_SNOWHEAD)
    {
        variable = SONG_EVENT_TEMPLE_SNOWHEAD;
        x = -554.f;
        y = 214.f;
        z = 6303.f;
    }

    if (play->sceneId == SCE_MM_ZORA_CAPE)
    {
        variable = SONG_EVENT_TEMPLE_GREATBAY;
        x = -5582.f;
        y = 14.f;
        z = 1610.f;
        ry = DEG_TO_BINANG(90);
    }

    /*
    if (play->sceneId == SCE_MM_GHOST_HUT)
    {
        variable = SONG_EVENT_HEALING_POEHUT;
        x = 0.f;
        y = 0.f;
        z = 1040.f;
    }
    */

    if (play->sceneId == SCE_MM_GORON_GRAVEYARD)
    {
        variable = SONG_EVENT_HEALING_DARMANI;
        x = 0.f;
        y = 10.f;
        z = 80.f;
        ry = DEG_TO_BINANG(180);
    }

    if (play->sceneId == SCE_MM_MUSIC_BOX_HOUSE)
    {
        variable = SONG_EVENT_HEALING_PAMELA_FATHER;
        x = 1.f;
        y = -140.f;
        z = 11.f;
        ry = DEG_TO_BINANG(180);
    }

    if (play->sceneId == SCE_MM_TERMINA_FIELD)
    {
        variable = SONG_EVENT_HEALING_KAMARO;
        x = -2533.f;
        y = 47.f;
        z = -2978.f;
    }

    if (play->sceneId == SCE_MM_GREAT_BAY_COAST)
    {
        variable = SONG_EVENT_HEALING_MIKAU;
        x = -305.f;
        y = 80.f;
        z = 4793.f;
    }

    if (play->sceneId == SCE_MM_IKANA_GRAVEYARD)
    {
        variable = SONG_EVENT_AWAKENING_KEETA;
        x = -215.f;
        y = 313.f;
        z = -2140.f;
        room = 1;
    }

    /*
    if (play->sceneId == SCE_MM_SPIDER_HOUSE_SWAMP)
    {
        variable = SONG_EVENT_AWAKENING_SCRUB;
        x = 556.f;
        y = 0.f;
        z = -1035.f;
        room = 4;
    }
    */

    if (play->sceneId == SCE_MM_GORON_SHRINE)
    {
        variable = SONG_EVENT_LULLABY_KID;
        x = -1690.f;
        y = -133.f;
        z = 0.f;
        ry = DEG_TO_BINANG(90);
        room = 1;
    }

    if (play->sceneId == SCE_MM_IKANA_CANYON)
    {
        variable = SONG_EVENT_STORMS_COMPOSER;
        x = -173.f;
        y = 573.f;
        z = -1793.f;
        room = 1;
    }

    if (play->sceneId == SCE_MM_CLOCK_TOWER_ROOFTOP)
    {
        variable = SONG_EVENT_CLOCK_TOWER_ROOF;
        x = 0.f;
        y = 20.f;
        z = 222.f;
    }
#endif

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_LAKE_HYLIA)
    {
        variable = SWITCH_LAKE_HYLIA_WATER;
        x = -850.f;
        y = -1223.f;
        z = 6950.f;
    }
#endif

    if (variable == -1)
        return;

    Actor_CustomSongTag* songTag = (Actor_CustomSongTag*) Actor_Spawn(
        &play->actorCtx,
        play,
        ACTOR_CUSTOM_SONG_TAG,
        x, y, z,
        0, ry, 0,
        variable
    );
    if (songTag)
    {
        songTag->intendedRoom = room;
    }
}
