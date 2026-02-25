#include <combo.h>

static void Environment_InitWrapper(PlayState* play, EnvironmentContext* envCtx, int unused)
{
    /* Fixes for ER */
    switch (play->sceneId)
    {
    case SCE_MM_DEKU_PLAYGROUND:
    case SCE_MM_FAIRY_FOUNTAIN: /* Fix a vanilla bug with ikana fountain */
        gWeatherMode = 0;
        break;
    }

    Environment_Init(play, envCtx, unused);
}

PATCH_CALL(0x801692ac, Environment_InitWrapper);
