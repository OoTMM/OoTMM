#include <combo.h>

typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

#if defined(GAME_OOT)
# define FOREIGN_OFF    0x80000
# define FOREIGN_CART   (0x10001000 | MM_BASE)
#else
# define FOREIGN_OFF    0x400
# define FOREIGN_CART   0x10001000
#endif
#define FOREIGN_DRAM   (0x80000000 | FOREIGN_OFF)

static void waitSubsystems(void)
{
    u32 tmp;

    for (;;)
    {
        tmp = IO_READ(SP_STATUS_REG);
        if (tmp & 1)
            break;
    }

    for (;;)
    {
        tmp = IO_READ(SP_DMA_BUSY_REG);
        if (!(tmp & 1))
            break;
    }

    for (;;)
    {
        tmp = IO_READ(DPC_STATUS_REG);
        if (!(tmp & 0x170))
            break;
    }

    for (;;)
    {
        tmp = IO_READ(AI_STATUS_REG);
        if (!(tmp & 0x40000000))
            break;
    }
}

void comboGameSwitch(void)
{
#if defined(GAME_OOT)
    gComboCtx.saveIndex = gSaveContext.fileIndex;
#endif

    comboWriteSave();
    comboDisableInterrupts();
    waitSubsystems();
    osInvalICache((void*)FOREIGN_DRAM, 0x100000);
    osInvalDCache((void*)FOREIGN_DRAM, 0x100000);
    comboDma_NoCacheInval((void*)FOREIGN_OFF, FOREIGN_CART, 0x100000);
    comboExportContext();
    ((EntryPoint)FOREIGN_DRAM)();
}
