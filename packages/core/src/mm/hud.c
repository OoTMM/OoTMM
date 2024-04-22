#include <combo.h>
#include <combo/dpad.h>

void Play_DrawHudButtons(GameState_Play* play);

void Play_DrawHudButtons_Wrapper(GameState_Play* play)
{
    /* Fix a vanilla bug with the ammo outline is some cases */
    OPEN_DISPS(play->gs.gfx);
    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0xff);
    CLOSE_DISPS();

    Play_DrawHudButtons(play);
    Dpad_Draw(play);
}

PATCH_CALL(0x801201f4, Play_DrawHudButtons_Wrapper);
