#ifndef COMBO_DMA_H
#define COMBO_DMA_H

#include <combo/types.h>

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

typedef struct
{
    u32 vstart;
    u32 vend;
    u32 pstart;
    u32 pend;
}
DmaEntry;

typedef struct
{
    u32     vromAddr;
    void*   dramAddr;
    u32     size;
    char*   name;
    s32     line;
    u32     unk[3];
}
DmaRequest;

extern DmaEntry gDmaData[DMA_COUNT];

void comboDmaLookupForeignId(DmaEntry* dst, int id);

#endif
