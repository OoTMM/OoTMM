#ifndef SONG_NOTES_H
#define SONG_NOTES_H

#include <combo/types.h>

typedef union
{
    struct
    {
        u32 awakening_c_up_1:1;
        u32 awakening_c_left_1:1;
        u32 awakening_c_up_2:1;
        u32 awakening_c_left_2:1;
        u32 awakening_a_1:1;
        u32 awakening_c_right:1;
        u32 awakening_a_2:1;
    };
    u32 value;
} MmAwakeningNotes;

typedef union
{
    struct
    {
        u32 emptiness_c_right_1:1;
        u32 emptiness_c_left_1:1;
        u32 emptiness_c_right_2:1;
        u32 emptiness_c_down:1;
        u32 emptiness_c_right_3:1;
        u32 emptiness_c_up:1;
        u32 emptiness_c_left_2:1;
    };
    u32 value;
} MmEmptinessNotes;

typedef union
{
    struct
    {
        u32 epona_c_up_1:1;
        u32 epona_c_left_1:1;
        u32 epona_c_right_1:1;
        u32 epona_c_up_2:1;
        u32 epona_c_left_2:1;
        u32 epona_c_right_2:1;
    };
    u32 value;
} MmEponaNotes;

typedef union
{
    struct
    {
        u32 goron_half_a_1:2;
        u32 goron_half_c_right_1:2;
        u32 goron_half_c_left_1:2;
        u32 goron_half_a_2:2;
        u32 goron_half_c_right_2:2;
        u32 goron_half_c_left_2:2;
    };
    u32 value;
} MmGoronHalfNotes;

typedef union
{
    struct
    {
        u32 goron_a_1:1;
        u32 goron_c_right_1:1;
        u32 goron_c_left_1:1;
        u32 goron_a_2:1;
        u32 goron_c_right_2:1;
        u32 goron_c_left_2:1;
        u32 goron_c_right_3:1;
        u32 goron_a_3:1;
    };
    u32 value;
} MmGoronNotes;

typedef union
{
    struct
    {
        u32 healing_c_left_1:1;
        u32 healing_c_right_1:1;
        u32 healing_c_down_1:1;
        u32 healing_c_left_2:1;
        u32 healing_c_right_2:1;
        u32 healing_c_down_2:1;
    };
    u32 value;
} MmHealingNotes;

typedef union
{
    struct
    {
        u32 order_c_right_1:1;
        u32 order_c_down_1:1;
        u32 order_a:1;
        u32 order_c_down_2:1;
        u32 order_c_right_2:1;
        u32 order_c_up:1;
    };
    u32 value;
} MmOrderNotes;

typedef union
{
    struct
    {
        u32 soaring_c_down_1:1;
        u32 soaring_c_left_1:1;
        u32 soaring_c_up_1:1;
        u32 soaring_c_down_2:1;
        u32 soaring_c_left_2:1;
        u32 soaring_c_up_2:1;
    };
    u32 value;
} MmSoaringNotes;

typedef union
{
    struct
    {
        u32 storms_a_1:1;
        u32 storms_c_down_1:1;
        u32 storms_c_up_1:1;
        u32 storms_a_2:1;
        u32 storms_c_down_2:1;
        u32 storms_c_up_2:1;
    };
    u32 value;
} MmStormsNotes;

typedef union
{
    struct
    {
        u32 sun_c_right_1:1;
        u32 sun_c_down_1:1;
        u32 sun_c_up_1:1;
        u32 sun_c_right_2:1;
        u32 sun_c_down_2:1;
        u32 sun_c_up_2:1;
    };
    u32 value;
} MmSunNotes;

typedef union
{
    struct
    {
        u32 time_c_right_1:1;
        u32 time_a_1:1;
        u32 time_c_down_1:1;
        u32 time_c_right_2:1;
        u32 time_a_2:1;
        u32 time_c_down_3:1;
    };
    u32 value;
} MmTimeNotes;

typedef union
{
    struct
    {
        u32 zora_c_left_1:1;
        u32 zora_c_up:1;
        u32 zora_c_left_2:1;
        u32 zora_c_right_1:1;
        u32 zora_c_down:1;
        u32 zora_c_left_3:1;
        u32 zora_c_right_2:1;
    };
    u32 value;
} MmZoraNotes;

typedef struct
{
    MmAwakeningNotes   mm_awakening_notes;
    MmEmptinessNotes   mm_emptiness_notes;
    MmEponaNotes       mm_epona_notes;
    MmGoronHalfNotes   mm_goron_half_notes;
    MmGoronNotes       mm_goron_notes;
    MmHealingNotes     mm_healing_notes;
    MmOrderNotes       mm_order_notes;
    MmSoaringNotes     mm_soaring_notes;
    MmStormsNotes      mm_storms_notes;
    MmSunNotes         mm_sun_notes;
    MmTimeNotes        mm_time_notes;
    MmZoraNotes        mm_zora_notes;
}
MmSongNotes;

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
