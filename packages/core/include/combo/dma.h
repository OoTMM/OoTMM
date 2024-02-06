#ifndef COMBO_DMA_H
#define COMBO_DMA_H

#include <combo/types.h>

#define OOT_DMA_ADDR    0x7430
#define OOT_DMA_COUNT   0x5e6
#define MM_DMA_ADDR     (0x1a500 + MM_BASE)
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

extern DmaEntry gDmaData[DMA_COUNT];

int  comboDmaLookup(DmaEntry* buf, u32 vromAddr);
void comboDmaLookupForeignId(DmaEntry* dst, int id);
u32  comboDmaLoadFile(void* dst, u32 vrom);
u32  comboDmaLoadFilePartial(void* dst, u32 vrom, u32 offset, s32 size);

#endif
