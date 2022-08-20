#include <combo.h>

void KaleidoSetCursorColor(GameState_Play* play)
{
    u8 r;
    u8 g;
    u8 b;

    r = 0xff;
    g = 0xff;
    b = 0xff;

    /* Hack: Check for L button presses */
    if (play->gs.input[0].pressed.buttons & 0x20)
    {
        gOotSave.inventory[1]++;
    }

    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
    CLOSE_DISPS();
}
