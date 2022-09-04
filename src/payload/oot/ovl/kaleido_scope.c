#include <combo.h>

u32 popcount(u32 x)
{
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
}

static void toggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize)
{
    int bitPos;
    /* We need to get the bit index of the current item */

    bitPos = -1;
    for (u32 i = 0; i < tableSize; ++i)
    {
        if (*slot == table[i])
        {
            bitPos = i;
            break;
        }
    }
    if (bitPos == -1)
        return;
    for (;;)
    {
        bitPos++;
        if (bitPos >= tableSize)
            bitPos = 0;
        if (flags & (1 << bitPos))
        {
            *slot = table[bitPos];
            break;
        }
    }
}

static void toggleTradeAdult(void)
{
    toggleTrade(gSave.inventory + ITS_OOT_TRADE_ADULT, gOotExtraTrade.adult, kOotTradeAdult, 10);
}

static void toggleTradeChild(void)
{
    toggleTrade(gSave.inventory + ITS_OOT_TRADE_CHILD, gOotExtraTrade.child, kOotTradeChild, 11);
}

static void toggleOcarina(void)
{
    u8* slot;

    slot = gOotSave.inventory + ITS_OOT_OCARINA;
    if (*slot == ITEM_OOT_OCARINA_FAIRY)
    {
        *slot = ITEM_OOT_OCARINA_TIME;
    }
    else
    {
        *slot = ITEM_OOT_OCARINA_FAIRY;
    }
}

static void toggleHookshots(void)
{
    u8* slot;

    slot = gOotSave.inventory + ITS_OOT_HOOKSHOT;
    if (*slot == ITEM_OOT_HOOKSHOT)
    {
        *slot = ITEM_OOT_LONGSHOT;
    }
    else
    {
        *slot = ITEM_OOT_HOOKSHOT;
    }
}

static int checkItemToggle(GameState_Play* play)
{
    PauseContext* p;
    int ret;
    int press;

    p = &play->pauseCtx;
    ret = 0;
    press = !!(play->gs.input[0].pressed.buttons & 0x20);

    if (p->item_cursor == ITS_OOT_OCARINA && (popcount(gOotExtraItems.ocarina) >= 2))
    {
        ret = 1;
        if (press)
            toggleOcarina();
    }

    if (p->item_cursor == ITS_OOT_HOOKSHOT && (popcount(gOotExtraItems.hookshot) >= 2))
    {
        ret = 1;
        if (press)
            toggleHookshots();
    }

    if (p->item_cursor == ITS_OOT_TRADE_ADULT && (popcount(gOotExtraTrade.adult) >= 2))
    {
        ret = 1;
        if (press)
            toggleTradeAdult();
    }

    if (p->item_cursor == ITS_OOT_TRADE_CHILD && (popcount(gOotExtraTrade.child) >= 2))
    {
        ret = 1;
        if (press)
            toggleTradeChild();
    }

    if (ret && press)
    {
        PlaySound(0x4809);
    }

    return ret;
}

void KaleidoSetCursorColor(GameState_Play* play)
{
    PauseContext* p;
    u8 r;
    u8 g;
    u8 b;

    p = &play->pauseCtx;
    r = 0xff;
    g = 0xff;
    b = 0xff;

    /* Not on Z/R */
    if (p->cursor_pos == 0)
    {
        switch (p->screen_idx)
        {
        case 0:
            /* Item select */
            r = 0xff;
            g = 0xff;
            b = 0x00;
            if (checkItemToggle(play))
            {
                r = 0x00;
                g = 0xff;
                b = 0xff;
            }
            break;
        case 3:
            /* Equipment */
            r = 0x00;
            g = 0x00;
            b = 0xff;
            break;
        }
    }

    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
    CLOSE_DISPS();
}
