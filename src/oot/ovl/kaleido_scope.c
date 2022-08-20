#include <combo.h>

void KaleidoSetCursorColor(GameState_Play* play)
{
    u8 r;
    u8 g;
    u8 b;

    r = 0xff;
    g = 0xff;
    b = 0xff;

    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
    CLOSE_DISPS();
}
