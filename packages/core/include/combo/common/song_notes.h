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
        u32 goron_half_a_1:1;
        u32 goron_half_c_right_1:1;
        u32 goron_half_c_left_1:1;
        u32 goron_half_a_2:1;
        u32 goron_half_c_right_2:1;
        u32 goron_half_c_left_2:1;
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
} OotEponaNotes;

typedef union
{
    struct
    {
        u32 saria_c_down_1:1;
        u32 saria_c_right_1:1;
        u32 saria_c_left_1:1;
        u32 saria_c_down_2:1;
        u32 saria_c_right_2:1;
        u32 saria_c_left_2:1;
    };
    u32 value;
} OotSariaNotes;

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
} OotStormsNotes;

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
} OotSunNotes;

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
} OotTimeNotes;

typedef union
{
    struct
    {
        u32 tp_fire_c_down_1:1;
        u32 tp_fire_a_1:1;
        u32 tp_fire_c_down_2:1;
        u32 tp_fire_a_2:1;
        u32 tp_fire_c_right_1:1;
        u32 tp_fire_c_right_2:1;
    };
    u32 value;
} OotTpFireNotes;

typedef union
{
    struct
    {
        u32 tp_forest_a:1;
        u32 tp_forest_c_up:1;
        u32 tp_forest_c_left_1:1;
        u32 tp_forest_c_right_1:1;
        u32 tp_forest_c_left_2:1;
        u32 tp_forest_c_right_2:1;
    };
    u32 value;
} OotTpForestNotes;

typedef union
{
    struct
    {
        u32 tp_light_c_up_1:1;
        u32 tp_light_c_right_1:1;
        u32 tp_light_c_up_2:1;
        u32 tp_light_c_right_2:1;
        u32 tp_light_c_left:1;
        u32 tp_light_c_up_3:1;
    };
    u32 value;
} OotTpLightNotes;

typedef union
{
    struct
    {
        u32 tp_shadow_c_left_1:1;
        u32 tp_shadow_c_right_1:1;
        u32 tp_shadow_c_right_2:1;
        u32 tp_shadow_a:1;
        u32 tp_shadow_c_left_2:1;
        u32 tp_shadow_c_right_3:1;
        u32 tp_shadow_c_down:1;
    };
    u32 value;
} OotTpShadowNotes;

typedef union
{
    struct
    {
        u32 tp_spirit_a_1:1;
        u32 tp_spirit_c_down_1:1;
        u32 tp_spirit_a_2:1;
        u32 tp_spirit_c_right:1;
        u32 tp_spirit_c_down_2:1;
    };
    u32 value;
} OotTpSpiritNotes;

typedef union
{
    struct
    {
        u32 tp_water_a:1;
        u32 tp_water_c_down:1;
        u32 tp_water_c_right_1:1;
        u32 tp_water_c_right_2:1;
        u32 tp_water_c_left:1;
    };
    u32 value;
} OotTpWaterNotes;

typedef union
{
    struct
    {
        u32 zelda_c_left_1:1;
        u32 zelda_c_up_1:1;
        u32 zelda_c_right_1:1;
        u32 zelda_c_left_2:1;
        u32 zelda_c_up_2:1;
        u32 zelda_c_right_2:1;
    };
    u32 value;
} OotZeldaNotes;

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
} OotEmptinessNotes;

typedef struct
{
    OotEponaNotes      oot_epona_notes;
    OotSariaNotes      oot_saria_notes;
    OotStormsNotes     oot_storms_notes;
    OotSunNotes        oot_sun_notes;
    OotTimeNotes       oot_time_notes;
    OotTpFireNotes     oot_tp_fire_notes;
    OotTpForestNotes   oot_tp_forest_notes;
    OotTpLightNotes    oot_tp_light_notes;
    OotTpShadowNotes   oot_tp_shadow_notes;
    OotTpSpiritNotes   oot_tp_spirit_notes;
    OotTpWaterNotes    oot_tp_water_notes;
    OotZeldaNotes      oot_zelda_notes;
    OotEmptinessNotes  oot_emptiness_notes;
}
OotSongNotes;


typedef struct
{
  OotSongNotes oot_song_notes;
  MmSongNotes  mm_song_notes;
}
SongNotes;

#endif //SONG_NOTES_H
