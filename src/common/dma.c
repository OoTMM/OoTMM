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
