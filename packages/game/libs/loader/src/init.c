#include <loader.h>
#include <string.h>
#include <ultra64.h>

__attribute__ ((aligned(8))) char gLoaderStack[0x400];

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

    /* DMA */
    waitForPi();
    IO_WRITE(PI_DRAM_ADDR_REG, ((u32)cfg->ram) & 0x1fffffff);
    IO_WRITE(PI_CART_ADDR_REG, (cfg->rom) | PI_DOM1_ADDR2);
    IO_WRITE(PI_WR_LEN_REG, cfg->size - 1);
    waitForPi();

    cfg->entrypoint();
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
    LoadGame(1);

    for (;;) {}
}
