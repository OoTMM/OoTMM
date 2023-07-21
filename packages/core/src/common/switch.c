#include <combo.h>
#include <combo/net.h>

typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

#if defined(GAME_OOT)
# define FOREIGN_OFF    0x80000
# define FOREIGN_CART   (0x10001000 + MM_BASE)
# define FOREIGN_SIZE   0x19500
#else
# define FOREIGN_OFF    0x400
# define FOREIGN_CART   0x10001000
# define FOREIGN_SIZE   0x6430
#endif
#define FOREIGN_DRAM   (0x80000000 | FOREIGN_OFF)

static void waitSubsystems(void)
{
    u32 tmp;

    /* VI and vblank */
    while (IO_READ(VI_CURRENT_REG) != 512)
        ;
    while (IO_READ(VI_CURRENT_REG) == 512)
        ;
    while (IO_READ(VI_CURRENT_REG) != 512)
        ;
    tmp = IO_READ(VI_CONTROL_REG);
    IO_WRITE(VI_CONTROL_REG, tmp & ~0x3);
    tmp = IO_READ(VI_CURRENT_REG);
    IO_WRITE(VI_CURRENT_REG, tmp);


    for (;;)
    {
        tmp = IO_READ(SP_STATUS_REG);
        if ((tmp & 2) && (tmp & 1) && !(tmp & (SP_STATUS_DMA_BUSY | SP_STATUS_DMA_FULL)))
            break;
    }
    IO_WRITE(SP_STATUS_REG, SP_SET_HALT | SP_CLR_BROKE | SP_CLR_INTR | SP_CLR_SSTEP | SP_CLR_INTR_BREAK | SP_CLR_SIG0 | SP_CLR_SIG1 | SP_CLR_SIG2 | SP_CLR_SIG3 | SP_CLR_SIG4 | SP_CLR_SIG5 | SP_CLR_SIG6 | SP_CLR_SIG7);
    IO_WRITE(SP_SEMAPHORE_REG, 0);
    IO_WRITE(SP_PC_REG, 0);

    for (;;)
    {
        tmp = IO_READ(DPC_STATUS_REG);
        if (!(tmp & 0x170))
            break;
    }
    IO_WRITE(MI_BASE_REG, 0x800);

    for (;;)
    {
        tmp = IO_READ(AI_STATUS_REG);
        if (!(tmp & 0xc0000001))
            break;
    }
    IO_WRITE(AI_STATUS_REG, 0);
    IO_WRITE(AI_CONTROL_REG, 0);

    for (;;)
    {
        tmp = IO_READ(PI_STATUS_REG);
        if (!(tmp & 3))
            break;
    }
    IO_WRITE(PI_STATUS_REG, 3);

    for (;;)
    {
        tmp = IO_READ(SI_STATUS_REG);
        if (!(tmp & 3))
            break;
    }
    IO_WRITE(SI_STATUS_REG, 0);
}

NORETURN void comboGameSwitch2(void);
NORETURN void comboGameSwitch4(u32);

NORETURN void comboGameSwitch(GameState_Play* play, s32 entrance)
{
    if (entrance == -1)
        gComboCtx.entrance = -1;
    else
        gComboCtx.entrance = (entrance & 0x7fffffff);

    netClose();
    comboSave(play, SF_OWL);
    comboDisableInterrupts();
    waitSubsystems();
    comboGameSwitch2();
}

NORETURN void comboGameSwitch3(void)
{
    comboInvalDCache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboInvalICache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboDma_NoCacheInval((void*)FOREIGN_OFF, FOREIGN_CART, FOREIGN_SIZE);
    comboInvalDCache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboInvalICache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    IO_WRITE(PI_STATUS_REG, PI_STATUS_RESET | PI_STATUS_CLR_INTR);

    comboExportContext();
    comboGameSwitch4(FOREIGN_DRAM);
}
