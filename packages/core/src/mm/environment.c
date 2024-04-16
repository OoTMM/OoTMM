#include <combo.h>

static void Environment_InitWrapper(GameState_Play* play, EnvironmentContext* envCtx, int unused)
{
    /* Fix a crash with deku playground */
    if (play->sceneId == SCE_MM_DEKU_PLAYGROUND)
        gWeatherMode = 0;

    Environment_Init(play, envCtx, unused);
}

PATCH_CALL(0x801692ac, Environment_InitWrapper);
