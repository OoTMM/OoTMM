#ifndef SONG_NOTES_H
#define SONG_NOTES_H

#include <combo/types.h>

typedef union
{
    struct
    {
        u32 songTpForest:4;
        u32 songTpFire:4;
        u32 songTpWater:4;
        u32 songTpShadow:4;
        u32 songTpSpirit:4;
        u32 songTpLight:4;
        u32 songZelda:4;
        u32 songEpona:4;
        u32 songSaria:4;
        u32 songSun:4;
        u32 songTime:4;
        u32 songStorms:4;
        u32 songEmpty:4;
    };
    u64 value;
} OotSongNotes;

typedef union
{
    struct
    {
        u32 songAwakening:4;
        u32 songEmpty:4;
        u32 songGoron:4;
        u32 songGoronHalf:4;
        u32 songEpona:4;
        u32 songZora:4;
        u32 songHealing:4;
        u32 songOrder:4;
        u32 songTime:4;
        u32 songSoaring:4;
        u32 songStorms:4;
        u32 songSun:4;
    };
    u64 value;
} MmSongNotes;

typedef struct
{
    OotSongNotes ootSongNotes;
    MmSongNotes  MmSongNotes;
} SongNotes;


#define QUEST_OOT_SONG_TP_FOREST_NOTE 0
#define QUEST_OOT_SONG_TP_FIRE_NOTE 1
#define QUEST_OOT_SONG_TP_WATER_NOTE 2
#define QUEST_OOT_SONG_TP_SHADOW_NOTE 3
#define QUEST_OOT_SONG_TP_SPIRIT_NOTE 4
#define QUEST_OOT_SONG_TP_LIGHT_NOTE 5
#define QUEST_OOT_SONG_ZELDA_NOTE 6
#define QUEST_OOT_SONG_EPONA_NOTE 7
#define QUEST_OOT_SONG_SARIA_NOTE 8
#define QUEST_OOT_SONG_SUN_NOTE 9
#define QUEST_OOT_SONG_TIME_NOTE 10
#define QUEST_OOT_SONG_STORMS_NOTE 11
#define QUEST_OOT_SONG_EMPTY_NOTE 12

#define QUEST_MM_SONG_AWAKENING_NOTE 0
#define QUEST_MM_SONG_EMPTY_NOTE 1
#define QUEST_MM_SONG_GORON_NOTE 2
#define QUEST_MM_SONG_GORON_HALF_NOTE 3
#define QUEST_MM_SONG_EPONA_NOTE 4
#define QUEST_MM_SONG_ZORA_NOTE 5
#define QUEST_MM_SONG_HEALING_NOTE 6
#define QUEST_MM_SONG_ORDER_NOTE 7
#define QUEST_MM_SONG_TIME_NOTE 8
#define QUEST_MM_SONG_SOARING_NOTE 9
#define QUEST_MM_SONG_STORMS_NOTE 10
#define QUEST_MM_SONG_SUN_NOTE 11


#define TRIGGER_OOT_SONG_EPONA       0x13
#define TRIGGER_OOT_SONG_SARIA       0x14
#define TRIGGER_OOT_SONG_STORMS      0x15
#define TRIGGER_OOT_SONG_SUN         0x16
#define TRIGGER_OOT_SONG_TIME        0x17
#define TRIGGER_OOT_SONG_TP_FIRE     0x18
#define TRIGGER_OOT_SONG_TP_FOREST   0x19
#define TRIGGER_OOT_SONG_TP_LIGHT    0x1A
#define TRIGGER_OOT_SONG_TP_SHADOW   0x1B
#define TRIGGER_OOT_SONG_TP_SPIRIT   0x1C
#define TRIGGER_OOT_SONG_TP_WATER    0x1D
#define TRIGGER_OOT_SONG_ZELDA       0x1E
#define TRIGGER_OOT_SONG_EMPTINESS   0x1F
#define TRIGGER_MM_SONG_AWAKENING    0x22
#define TRIGGER_MM_SONG_EMPTINESS    0x23
#define TRIGGER_MM_SONG_EPONA        0x24
#define TRIGGER_MM_SONG_GORON_HALF   0x25
#define TRIGGER_MM_SONG_GORON        0x26
#define TRIGGER_MM_SONG_HEALING      0x27
#define TRIGGER_MM_SONG_ORDER        0x28
#define TRIGGER_MM_SONG_SOARING      0x29
#define TRIGGER_MM_SONG_STORMS       0x2A
#define TRIGGER_MM_SONG_SUN          0x2B
#define TRIGGER_MM_SONG_TIME         0x2C
#define TRIGGER_MM_SONG_ZORA         0x2D


#endif //SONG_NOTES_H
