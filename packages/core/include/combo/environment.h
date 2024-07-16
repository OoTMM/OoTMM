#ifndef COMBO_ENVIRONMENT_H
#define COMBO_ENVIRONMENT_H

#include <combo/types.h>

typedef struct EnvironmentContext EnvironmentContext;

#if defined(GAME_MM)
void Environment_NewDay(EnvironmentContext* ctxt);
void Environment_PlaySceneSequence(struct GameState_Play* play);
void Environment_PlayStormNatureAmbience(struct GameState_Play* play);
void Environment_StopStormNatureAmbience(struct GameState_Play* play);
u32 Environment_GetStormState(struct GameState_Play* play);

#define WEATHER_MODE_CLEAR 0
#define WEATHER_MODE_RAIN 1
#define WEATHER_MODE_2 2
#define WEATHER_MODE_SNOW 3

#define LIGHTNING_OFF 0
#define LIGHTNING_ON 1
#define LIGHTNING_LAST 2

#define STORM_STATE_OFF 0
#define STORM_STATE_ON 1

#define LIGHT_BLEND_OVERRIDE_NONE 0
#define LIGHT_BLEND_OVERRIDE_ON 1
#define LIGHT_BLEND_OVERRIDE_FULL_CONTROL 2

#endif

#endif
