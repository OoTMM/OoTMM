#include <combo.h>
#include <combo/custom.h>

#define OOT_DMA_ADDR    0x7430
#define OOT_DMA_COUNT   0x5e6
#define MM_DMA_ADDR     (0x1a500 | MM_BASE)
#define MM_DMA_COUNT    0x610

#if defined(GAME_OOT)
# define DMA_ADDR           OOT_DMA_ADDR
# define DMA_COUNT          OOT_DMA_COUNT
# define DMA_ADDR_FOREIGN   MM_DMA_ADDR
# define DMA_COUNT_FOREIGN  MM_DMA_COUNT
#else
# define DMA_ADDR           MM_DMA_ADDR
# define DMA_COUNT          MM_DMA_COUNT
# define DMA_ADDR_FOREIGN   OOT_DMA_ADDR
# define DMA_COUNT_FOREIGN  OOT_DMA_COUNT
#endif

#define VROM_FOREIGN_OFFSET  0x04000000
#define VROM_CUSTOM_OFFSET   0x08000000

ALIGNED(16) DmaEntry kComboDmaData[DMA_COUNT + DMA_COUNT_FOREIGN + CUSTOM_DMA_SIZE + 1];

extern s16 gDmaDataCount;

void comboInitDma(void)
{
    DmaEntry* e;
    DMARomToRam(DMA_ADDR | PI_DOM1_ADDR2, kComboDmaData, DMA_COUNT * sizeof(DmaEntry));
    DMARomToRam(DMA_ADDR_FOREIGN | PI_DOM1_ADDR2, kComboDmaData + DMA_COUNT, DMA_COUNT_FOREIGN * sizeof(DmaEntry));
    DMARomToRam(CUSTOM_DMA_ADDR | PI_DOM1_ADDR2, kComboDmaData + DMA_COUNT + DMA_COUNT_FOREIGN, CUSTOM_DMA_SIZE * sizeof(DmaEntry));
    for (int i = DMA_COUNT; i < DMA_COUNT + DMA_COUNT_FOREIGN; ++i)
    {
        e = kComboDmaData + i;

        if (e->pend == 0xffffffff)
            continue;

        e->vstart |= VROM_FOREIGN_OFFSET;
        e->vend |= VROM_FOREIGN_OFFSET;
    }

#if defined(GAME_MM)
    gDmaDataCount = DMA_COUNT + DMA_COUNT_FOREIGN + CUSTOM_DMA_SIZE;
#endif
}

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

void comboDma_NoCacheInval(void* dramAddr, u32 cartAddr, u32 size)
{
    u32 tmp;

    waitForPi();
    while (size)
    {
        tmp = size;
        if (tmp > 0x2000)
            tmp = 0x2000;
        IO_WRITE(PI_DRAM_ADDR_REG, (u32)dramAddr & 0x1fffffff);
        IO_WRITE(PI_CART_ADDR_REG, cartAddr | PI_DOM1_ADDR2);
        IO_WRITE(PI_WR_LEN_REG, tmp - 1);
        waitForPi();
        size -= tmp;
        dramAddr = (void*)((u32)dramAddr + tmp);
        cartAddr += tmp;
    }
}

u32 comboLoadFile(void* dest, s32 fileIndex)
{
    u32 src = kComboDmaData[fileIndex].vstart;
    u32 size = kComboDmaData[fileIndex].vend - src;
    LoadFile(dest, src, size);
    return size;
}

u32 comboLoadForeignFile(void* dest, s32 foreignFileIndex)
{
#if defined(GAME_OOT)
    u32 fileIndexOffset = 0x5E6;
#else
    u32 fileIndexOffset = 0x610;
#endif
    return comboLoadFile(dest, fileIndexOffset + foreignFileIndex);
}
