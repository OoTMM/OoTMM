#ifndef COMBO_OVL_EN_TEST4_H
#define COMBO_OVL_EN_TEST4_H

#include <combo/actor.h>

#define THREEDAY_GET_SKYBOX_NUM_STARS(this)             (((((s16)(this)->variable)) >> 10) * 100)
#define THREEDAY_GET_BIG_BELLS_SFX_VOLUME_INDEX(this)   (((this)->variable >> 5) & 0xF)

struct Actor_EnTest4;

typedef void (*Actor_EnTest4_ActionFunc)(struct Actor_EnTest4*, GameState_Play*);

#define THREEDAY_DAYTIME_NIGHT  0
#define THREEDAY_DAYTIME_DAY    1
#define THREEDAY_DAYTIME_MAX    2

#define THREEDAY_WEATHER_CLEAR  0
#define THREEDAY_WEATHER_RAIN   1

typedef struct Actor_EnTest4
{
    Actor actor;
    s8 daytimeIndex; // See `ThreeDayDaytime`
    u8 transitionCsTimer;
    u16 prevTime;
    u16 nextBellTime; // Next time the bell will sound
    u16 prevBellTime; // Last time the bell sounded
    u8 weather; // See `ThreeDayWeather`
    Actor_EnTest4_ActionFunc actionFunc;
}
Actor_EnTest4; // size = 0x154

_Static_assert(sizeof(Actor_EnTest4) == 0x154, "Size of MM Actor_EnTest4 is wrong");

#endif
