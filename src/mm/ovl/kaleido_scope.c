#include <combo.h>

void KaleidoScope_AfterSetCutsorColor(GameState_Play* play)
{
    int press;
    int effect;

    press = !!(play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS));
    effect = 0;

    if (play->pauseCtx.cursorSlot[0] == ITS_MM_TRADE1 && popcount(gMmExtraTrade.trade1) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade1();
            effect = 1;
        }
    }

    if (play->pauseCtx.cursorSlot[0] == ITS_MM_TRADE2 && popcount(gMmExtraTrade.trade2) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade2();
            effect = 1;
        }
    }

    if (play->pauseCtx.cursorSlot[0] == ITS_MM_TRADE3 && popcount(gMmExtraTrade.trade3) > 1)
    {
        play->pauseCtx.cursorColorIndex = 4;
        if (press)
        {
            comboToggleTrade3();
            effect = 1;
        }
    }

    if (effect)
    {
        PlaySound(0x4809);
    }
}
