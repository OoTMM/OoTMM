#include <combo.h>
#include <string.h>
#include <ultra64.h>

char gGameSwitch;
char gGameId;
char gNextGameId;

void System_DisableInterrupts(void);

__attribute__ ((aligned(8))) char gLoaderStack[0x400];
__attribute__ ((aligned(16))) Save gSave;
__attribute__ ((aligned(16))) SaveOptions gSaveOptions;
s8 gSaveFileNum;

static void waitForPi(void)
{
    u32 status;

    for (;;)
    {
        status = IO_READ(PI_STATUS_REG);
        if ((status & 3) == 0)
            return;
    }
}

void LoadGame(int gameId)
{
    const GameConfig* cfg = gLoaderGameConfigs + (gameId - 1);

    System_DisableInterrupts();

    if (gGameId != gameId)
    {
        /* DMA */
        waitForPi();
        IO_WRITE(PI_DRAM_ADDR_REG, ((u32)cfg->ram) & 0x1fffffff);
        IO_WRITE(PI_CART_ADDR_REG, (cfg->rom) | PI_DOM1_ADDR2);
        IO_WRITE(PI_WR_LEN_REG, cfg->size - 1);
        waitForPi();
    }

    gGameSwitch = 0;
    gGameId = gameId;
    gNextGameId = GAME_NONE;

    cfg->entrypoint();
}

void Game_Switch(int gameId)
{
    __asm__ __volatile__ ("la $sp, gLoaderStack + 0x400\r\n");

    gNextGameId = (char)gameId;
    gGameSwitch = 1;

    LoadGame(gNextGameId);
}

extern char _loaderSegmentBssStart;
extern char _loaderSegmentBssEnd;

void ClearBSS(void)
{
    memset(&_loaderSegmentBssStart, 0, &_loaderSegmentBssEnd - &_loaderSegmentBssStart);
}

void LoaderInit(void)
{
    ClearBSS();

    gGameId = GAME_NONE;
    gNextGameId = GAME_NONE;
    gGameSwitch = 0;

    LoadGame(GAME_OOT);

    for (;;) {}
}
