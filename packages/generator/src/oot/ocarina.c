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
    CUSTOM_SONG_HEALING,
    CUSTOM_SONG_AWAKENING,
    CUSTOM_SONG_GORON,
    CUSTOM_SONG_GORON_HALF,
    CUSTOM_SONG_ZORA,
    CUSTOM_SONG_ORDER,
    CUSTOM_SONG_MAX,
} CustomOcarinaSong;

extern u32 gOcarinaPressedButtons;
static u8 sInCustomSong = CUSTOM_SONG_NONE;
static u8 sCustomSongPlayed = CUSTOM_SONG_NONE;

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

static OcarinaSongButtons sSongHealing = {
    6,
    {
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_DOWN,
    }
};

static OcarinaSongButtons sSongAwakening = {
    7,
    {
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_A,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_A,
    }
};

static OcarinaSongButtons sSongGoron = {
    8,
    {
        OCARINA_BTN_A,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_A,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_A,
    }
};

static OcarinaSongButtons sSongGoronHalf = {
    6,
    {
        OCARINA_BTN_A,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_A,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_LEFT,
    }
};

static OcarinaSongButtons sSongZora = {
    7,
    {
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_RIGHT,
    }
};

static OcarinaSongButtons sSongOrder = {
    6,
    {
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_A,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_RIGHT,
        OCARINA_BTN_C_UP,
    }
};

void Ocarina_CheckCustomSongs(void)
{
    if (gCustomSave.hasSongSoaring
        && (Config_Flag(CFG_OOT_AGELESS_SOARING) || gSave.age == AGE_CHILD))
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

    if (gCustomSave.hasSongHealing)
    {
        comboCheckSong(&sSongHealing, CUSTOM_SONG_HEALING);
    }

    if (gCustomSave.hasSongAwakening)
    {
        comboCheckSong(&sSongAwakening, CUSTOM_SONG_AWAKENING);
    }

    if (gCustomSave.hasSongGoron)
    {
        comboCheckSong(&sSongGoron, CUSTOM_SONG_GORON);
    }
    else if (gCustomSave.hasSongGoronHalf)
    {
        comboCheckSong(&sSongGoronHalf, CUSTOM_SONG_GORON_HALF);
    }

    if (gCustomSave.hasSongZora)
    {
        comboCheckSong(&sSongZora, CUSTOM_SONG_ZORA);
    }

    if (gCustomSave.hasSongOrder)
    {
        comboCheckSong(&sSongOrder, CUSTOM_SONG_ORDER);
    }
}

#define OCARINA_SONG_MAX 14
extern u8 sOcarinaHasStartedSong;
extern s8 sCurOcarinaBendIndex;
extern u32 sOcarinaFlags;
extern u8 sPrevOcarinaPitch;
extern u8 sCurOcarinaPitch;
extern u8 sFirstOcarinaSongIndex;
extern u8 sLastOcarinaSongIndex;
extern u16 sAvailOcarinaSongFlags;
extern u16 sMusicStaffCurHeldLength[OCARINA_SONG_MAX];
extern u16 sMusicStaffExpectedLength[OCARINA_SONG_MAX];
extern OcarinaNote sOcarinaSongNotes[OCARINA_SONG_MAX][20];
extern u16 sMusicStaffPos[OCARINA_SONG_MAX];
extern u8 sMusicStaffPrevPitch;
extern u8 sMusicStaffExpectedPitch[OCARINA_SONG_MAX];
extern u8 sIsOcarinaInputEnabled;
extern u8 sStaffOcarinaPlayingPos;
extern u8 sMusicStaffNumNotesPerTest;
extern u16 sPrevOcarinaWithMusicStaffFlags;

static u16 sMusicStaffCurHeldLengthCustom[CUSTOM_SONG_MAX - 1];
static u16 sMusicStaffExpectedLengthCustom[CUSTOM_SONG_MAX - 1];
static u16 sMusicStaffPosCustom[CUSTOM_SONG_MAX - 1];
static u8 sMusicStaffExpectedPitchCustom[CUSTOM_SONG_MAX - 1];
static u16 sAvailOcarinaSongFlagsCustom;

static OcarinaNote sOcarinaCustomSongNotes[CUSTOM_SONG_MAX - 1][20] = {
    // OCARINA_SONG_SOARING
    {
        { OCARINA_PITCH_F4, 18, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 80, 0, 0, 0 },
        { OCARINA_PITCH_D5, 36, 94, 0, 0, 0 },
        { OCARINA_PITCH_F4, 18, 73, 0, 0, 0 },
        { OCARINA_PITCH_B4, 18, 76, 0, 0, 0 },
        { OCARINA_PITCH_D5, 108, 96, 2, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_TIME
    {
        { OCARINA_PITCH_A4, 32, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 65, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 33, 80, 0, 0, 0 },
        { OCARINA_PITCH_A4, 32, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 65, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 99, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_ELEGY
    {
        { OCARINA_PITCH_A4, 85, 93, 0, 0, 0 },
        { OCARINA_PITCH_B4, 43, 91, 0, 0, 0 },
        { OCARINA_PITCH_A4, 43, 93, 0, 0, 0 },
        { OCARINA_PITCH_F4, 21, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 21, 88, 0, 0, 0 },
        { OCARINA_PITCH_D5, 43, 101, 0, 0, 0 },
        { OCARINA_PITCH_B4, 85, 95, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 94, 0, 0, 0 },
    },

    // OCARINA_SONG_DOUBLE_TIME
    {
        { OCARINA_PITCH_A4, 29, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_A4, 30, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 29, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 30, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 29, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 3, 84, 0, 0, 0 },
        { OCARINA_PITCH_F4, 99, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 0, 0, 0, 0 },
    },

    // OCARINA_SONG_HEALING
    {
        { OCARINA_PITCH_B4, 32, 88, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 33, 69, 0, 0, 0 },
        { OCARINA_PITCH_B4, 32, 94, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 121, 86, 2, 0, 0 },
        { OCARINA_PITCH_NONE, 10, 84, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_SONATA
    {
        { OCARINA_PITCH_D5, 19, 92, 0, 0, 0 },
        { OCARINA_PITCH_B4, 19, 90, 0, 0, 0 },
        { OCARINA_PITCH_D5, 19, 90, 0, 0, 0 },
        { OCARINA_PITCH_B4, 38, 90, 0, 0, 0 },
        { OCARINA_PITCH_D4, 39, 92, 0, 0, 0 },
        { OCARINA_PITCH_A4, 76, 89, 0, 0, 0 },
        { OCARINA_PITCH_D4, 77, 82, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 86, 0, 0, 0 },
    },

    // OCARINA_SONG_GORON_LULLABY
    {
        { OCARINA_PITCH_D4, 41, 80, 0, 0, 0 },
        { OCARINA_PITCH_A4, 40, 72, 0, 0, 0 },
        { OCARINA_PITCH_B4, 39, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 42, 76, 0, 0, 0 },
        { OCARINA_PITCH_A4, 40, 84, 0, 0, 0 },
        { OCARINA_PITCH_B4, 39, 76, 0, 0, 0 },
        { OCARINA_PITCH_A4, 41, 84, 0, 0, 0 },
        { OCARINA_PITCH_D4, 80, 76, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 40, 76, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 66, 0, 0, 0 },
    },

    // OCARINA_SONG_GORON_LULLABY_INTRO
    {
        { OCARINA_PITCH_D4, 32, 78, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 90, 0, 0, 0 },
        { OCARINA_PITCH_B4, 33, 87, 0, 0, 0 },
        { OCARINA_PITCH_D4, 32, 92, 0, 0, 0 },
        { OCARINA_PITCH_A4, 33, 86, 0, 0, 0 },
        { OCARINA_PITCH_B4, 130, 80, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 66, 0, 0, 0 },
    },

    // OCARINA_SONG_NEW_WAVE
    {
        { OCARINA_PITCH_B4, 64, 74, 0, 0, 0 },
        { OCARINA_PITCH_D5, 13, 88, 0, 0, 0 },
        { OCARINA_PITCH_B4, 12, 90, 0, 0, 0 },
        { OCARINA_PITCH_A4, 78, 88, 0, 0, 0 },
        { OCARINA_PITCH_F4, 12, 76, 0, 0, 0 },
        { OCARINA_PITCH_B4, 13, 76, 0, 0, 0 },
        { OCARINA_PITCH_A4, 114, 76, 6, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 90, 0, 0, 0 },
    },

    // OCARINA_SONG_OATH
    {
        { OCARINA_PITCH_A4, 97, 104, 0, 0, 0 },
        { OCARINA_PITCH_F4, 48, 88, 0, 0, 0 },
        { OCARINA_PITCH_D4, 49, 78, 0, 0, 0 },
        { OCARINA_PITCH_F4, 49, 78, 0, 0, 0 },
        { OCARINA_PITCH_A4, 48, 94, 0, 0, 0 },
        { OCARINA_PITCH_D5, 97, 100, 0, 0, 0 },
        { OCARINA_PITCH_NONE, 0, 96, 0, 0, 0 },

    },
};

void Ocarina_AfterStart()
{
    sAvailOcarinaSongFlagsCustom = 0;
    for (u8 i = 0; i < CUSTOM_SONG_MAX - 1; i++) {
        sMusicStaffPosCustom[i] = 0;
        sMusicStaffCurHeldLengthCustom[i] = 0;
        sMusicStaffExpectedLengthCustom[i] = 0;
        sMusicStaffExpectedPitchCustom[i] = 0;

        u8 check = 0;
        u8 songIndex = i + 1;
        switch (songIndex)
        {
            case CUSTOM_SONG_SOARING:
                check = gCustomSave.hasSongSoaring
                    && (Config_Flag(CFG_OOT_AGELESS_SOARING) || gSave.age == AGE_CHILD);
                break;
            case CUSTOM_SONG_HEALING:
                check = gCustomSave.hasSongHealing;
                break;
            case CUSTOM_SONG_AWAKENING:
                check = gCustomSave.hasSongAwakening;
                break;
            case CUSTOM_SONG_GORON:
                check = gCustomSave.hasSongGoron;
                if (gPlay->msgCtx.ocarinaAction == OCARINA_ACTION_CHECK_CUSTOM_SONG_GORON_HALF)
                {
                    check = 0;
                }
                break;
            case CUSTOM_SONG_GORON_HALF:
                check = gCustomSave.hasSongGoronHalf && (!gCustomSave.hasSongGoron || gPlay->msgCtx.ocarinaAction == OCARINA_ACTION_CHECK_CUSTOM_SONG_GORON_HALF);
                break;
            case CUSTOM_SONG_ZORA:
                check = gCustomSave.hasSongZora;
                break;
            case CUSTOM_SONG_ELEGY:
                check = gCustomSave.hasElegy;
                break;
            case CUSTOM_SONG_ORDER:
                check = gCustomSave.hasSongOrder;
                break;
        }

        if (check)
        {
            sAvailOcarinaSongFlagsCustom |= (1 << songIndex);
        }
    }
    sCustomSongPlayed = CUSTOM_SONG_NONE;
}

PATCH_FUNC(0x800c22d4, Ocarina_AfterStart)

void Ocarina_PlayStaffAppearSound(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd)
{
    u16 ocarinaAction = gPlay->msgCtx.ocarinaAction;
    if (ocarinaAction >= OCARINA_ACTION_CHECK_MINUET && ocarinaAction <= OCARINA_ACTION_CHECK_STORMS)
    {
        Audio_PlaySfxGeneral(sfxId, pos, token, freqScale, vol, reverbAdd);
    }
    if (ocarinaAction >= OCARINA_ACTION_CHECK_CUSTOM_SONG_SOARING && ocarinaAction < OCARINA_ACTION_CHECK_MAX)
    {
        Audio_PlaySfxGeneral(sfxId, pos, token, freqScale, vol, reverbAdd);
    }
}

PATCH_CALL(0x800dd128, Ocarina_PlayStaffAppearSound)

static s32 checkSong(u8 songIndex, u8 noNewValidInput, OcarinaNote ocarinaSongNotes[][20], u16 musicStaffCurHeldLength[], u16 musicStaffExpectedLength[], u16 musicStaffPos[], u8 musicStaffExpectedPitch[], s32 isScarecrow)
{
    OcarinaNote* curNote;
    OcarinaNote* nextNote;
    u32 result = 0;

    musicStaffCurHeldLength[songIndex] = musicStaffExpectedLength[songIndex] + 18;

    if (noNewValidInput)
    {
        if ((musicStaffCurHeldLength[songIndex] >= musicStaffExpectedLength[songIndex] - 18) &&
            (musicStaffCurHeldLength[songIndex] >= musicStaffExpectedLength[songIndex] + 18) &&
            (ocarinaSongNotes[songIndex][musicStaffPos[songIndex]].length == 0) &&
            (sMusicStaffPrevPitch == musicStaffExpectedPitch[songIndex]))
        {
            result = 1;
        }
    }
    else if (musicStaffCurHeldLength[songIndex] >= (musicStaffExpectedLength[songIndex] - 18))
    {
        // This else-if statement always holds true, taken if a new note is played
        if (sMusicStaffPrevPitch != OCARINA_PITCH_NONE)
        {
            // New note is played
            if (sMusicStaffPrevPitch == musicStaffExpectedPitch[songIndex])
            {
                // Note is part of expected song
                if (isScarecrow)
                {
                    musicStaffCurHeldLength[songIndex] = 0;
                }
            }
            else
            {
                // Note is not part of expected song, so this song is no longer available as an option in this
                // playback
                result = -1;
            }
        }

        curNote = &ocarinaSongNotes[songIndex][musicStaffPos[songIndex]];
        nextNote = &ocarinaSongNotes[songIndex][++musicStaffPos[songIndex]];
        musicStaffExpectedLength[songIndex] = curNote->length;
        musicStaffExpectedPitch[songIndex] = curNote->pitch;

        // The current note is not the expected note.
        if (sCurOcarinaPitch != musicStaffExpectedPitch[songIndex])
        {
            result = -1;
        }

        while (curNote->pitch == nextNote->pitch ||
                (nextNote->pitch == OCARINA_BTN_INVALID && nextNote->length != 0))
        {
            musicStaffExpectedLength[songIndex] += nextNote->length;
            curNote = &ocarinaSongNotes[songIndex][musicStaffPos[songIndex]];
            nextNote = &ocarinaSongNotes[songIndex][musicStaffPos[songIndex] + 1];
            musicStaffPos[songIndex]++;
        }
    }
    return result;
}

void Ocarina_CheckSongsWithMusicStaff()
{
    u16 curOcarinaSongFlag;
    u8 noNewValidInput = false;
    s8 staffOcarinaPlayingPosOffset = 0;
    u8 songIndex;

    AudioOcarina_CheckIfStartedSong();

    if (!sOcarinaHasStartedSong)
    {
        return;
    }

    if (ABS_ALT(sCurOcarinaBendIndex) > 20)
    {
        sOcarinaFlags = 0;
        return;
    }

    if (sPrevOcarinaPitch == sCurOcarinaPitch || sCurOcarinaPitch == OCARINA_PITCH_NONE)
    {
        noNewValidInput = true;
    }

    for (songIndex = 1; songIndex < CUSTOM_SONG_MAX; songIndex++)
    {
        curOcarinaSongFlag = 1 << songIndex;

        if (sAvailOcarinaSongFlagsCustom & curOcarinaSongFlag)
        {
            s32 result = checkSong(songIndex - 1,
                noNewValidInput,
                sOcarinaCustomSongNotes,
                sMusicStaffCurHeldLengthCustom,
                sMusicStaffExpectedLengthCustom,
                sMusicStaffPosCustom,
                sMusicStaffExpectedPitchCustom,
                0);
            if (result > 0)
            {
                gPlayedOcarinaSong = songIndex + 0x80 + 1;
                sIsOcarinaInputEnabled = false;
                sOcarinaFlags = 0;
            }
            else if (result < 0)
            {
                sAvailOcarinaSongFlagsCustom ^= curOcarinaSongFlag;
            }
        }
    }

    for (songIndex = sFirstOcarinaSongIndex; songIndex < sLastOcarinaSongIndex; songIndex++)
    {
        curOcarinaSongFlag = 1 << songIndex;

        if (sAvailOcarinaSongFlags & curOcarinaSongFlag)
        {
            s32 result = checkSong(songIndex,
                noNewValidInput,
                sOcarinaSongNotes,
                sMusicStaffCurHeldLength,
                sMusicStaffExpectedLength,
                sMusicStaffPos,
                sMusicStaffExpectedPitch,
                songIndex == OCARINA_SONG_SCARECROW_SPAWN);
            if (result > 0)
            {
                gPlayedOcarinaSong = songIndex + 1;
                sIsOcarinaInputEnabled = false;
                sOcarinaFlags = 0;
            }
            else if (result < 0)
            {
                sAvailOcarinaSongFlags ^= curOcarinaSongFlag;
            }
        }

        // if a note is played that doesn't match a song, the song bit in sAvailOcarinaSongFlags is turned off
        // if there are no more songs remaining that it could be and the maximum position has been exceeded, then
        if (sAvailOcarinaSongFlags == 0 && sStaffOcarinaPlayingPos >= sMusicStaffNumNotesPerTest)
        {
            sIsOcarinaInputEnabled = false;
            sOcarinaFlags = 0;
            return;
        }
    }

    if (!noNewValidInput)
    {
        sMusicStaffPrevPitch = sCurOcarinaPitch;
        sStaffOcarinaPlayingPos += staffOcarinaPlayingPosOffset + 1;
    }
}

PATCH_FUNC(0x800c2318, Ocarina_CheckSongsWithMusicStaff)

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

    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    if (toChild)
        comboTextAppendStr(&b, "Become a " TEXT_COLOR_RED "child");
    else
        comboTextAppendStr(&b, "Become an " TEXT_COLOR_RED "adult");
    comboTextAppendStr(&b, TEXT_CZ "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "OK" TEXT_NL "No" TEXT_END);
}

static void soaringNoStatuesMessage(PlayState* play)
{
    char* b;

    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "You have yet to leave your " TEXT_COLOR_RED "mark" TEXT_CZ TEXT_NL);
    comboTextAppendStr(&b, "on any of the " TEXT_COLOR_RED "statues" TEXT_CZ " you have" TEXT_NL);
    comboTextAppendStr(&b, "come across. Hurry up and find" TEXT_NL);
    comboTextAppendStr(&b, "some." TEXT_END);
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

static void songOfDoubleTimeMessage(PlayState* play)
{
    char* b;

    b = play->msgCtx.font.msgBuf;
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

    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Your notes echoed far.... " TEXT_NL "but nothing happened." TEXT_FADE("\x28") TEXT_END);
}

static void SetupSongOfDoubleTime(PlayState* play)
{
    if (gSaveContext.sunSongState != 0 || (play->envCtx.sceneTimeSpeed == 0 && (play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_1 || play->interfaceCtx.restrictions.sunSong == 3)))
    {
        PlayerDisplayTextBox(play, 0x88c, NULL);
        songOfDoubleTimeFailMessage(play);
        play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
        sInCustomSong = CUSTOM_SONG_NONE;
    }
    else
    {
        PlayerDisplayTextBox(play, 0xe0, NULL); /* You want to talk to Saria, right? */
        songOfDoubleTimeMessage(play);
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

void Ocarina_ClearLastPlayedSong(PlayState* play)
{
    sCustomSongPlayed = CUSTOM_SONG_NONE;
}

void Ocarina_HandleLastPlayedSong(PlayState* play, Player* player, s16 lastPlayedSong)
{
    if (sCustomSongPlayed > CUSTOM_SONG_NONE && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        sInCustomSong = sCustomSongPlayed;
        sCustomSongPlayed = CUSTOM_SONG_NONE;
        if (sInCustomSong == CUSTOM_SONG_SOARING && play->msgCtx.ocarinaAction == OCARINA_ACTION_CHECK_CUSTOM_SONG_SOARING)
        {
            sInCustomSong = CUSTOM_SONG_NONE;
        }
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
        case CUSTOM_SONG_HEALING:
        case CUSTOM_SONG_AWAKENING:
        case CUSTOM_SONG_GORON:
        case CUSTOM_SONG_GORON_HALF:
        case CUSTOM_SONG_ZORA:
        case CUSTOM_SONG_ORDER:
            play->msgCtx.ocarinaMode = 4; /* OCARINA_MODE_END */
            Message_Close(play);
            break;
        }
        return;
    }

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

extern Vec3f gPlayerLastSafePos;
extern s16 gPlayerLastSafeRot;
extern s8 gPlayerLastSafeRoom;
extern s32 gPlayerLastSafeExists;

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

                Play_SetRespawnData(play, RESPAWN_MODE_DOWN, gSaveContext.save.entrance, gPlayerLastSafeRoom, 0xdff, &gPlayerLastSafePos, gPlayerLastSafeRot);

                gSaveContext.respawnFlag = -4;
                play->nextEntranceIndex = gSaveContext.save.entrance;
                play->transitionTrigger = TRANS_TRIGGER_START;
                gSaveContext.sunSongState = 0; /* SUNSSONG_INACTIVE */
                gSaveContext.seqId = (u8)NA_BGM_DISABLED;
                gSaveContext.natureAmbienceId = NATURE_ID_DISABLED;
            }
        }
    }
}

u8 Ocarina_BeforeSongPlayingProcessed(PlayState* play)
{
    u8 songPlayed = play->msgCtx.ocarinaStaff->state;

    if (songPlayed > 0x80 && songPlayed < 0x80 + CUSTOM_SONG_MAX)
    {
        sCustomSongPlayed = songPlayed - 0x80;

        if (sCustomSongPlayed == CUSTOM_SONG_ELEGY && play->msgCtx.ocarinaAction != OCARINA_ACTION_CHECK_CUSTOM_SONG_ELEGY)
        {
            PlaySound(NA_SE_SY_TRE_BOX_APPEAR);
            HandleElegy(play);
        }
        else
        {
            if (play->msgCtx.ocarinaAction == OCARINA_ACTION_CHECK_SCARECROW_SPAWN)
            {
                AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_OFF);
                PlaySound(NA_SE_SY_OCARINA_ERROR);
                play->msgCtx.stateTimer = 10;
                play->msgCtx.msgMode = MSGMODE_OCARINA_FAIL;
            }
            else if (play->msgCtx.ocarinaAction == OCARINA_ACTION_FREE_PLAY)
            {
                Message_ContinueTextbox(play, 0x86f);
                play->msgCtx.msgMode = MSGMODE_SONG_PLAYED;
                play->msgCtx.textBoxType = 3; /* TEXTBOX_TYPE_OCARINA */
                play->msgCtx.stateTimer = 10;
                PlaySound(NA_SE_SY_TRE_BOX_APPEAR);
                Interface_ChangeHudVisibilityMode(1); /* HUD_VISIBILITY_NOTHING */
            }
            else
            {
                play->msgCtx.msgMode = MSGMODE_OCARINA_CORRECT_PLAYBACK;
                play->msgCtx.stateTimer = 20;
                PlaySound(NA_SE_SY_TRE_BOX_APPEAR);
                Interface_ChangeHudVisibilityMode(1); /* HUD_VISIBILITY_NOTHING */
            }
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

static s16 sCustomSongActorEffectIds[CUSTOM_SONG_MAX - 1] = {
    ACTOR_CUSTOM_OCEFF_WIPE6,
    ACTOR_OCEFF_WIPE,
    ACTOR_CUSTOM_OCEFF_WIPE5,
    ACTOR_OCEFF_WIPE,
    ACTOR_CUSTOM_OCEFF_WIPE7,
    ACTOR_CUSTOM_OCEFF_WIPE5,
    ACTOR_CUSTOM_OCEFF_WIPE5,
    ACTOR_CUSTOM_OCEFF_WIPE5,
    ACTOR_CUSTOM_OCEFF_WIPE5,
    ACTOR_CUSTOM_OCEFF_WIPE5,
};

static u16 sCustomSongActorEffectParams[CUSTOM_SONG_MAX - 1] = {
    0,
    1,
    3,
    1,
    0,
    0,
    1,
    1,
    2,
    4
};

void Ocarina_ResetOcarinaNoteState()
{
    PlayState* play = gPlay;
    Message_ResetOcarinaNoteState();

    if (sCustomSongPlayed > CUSTOM_SONG_NONE && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        Player* player = GET_PLAYER(play);
        s16 actorId = sCustomSongActorEffectIds[sCustomSongPlayed - 1];
        if (actorId >= 0)
        {
            u16 actorParams = sCustomSongActorEffectParams[sCustomSongPlayed - 1];
            Actor_Spawn(&play->actorCtx, play, actorId, player->actor.world.pos.x, player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, actorParams);
        }
        play->msgCtx.unk_e3f2 = play->msgCtx.lastPlayedSong = OCARINA_SONG_CUSTOM_START + sCustomSongPlayed;
    }
}

PATCH_CALL(0x800dee00, Ocarina_ResetOcarinaNoteState)

void Ocarina_SetPlaybackSong(s8 songIndexPlusOne, u8 playbackState)
{
    if (sCustomSongPlayed > CUSTOM_SONG_NONE && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        OcarinaNote** sPlaybackSong = (OcarinaNote**)0x80102b3c;
        AudioOcarina_SetPlaybackSong(1, playbackState);
        *sPlaybackSong = sOcarinaCustomSongNotes[sCustomSongPlayed - 1];
    }
    else
    {
        AudioOcarina_SetPlaybackSong(songIndexPlusOne, playbackState);
    }
}

PATCH_CALL(0x800deec8, Ocarina_SetPlaybackSong);

static u16 sOcarinaCustomSongFanfares[CUSTOM_SONG_MAX - 1] = {
    FANFARE_SONG_SOARING,
    FANFARE_SONG_TIME,
    FANFARE_SONG_EMPTINESS,
    FANFARE_SONG_DOUBLE_TIME,
    FANFARE_SONG_HEALING,
    FANFARE_SONG_AWAKENING,
    FANFARE_SONG_GORON,
    FANFARE_SONG_GORON_HALF,
    FANFARE_SONG_ZORA,
    FANFARE_SONG_ORDER,
};

void Ocarina_PlayFanfare(u16 seqId, u8 ioData)
{
    if (sCustomSongPlayed > CUSTOM_SONG_NONE && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        Audio_PlayFanfare(sOcarinaCustomSongFanfares[sCustomSongPlayed - 1]);
    }
    else
    {
        Audio_PlayFanfare(seqId);
    }
}

PATCH_CALL(0x800deee8, Ocarina_PlayFanfare)

const char* sCustomSongPlayedText[CUSTOM_SONG_MAX - 1] = {
    TEXT_COLOR_RED "Song of Soaring",
    TEXT_COLOR_RED "Song of Time",
    TEXT_COLOR_RED "Elegy of Emptiness",
    TEXT_COLOR_RED "Song of Double Time",
    TEXT_COLOR_RED "Song of Healing",
    TEXT_COLOR_RED "Sonata of Awakening",
    TEXT_COLOR_RED "Goron Lullaby",
    TEXT_COLOR_RED "Goron Lullaby Intro",
    TEXT_COLOR_RED "New Wave Bossa Nova",
    TEXT_COLOR_RED "Oath to Order",
};

void Ocarina_DisplaySongPlayedTextbox(PlayState* play, u16 textId)
{
    char* b;
    if (sCustomSongPlayed > CUSTOM_SONG_NONE && sCustomSongPlayed < CUSTOM_SONG_MAX)
    {
        DisplayTextBox2(play, 0x893);
        b = play->msgCtx.font.msgBuf;
        comboTextAppendHeader(&b);
        comboTextAppendStr(&b, "You played the ");
        comboTextAppendStr(&b, sCustomSongPlayedText[sCustomSongPlayed - 1]);
        comboTextAppendClearColor(&b);
        comboTextAppendStr(&b, "." TEXT_NL TEXT_NL TEXT_NL TEXT_END);
    }
    else
    {
        DisplayTextBox2(play, textId);
    }
}

PATCH_CALL(0x800df070, Ocarina_DisplaySongPlayedTextbox)
