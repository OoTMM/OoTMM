#include <combo.h>

const char* warpTexts[6] = {
    TEXT_COLOR_GREEN "Warp to the Lost Woods?",
    TEXT_COLOR_RED "Warp to the Death Mountain" TEXT_NL "Crater?",
    TEXT_COLOR_BLUE "Warp to Lake Hylia?",
    TEXT_COLOR_ORANGE "Warp to the Desert Colossus?",
    TEXT_COLOR_PINK "Warp to the graveyard?",
    TEXT_COLOR_TEAL "Warp to the Temple of Time?",
};

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

static u8 sWarpSongPlayed = 0xff;

u8 Ocarina_BeforeSongPlayingProcessed(GameState_Play* ctxt)
{
    char* b;
    u8 songPlayed = ctxt->ocarinaStaff->state;

    if (songPlayed >= 0x80 && songPlayed <= 0x85)
    {
        PlaySound(0x4807); // NA_SE_SY_TRE_BOX_APPEAR

        u8 songIndex = songPlayed - 0x80;
        sWarpSongPlayed = songIndex;

        if (ctxt->interfaceCtx.restrictions.songOfSoaring)
        {
            PlayerDisplayTextBox(ctxt, 0x1B95, NULL);
            ctxt->ocarinaMode = 0x27; // OCARINA_MODE_PROCESS_RESTRICTED_SONG
            return 0xFE;
        }

        PlayerDisplayTextBox(ctxt, 0x1B93, NULL); // Soar to X?
        b = ctxt->textBuffer;
        b += 11;
        comboTextAppendStr(&b, warpTexts[songIndex]);
        if (songIndex != 1)
        {
            comboTextAppendStr(&b, TEXT_NL " ");
        }
        comboTextAppendStr(&b, TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "     OK" TEXT_NL "     No" TEXT_END);

        ctxt->ocarinaMode = 1; // OCARINA_MODE_ACTIVE
        return 0xfe;
    }

    return songPlayed;
}

void Ocarina_HandleWarp(Actor_Player* player, GameState_Play* play)
{
    int messageState;
    if (sWarpSongPlayed >= 0 && sWarpSongPlayed <= 5)
    {
        messageState = Message_GetState((char*)play + 0x4908);
        if (messageState == 4) // TEXT_STATE_CHOICE
        {
            AudioOcarina_SetInstrument(0);
        }

        if (play->ocarinaMode == 0x27) // OCARINA_MODE_PROCESS_RESTRICTED_SONG
        {
            if (messageState == 6) // TEXT_STATE_DONE
            {
                AudioOcarina_SetInstrument(0);
                sWarpSongPlayed = 0xFF;
            }
        }

        if (play->ocarinaMode == 2) // OCARINA_MODE_WARP
        {
            play->interfaceCtx.unk_222 = 0;
            ActorCutscene_Stop(play->playerActorCsIds[0]);
            player->base.flags &= ~0x20000000; // ACTOR_FLAG_20000000
            Actor* actor = SpawnActor((char*)play + 0x1ca0, play, 0x1CE, player->base.position.x, player->base.position.y, player->base.position.z, 0, 0, 0, 0x8000 | sWarpSongPlayed);
            if (actor)
            {
                player->state &= ~0x20000000; // PLAYER_STATE1_TIME_STOP
                player->csMode = 0; // csMode = PLAYER_CSMODE_0;

                void (*Player_func_8085B28C)(GameState_Play* play, Actor_Player* link, s32 csMode);
                Player_func_8085B28C = OverlayAddr(0x8085B28C);
                Player_func_8085B28C(play, NULL, 19);

                player->state |= 0x30000000; // PLAYER_STATE1_SPECIAL_2 | PLAYER_STATE1_TIME_STOP
            }
            else
            {
                // TODO not sure what this is for in the vanilla code. perhaps to prevent the player softlocking in the case the soaring actor fails to spawn?
                /*
                func_80836A5C(this, play);
                Player_AnimationPlayOnceReverse(play, this, D_8085D17C[this->transformation]);
                */
            }
            sWarpSongPlayed = 0xFF;
        }
        else if (play->ocarinaMode == 4) // OCARINA_MODE_END
        {
            sWarpSongPlayed = 0xFF;
        }
    }
}

void Ocarina_CheckCustomSongs(void)
{
    u32 enabledWarpSongs = (gOotSave.inventory.quest.value >> 6) & 0x3f;
    u8 songIndex;

    if (!comboConfig(CFG_OOT_CROSS_WARP))
        return;

    for (songIndex = 0; songIndex < 6; songIndex++)
    {
        if (enabledWarpSongs & (1 << songIndex))
            comboCheckSong(&sWarpSongs[songIndex], songIndex);
    }
}
