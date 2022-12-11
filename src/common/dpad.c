#include <combo.h>

void comboDpadDraw(GameState_Play* play)
{
#if defined(GAME_OOT)
    /* Init */
    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(OVERLAY_DISP++);
    gSPSegment(OVERLAY_DISP++, 0x06, gCustomKeep);
    CLOSE_DISPS();

    /* Draw */
    comboDrawInit2D(play);
    comboDrawBlit2D(play, 0x06000000, 32, 32, 265, 70, 0.5f);
#endif
}
