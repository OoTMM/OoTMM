#include <combo.h>
#include <combo/custom.h>

static int checkItemToggle(GameState_Play* play)
{
    PauseContext* p;
    int ret;
    int press;

    p = &play->pauseCtx;
    ret = 0;
    press = !!(play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS));

    if (p->item_cursor == ITS_OOT_OCARINA && (popcount(gOotExtraItems.ocarina) >= 2))
    {
        ret = 1;
        if (press)
            comboToggleOcarina();
    }

    if (p->item_cursor == ITS_OOT_HOOKSHOT && (popcount(gOotExtraItems.hookshot) >= 2))
    {
        ret = 1;
        if (press)
            comboToggleHookshot();
    }

    if (p->item_cursor == ITS_OOT_TRADE_ADULT && (popcount(gOotExtraTrade.adult) >= 2))
    {
        ret = 1;
        if (press)
            comboToggleTradeAdult();
    }

    if (p->item_cursor == ITS_OOT_TRADE_CHILD && (popcount(gOotExtraTrade.child) >= 2))
    {
        ret = 1;
        if (press)
            comboToggleTradeChild();
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

static int isKeysMenu;

typedef void (*KaleidoScopeHandler)(GameState_Play*, void*);

static void KaleidoScope_HandleMapDungeonMenu(GameState_Play* play, void* unk, u32 overlayAddr)
{
    KaleidoScopeHandler handler;
    int onMenu;

    onMenu = play->pauseCtx.screen_idx == 1;
    if (onMenu && play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS))
    {
        isKeysMenu = !isKeysMenu;
        PlaySound(0x4809);
    }

    if (isKeysMenu)
    {
        if (onMenu)
            comboMenuKeysUpdate(play);
        comboMenuKeysDraw(play);
    }
    else
    {
        handler = OverlayAddr(overlayAddr);
        handler(play, unk);
    }
}

static void KaleidoScope_HandleMapMenu(GameState_Play* play, void* unk)
{
    KaleidoScope_HandleMapDungeonMenu(play, unk, 0x8081ce54);
}

PATCH_CALL(0x80820850, KaleidoScope_HandleMapMenu);
PATCH_CALL(0x80820b48, KaleidoScope_HandleMapMenu);

static void KaleidoScope_HandleDungeonMenu(GameState_Play* play, void* unk)
{
    KaleidoScope_HandleMapDungeonMenu(play, unk, 0x8081b660);
}

PATCH_CALL(0x808207e4, KaleidoScope_HandleDungeonMenu);
PATCH_CALL(0x80820ac4, KaleidoScope_HandleDungeonMenu);
