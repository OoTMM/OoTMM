#include <combo.h>

void KaleidoScope_AfterSetCutsorColor(GameState_Play* play)
{
    if (play->pauseCtx.cursorSlot[0] == ITS_MM_TRADE1)
        play->pauseCtx.cursorColorIndex = 4;
}
