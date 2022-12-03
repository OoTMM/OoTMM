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
#define FOREIGN_SIZE   0x100000

void setSR(u32);
void setConfig(u32);

static void fakeIPL1(void)
{
    u32 tmp;

    /* IPL1 */

    /* Set Status & Config reg */
    setSR(0x34000000);
    setConfig(0x0006E463);

    /* Halt RSP */
    for (;;)
    {
        tmp = IO_READ(SP_STATUS_REG);
        if (tmp & SP_STATUS_HALT)
            break;
    }
    IO_WRITE(SP_STATUS_REG, SP_SET_HALT | SP_CLR_INTR);

    /* Wait for DMA busy SP reg */
    for (;;)
    {
        tmp = IO_READ(SP_DMA_BUSY_REG);
        if (!(tmp & 1))
            break;
    }

    /* Reset PI */
    IO_WRITE(PI_STATUS_REG, PI_SET_RESET | PI_CLR_INTR);

    /* Clear video */
    IO_WRITE(VI_INTR_REG, 0x3ff);
    IO_WRITE(VI_H_VIDEO_REG, 0);
    IO_WRITE(VI_CURRENT_REG, 0);

    /* Stop audio */
    IO_WRITE(AI_DRAM_ADDR_REG, 0);
    IO_WRITE(AI_LEN_REG, 0);

    /* Wait for SP DMA (again?) */
    for (;;)
    {
        tmp = IO_READ(SP_STATUS_REG);
        if (!(tmp & SP_STATUS_DMA_BUSY))
            break;
    }
}

static void fakeIPL3(void)
{
    u32 tmp;

    /* Reset RDP PC */
    tmp = IO_READ(SP_PC_REG);
    if (tmp != 0)
        IO_WRITE(SP_PC_REG, 0);

    /* Reset the whole MIPS interface */
    IO_WRITE(SP_STATUS_REG, 0xaaaaae);
    IO_WRITE(MI_INTR_MASK_REG, 0x555);
    IO_WRITE(SI_STATUS_REG, 0);
    IO_WRITE(AI_STATUS_REG, 0);
    IO_WRITE(MI_BASE_REG, 0x800);
    IO_WRITE(PI_STATUS_REG, 2);

    /* Clear DMEM & IMEM */
    for (tmp = 0; tmp < 0x1000; tmp += 4)
    {
        IO_WRITE(SP_DMEM_START + tmp, 0);
        IO_WRITE(SP_IMEM_START + tmp, 0);
    }
}

NORETURN void comboGameSwitch2(void);
NORETURN void comboGameSwitch4(u32);

NORETURN void comboGameSwitch(void)
{
#if defined(GAME_OOT)
    gComboCtx.saveIndex = gSaveContext.fileIndex;
#endif

    comboWriteSave();
    fakeIPL1();
    comboGameSwitch2();
}

NORETURN void comboGameSwitch3(void)
{
    comboInvalDCache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    comboDma_NoCacheInval((void*)FOREIGN_OFF, FOREIGN_CART, FOREIGN_SIZE);
    comboInvalICache((void*)FOREIGN_DRAM, FOREIGN_SIZE);
    IO_WRITE(PI_STATUS_REG, PI_STATUS_RESET | PI_STATUS_CLR_INTR);

    comboExportContext();

    NORETURN void (*entry)(u32);
    entry = (void*)(((u32)(&comboGameSwitch4) & 0x0fffffff) | 0xa0000000);

    fakeIPL3();
    entry(FOREIGN_DRAM);
}
