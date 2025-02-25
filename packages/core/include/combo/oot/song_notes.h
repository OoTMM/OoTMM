#ifndef SONG_NOTES_H
#define SONG_NOTES_H

#include <combo/types.h>


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
        u32 tp_spirit_a_3:1;
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

#define TRIGGER_OOT_SONG_EPONA      0x13
#define TRIGGER_OOT_SONG_SARIA      0x14
#define TRIGGER_OOT_SONG_STORMS     0x15
#define TRIGGER_OOT_SONG_SUN        0x16
#define TRIGGER_OOT_SONG_TIME       0x17
#define TRIGGER_OOT_SONG_TP_FIRE    0x18
#define TRIGGER_OOT_SONG_TP_FOREST  0x19
#define TRIGGER_OOT_SONG_TP_LIGHT   0x1A
#define TRIGGER_OOT_SONG_TP_SHADOW  0x1B
#define TRIGGER_OOT_SONG_TP_SPIRIT  0x1C
#define TRIGGER_OOT_SONG_TP_WATER   0x1D
#define TRIGGER_OOT_SONG_ZELDA      0x1E
#define TRIGGER_OOT_SONG_EMPTINESS  0x1F



#endif //SONG_NOTES_H
