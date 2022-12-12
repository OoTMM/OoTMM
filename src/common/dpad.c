#include <combo.h>

#define DPAD_DOWN   0
#define DPAD_UP     1
#define DPAD_LEFT   2
#define DPAD_RIGHT  3

static void* sDpadIconBuffer;
static u8 sDpadItems[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };
static u8 sDpadItemsOld[] = { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE };

static const int kDpadPosX = 265;
static const int kDpadPosY = 70;

static const int kDpadOffX[] = { 0, 0, -1, 1 };
static const int kDpadOffY[] = { 1, -1, 0, 0 };

static float kDpadItemScale = 0.4f;

static void reloadIcons(GameState_Play* play)
{
    if (!sDpadIconBuffer)
    {
        sDpadIconBuffer = malloc(32 * 32 * 4 * 4);
    }

    for (int i = 0; i < 4; ++i)
    {
        if (sDpadItems[i] != sDpadItemsOld[i] && sDpadItems[i] != ITEM_NONE)
        {
            DMARomToRam((kComboDmaData[8].pstart + 0x1000 * sDpadItems[i]) | PI_DOM1_ADDR2, sDpadIconBuffer + (i * 32 * 32 * 4), 32 * 32 * 4);
            sDpadItemsOld[i] = sDpadItems[i];
        }
    }
}

void comboDpadDraw(GameState_Play* play)
{
#if defined(GAME_OOT)
    reloadIcons(play);

    /* Init */
    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(OVERLAY_DISP++);
    gSPSegment(OVERLAY_DISP++, 0x06, gCustomKeep);
    gSPSegment(OVERLAY_DISP++, 0x07, sDpadIconBuffer);
    CLOSE_DISPS();

    /* Draw */
    comboDrawInit2D(play);
    comboDrawBlit2D(play, 0x06000000, 32, 32, kDpadPosX, kDpadPosY, 0.5f);

    for (int i = 0; i < 4; ++i)
    {
        if (sDpadItems[i] != ITEM_NONE)
        {
            comboDrawBlit2D(play, 0x07000000 | (i * 32 * 32 * 4), 32, 32, kDpadPosX + kDpadOffX[i] * 16, kDpadPosY + kDpadOffY[i] * 16, kDpadItemScale);
        }
    }
#endif
}

/*
void comboDpadRefresh(GameState_Play* play)
{

}
*/


void comboDpadUpdate(GameState_Play* play)
{
    sDpadItems[DPAD_DOWN] = ITEM_OOT_OCARINA_TIME;
}
