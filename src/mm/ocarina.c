#include <combo.h>

const char* warpTexts[6] = {
    TEXT_COLOR_GREEN "Warp to the Lost Woods?",
    TEXT_COLOR_RED "Warp to the Death Mountain" TEXT_NL "Crater?",
    TEXT_COLOR_BLUE "Warp to Lake Hylia?",
    TEXT_COLOR_ORANGE "Warp to the Desert Colossus?",
    TEXT_COLOR_PINK "Warp to the graveyard?",
    TEXT_COLOR_TEAL "Warp to the Temple of Time?",
};

static u8 sWarpSongPlayed = 0xFF;
u8 Ocarina_BeforeSongPlayingProcessed(GameState_Play* ctxt) {
    char* b;
    u8 songPlayed = ctxt->ocarinaStaff->state;

    if (songPlayed >= 0x80 && songPlayed <= 0x85) {
        PlaySound(0x4807); // NA_SE_SY_TRE_BOX_APPEAR

        if (ctxt->interfaceCtx.restrictions.songOfSoaring) {
            DisplayTextBox2(ctxt, 0x1B95);
            ctxt->ocarinaMode = 0x27; // OCARINA_MODE_PROCESS_RESTRICTED_SONG
            return 0xFE;
        }

        u8 songIndex = songPlayed - 0x80;

        DisplayTextBox2(ctxt, 0x1B93); // Soar to X?
        b = ctxt->textBuffer;
        comboTextAppendHeader(&b);
        comboTextAppendStr(&b, warpTexts[songIndex]);
        if (songIndex != 1) {
            comboTextAppendStr(&b, TEXT_NL " ");
        }
        comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "     OK" TEXT_NL "     No" TEXT_END);

        ctxt->ocarinaMode = 1; // OCARINA_MODE_ACTIVE
        sWarpSongPlayed = songPlayed - 0x80;
        return 0xFE;
    }

    return songPlayed;
}

void Ocarina_HandleWarp(Actor_Player* player, GameState_Play* ctxt) {
    if (ctxt->ocarinaMode == 2 && sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5) { // WARP
        ctxt->interfaceCtx.unk_222 = 0;
        ActorCutscene_Stop(ctxt->playerActorCsIds[0]);
        player->base.flags &= ~0x20000000; // ACTOR_FLAG_20000000
        Actor* actor = SpawnActor((char*)ctxt + 0x1ca0, ctxt, 0x1CE, player->base.position.x, player->base.position.y, player->base.position.z, 0, 0, 0, 0x8000 | sWarpSongPlayed);
        if (actor != NULL) {
            player->state &= ~0x20000000; // PLAYER_STATE1_TIME_STOP
            player->csMode = 0; // csMode = PLAYER_CSMODE_0;

            void (*Player_func_8085B28C)(GameState_Play* play, Actor_Player* link, s32 csMode);
            Player_func_8085B28C = OverlayAddr(0x8085B28C);
            Player_func_8085B28C(ctxt, NULL, 19);

            player->state |= 0x30000000; // PLAYER_STATE1_SPECIAL_2 | PLAYER_STATE1_TIME_STOP
        } else {
            // TODO not sure what this is for in the vanilla code. perhaps to prevent the player softlocking in the case the soaring actor fails to spawn?
            /*
            func_80836A5C(this, play);
            Player_AnimationPlayOnceReverse(play, this, D_8085D17C[this->transformation]);
            */
        }
        sWarpSongPlayed = 0xFF;
    }
}

static OcarinaSongButtons sWarpSongs[6] = {
    // MINUET_OF_FOREST
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

    // BOLERO_OF_FIRE
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

    // SERENADE_OF_WATER
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

    // REQUIEM_OF_SPIRIT
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

    // NOCTURNE_OF_SHADOW
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

    // PRELUDE_OF_LIGHT
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

void Ocarina_CheckCustomSongs(void) {
    u8 songIndex;
    u8 j;
    u8 k;
    u32 pitch;
    u32 enabledWarpSongs = (gOotSave.inventory.quest.value >> 6) & 0x3F;
    u8 sOcarinaWithoutMusicStaffPos = *(u8*)0x801D7028;
    u8* sCurOcarinaSongWithoutMusicStaff = (u8*)0x801D7020;
    u8* sButtonToPitchMap = (u8*)0x801D7030;
    u8* sPlayedOcarinaSongIndexPlusOne = (u8*)0x801D8528;
    u8* sIsOcarinaInputEnabled = (u8*)0x801D6FB4;
    s32* sOcarinaFlags = (s32*)0x801D6FEC;

    for (songIndex = 0; songIndex < 6; songIndex++) {
        if (enabledWarpSongs & (1 << songIndex)) {
            for (j = 0, k = 0; j < sWarpSongs[songIndex].numButtons && k == 0 &&
                                sOcarinaWithoutMusicStaffPos >= sWarpSongs[songIndex].numButtons;) {

                pitch = sCurOcarinaSongWithoutMusicStaff[(sOcarinaWithoutMusicStaffPos -
                                                            sWarpSongs[songIndex].numButtons) +
                                                            j];

                if (pitch == sButtonToPitchMap[sWarpSongs[songIndex].buttonIndex[j]]) {
                    j++;
                } else {
                    k++;
                }
            }

            if (j == sWarpSongs[songIndex].numButtons) {
                *sPlayedOcarinaSongIndexPlusOne = songIndex + 0x80 + 1;
                *sIsOcarinaInputEnabled = 0;
                *sOcarinaFlags = 0;
            }
        }
    }
}
