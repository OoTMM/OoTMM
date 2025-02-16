#ifndef COMMON_OCARINA_H
#define COMMON_OCARINA_H

#include <combo/types.h>

typedef enum
{
    OCARINA_BTN_A = 0,
    OCARINA_BTN_C_DOWN,
    OCARINA_BTN_C_RIGHT,
    OCARINA_BTN_C_LEFT,
    OCARINA_BTN_C_UP,
    OCARINA_BTN_C_RIGHT_OR_C_LEFT,
    OCARINA_BTN_INVALID = 0xFF
}
OcarinaButtonIndex;

typedef struct
{
    u8 numButtons;
    u8 buttonIndex[8];
}
OcarinaSongButtons;

typedef struct OcarinaStaff
{
    u8 buttonIndex;
    u8 state;
    u8 pos;
}
OcarinaStaff;

typedef enum OcarinaMode {
    /* 0x00 */ OCARINA_MODE_00,
    /* 0x01 */ OCARINA_MODE_01,
    /* 0x02 */ OCARINA_MODE_02,
    /* 0x03 */ OCARINA_MODE_03,
    /* 0x04 */ OCARINA_MODE_04,
    /* 0x05 */ OCARINA_MODE_05,
    /* 0x06 */ OCARINA_MODE_06,
    /* 0x07 */ OCARINA_MODE_07,
    /* 0x08 */ OCARINA_MODE_08,
    /* 0x09 */ OCARINA_MODE_09,
    /* 0x0A */ OCARINA_MODE_0A,
    /* 0x0B */ OCARINA_MODE_0B,
    /* 0x0C */ OCARINA_MODE_0C,
    /* 0x0D */ OCARINA_MODE_0D,
    /* 0x0E */ OCARINA_MODE_0E,
    /* 0x0F */ OCARINA_MODE_0F
} OcarinaMode;

#if defined(GAME_MM)
# define OCARINA_MODE_NONE                      0x00
# define OCARINA_MODE_ACTIVE                    0x01
# define OCARINA_MODE_WARP                      0x02
# define OCARINA_MODE_EVENT                     0x03
# define OCARINA_MODE_END                       0x04
# define OCARINA_MODE_PLAYED_TIME               0x05
# define OCARINA_MODE_PLAYED_HEALING            0x06
# define OCARINA_MODE_PLAYED_EPONAS             0x07
# define OCARINA_MODE_PLAYED_SOARING            0x08
# define OCARINA_MODE_PLAYED_STORMS             0x09
# define OCARINA_MODE_PLAYED_SUNS               0x0a
# define OCARINA_MODE_PLAYED_INVERTED_TIME      0x0b
# define OCARINA_MODE_PLAYED_DOUBLE_TIME        0x0c
# define OCARINA_MODE_PLAYED_SCARECROW_SPAWN    0x0d
# define OCARINA_MODE_E                         0x0e
# define OCARINA_MODE_F                         0x0f
# define OCARINA_MODE_10                        0x10
# define OCARINA_MODE_11                        0x11
# define OCARINA_MODE_PROCESS_SOT               0x12
# define OCARINA_MODE_PROCESS_INVERTED_TIME     0x13
# define OCARINA_MODE_14                        0x14
# define OCARINA_MODE_PROCESS_DOUBLE_TIME       0x15
# define OCARINA_MODE_APPLY_SOT                 0x16
# define OCARINA_MODE_17                        0x17
# define OCARINA_MODE_APPLY_INV_SOT_FAST        0x18
# define OCARINA_MODE_APPLY_INV_SOT_SLOW        0x19
# define OCARINA_MODE_APPLY_DOUBLE_SOT          0x1a
# define OCARINA_MODE_1B                        0x1b
# define OCARINA_MODE_WARP_TO_GREAT_BAY_COAST   0x1c
# define OCARINA_MODE_WARP_TO_ZORA_CAPE         0x1d
# define OCARINA_MODE_WARP_TO_SNOWHEAD          0x1e
# define OCARINA_MODE_WARP_TO_MOUNTAIN_VILLAGE  0x1f
# define OCARINA_MODE_WARP_TO_SOUTH_CLOCK_TOWN  0x20
# define OCARINA_MODE_WARP_TO_MILK_ROAD         0x21
# define OCARINA_MODE_WARP_TO_WOODFALL          0x22
# define OCARINA_MODE_WARP_TO_SOUTHERN_SWAMP    0x23
# define OCARINA_MODE_WARP_TO_IKANA_CANYON      0x24
# define OCARINA_MODE_WARP_TO_STONE_TOWER       0x25
# define OCARINA_MODE_WARP_TO_ENTRANCE          0x26
# define OCARINA_MODE_PROCESS_RESTRICTED_SONG   0x27
# define OCARINA_MODE_28                        0x28
# define OCARINA_MODE_29                        0x29
# define OCARINA_MODE_PLAYED_FULL_EVAN_SONG     0x2a

#define OCARINA_SONG_SONATA                     0x00
#define OCARINA_SONG_GORON_LULLABY              0x01
#define OCARINA_SONG_NEW_WAVE                   0x02
#define OCARINA_SONG_ELEGY                      0x03
#define OCARINA_SONG_OATH                       0x04
#define OCARINA_SONG_SARIAS                     0x05
#define OCARINA_SONG_TIME                       0x06
#define OCARINA_SONG_HEALING                    0x07
#define OCARINA_SONG_EPONAS                     0x08
#define OCARINA_SONG_SOARING                    0x09
#define OCARINA_SONG_STORMS                     0x0a
#define OCARINA_SONG_SUNS                       0x0b
#define OCARINA_SONG_INVERTED_TIME              0x0c
#define OCARINA_SONG_DOUBLE_TIME                0x0d
#define OCARINA_SONG_GORON_LULLABY_INTRO        0x0e
#define OCARINA_SONG_WIND_FISH_HUMAN            0x0f
#define OCARINA_SONG_WIND_FISH_GORON            0x10
#define OCARINA_SONG_WIND_FISH_ZORA             0x11
#define OCARINA_SONG_WIND_FISH_DEKU             0x12
#define OCARINA_SONG_EVAN_PART1                 0x13
#define OCARINA_SONG_EVAN_PART2                 0x14
#define OCARINA_SONG_ZELDAS_LULLABY             0x15
#define OCARINA_SONG_SCARECROW_SPAWN            0x16
#define OCARINA_SONG_TERMINA_WALL               0x17
#define OCARINA_SONG_MAX                        0x18
#define OCARINA_SONG_SCARECROW_LONG             OCARINA_SONG_MAX

#else
typedef enum OcarinaSongActionId {
    /* 0x00 */ OCARINA_ACTION_UNK_0, // acts like free play but never set
    /* 0x01 */ OCARINA_ACTION_FREE_PLAY,
    /* 0x02 */ OCARINA_ACTION_TEACH_MINUET, // Song demonstrations by teachers
    /* 0x03 */ OCARINA_ACTION_TEACH_BOLERO,
    /* 0x04 */ OCARINA_ACTION_TEACH_SERENADE,
    /* 0x05 */ OCARINA_ACTION_TEACH_REQUIEM,
    /* 0x06 */ OCARINA_ACTION_TEACH_NOCTURNE,
    /* 0x07 */ OCARINA_ACTION_TEACH_PRELUDE,
    /* 0x08 */ OCARINA_ACTION_TEACH_SARIA,
    /* 0x09 */ OCARINA_ACTION_TEACH_EPONA,
    /* 0x0A */ OCARINA_ACTION_TEACH_LULLABY,
    /* 0x0B */ OCARINA_ACTION_TEACH_SUNS,
    /* 0x0C */ OCARINA_ACTION_TEACH_TIME,
    /* 0x0D */ OCARINA_ACTION_TEACH_STORMS,
    /* 0x0E */ OCARINA_ACTION_UNK_E,
    /* 0x0F */ OCARINA_ACTION_PLAYBACK_MINUET, // Playing back a particular song
    /* 0x10 */ OCARINA_ACTION_PLAYBACK_BOLERO,
    /* 0x11 */ OCARINA_ACTION_PLAYBACK_SERENADE,
    /* 0x12 */ OCARINA_ACTION_PLAYBACK_REQUIEM,
    /* 0x13 */ OCARINA_ACTION_PLAYBACK_NOCTURNE,
    /* 0x14 */ OCARINA_ACTION_PLAYBACK_PRELUDE,
    /* 0x15 */ OCARINA_ACTION_PLAYBACK_SARIA,
    /* 0x16 */ OCARINA_ACTION_PLAYBACK_EPONA,
    /* 0x17 */ OCARINA_ACTION_PLAYBACK_LULLABY,
    /* 0x18 */ OCARINA_ACTION_PLAYBACK_SUNS,
    /* 0x19 */ OCARINA_ACTION_PLAYBACK_TIME,
    /* 0x1A */ OCARINA_ACTION_PLAYBACK_STORMS,
    /* 0x1B */ OCARINA_ACTION_UNK_1B,
    /* 0x1C */ OCARINA_ACTION_CHECK_MINUET, // Playing songs for check spots
    /* 0x1D */ OCARINA_ACTION_CHECK_BOLERO,
    /* 0x1E */ OCARINA_ACTION_CHECK_SERENADE,
    /* 0x1F */ OCARINA_ACTION_CHECK_REQUIEM,
    /* 0x20 */ OCARINA_ACTION_CHECK_NOCTURNE,
    /* 0x21 */ OCARINA_ACTION_CHECK_PRELUDE,
    /* 0x22 */ OCARINA_ACTION_CHECK_SARIA,
    /* 0x23 */ OCARINA_ACTION_CHECK_EPONA,
    /* 0x24 */ OCARINA_ACTION_CHECK_LULLABY,
    /* 0x25 */ OCARINA_ACTION_CHECK_SUNS,
    /* 0x26 */ OCARINA_ACTION_CHECK_TIME,
    /* 0x27 */ OCARINA_ACTION_CHECK_STORMS,
    /* 0x28 */ OCARINA_ACTION_CHECK_SCARECROW_SPAWN, // Playing back the song as adult that was set as child
    /* 0x29 */ OCARINA_ACTION_FREE_PLAY_DONE,
    /* 0x2A */ OCARINA_ACTION_SCARECROW_LONG_RECORDING,
    /* 0x2B */ OCARINA_ACTION_SCARECROW_LONG_PLAYBACK,
    /* 0x2C */ OCARINA_ACTION_SCARECROW_SPAWN_RECORDING,
    /* 0x2D */ OCARINA_ACTION_SCARECROW_SPAWN_PLAYBACK,
    /* 0x2E */ OCARINA_ACTION_MEMORY_GAME,
    /* 0x2F */ OCARINA_ACTION_FROGS,
    /* 0x30 */ OCARINA_ACTION_CHECK_NOWARP, // Check for any of sarias - storms
    /* 0x31 */ OCARINA_ACTION_CHECK_NOWARP_DONE
} OcarinaSongActionId;

typedef enum OcarinaInstrumentId {
    /* 0 */ OCARINA_INSTRUMENT_OFF,
    /* 1 */ OCARINA_INSTRUMENT_DEFAULT,
    /* 2 */ OCARINA_INSTRUMENT_MALON,
    /* 3 */ OCARINA_INSTRUMENT_WHISTLE,
    /* 4 */ OCARINA_INSTRUMENT_HARP,
    /* 5 */ OCARINA_INSTRUMENT_GRIND_ORGAN,
    /* 6 */ OCARINA_INSTRUMENT_FLUTE,
    /* 7 */ OCARINA_INSTRUMENT_MAX,
    /* 7 */ OCARINA_INSTRUMENT_DEFAULT_COPY1 = OCARINA_INSTRUMENT_MAX, // Unused but present in Sequence 0 table
    /* 8 */ OCARINA_INSTRUMENT_DEFAULT_COPY2 = OCARINA_INSTRUMENT_MAX + 1 // Unused but present in Sequence 0 table
} OcarinaInstrumentId;


#endif

#endif
