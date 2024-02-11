#include <combo.h>

#if defined(DEBUG)

#define DEBUGMENU_PAGE_NONE 0
#define DEBUGMENU_PAGE_MAIN 1

static u8 sDebugPage;

typedef void (*DebugMenuFunc)(void);

static void DebugHandler_None(void)
{
    OPEN_DISPS(gPlay->gs.gfx);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineMode(OVERLAY_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 0, 0, 0x80);
    gDPFillRectangle(OVERLAY_DISP++, 0, 0, 319, 239);
    CLOSE_DISPS();
}

static void DebugHandler_Main(void)
{

}

static const DebugMenuFunc kDebugMenuFuncs[] = {
    DebugHandler_None,
    DebugHandler_Main
};

void Debug_Init(void)
{
    sDebugPage = DEBUGMENU_PAGE_NONE;
}

void Debug_Update(void)
{
    kDebugMenuFuncs[sDebugPage]();
}
#endif
