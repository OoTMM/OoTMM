#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>

static DmaEntry sDmaCache[16];
static int sDmaCacheIndex;

static const DmaEntry* dmaLookupNative(u32 vromAddr)
{
    DmaEntry* e;

    for (int i = 0; i < ARRAY_SIZE(gDmaData); ++i)
    {
        e = gDmaData + i;

        if (e->pend == 0xffffffff)
            continue;

        if (vromAddr >= e->vstart && vromAddr < e->vend)
            return e;
    }

    return NULL;
}

static const DmaEntry* dmaLookupCache(u32 vromAddr)
{
    DmaEntry* e;

    for (int i = 0; i < ARRAY_SIZE(sDmaCache); ++i)
    {
        e = sDmaCache + i;

        if (vromAddr >= e->vstart && vromAddr < e->vend)
            return e;
    }

    return NULL;
}

static const DmaEntry* dmaLookupForeign(u32 vromAddr)
{
    const DmaEntry* e;
    ALIGNED(16) DmaEntry tmp[16];
    u32 rawVromAddr;
    u32 offset;
    int index;

    /* Try to load the file from the cache */
    e = dmaLookupCache(vromAddr);
    if (e)
        return e;

    /* Cache miss - load from foreign DMA table */
    rawVromAddr = vromAddr & ~VROM_FOREIGN_OFFSET;
    offset = 0;
    for (;;)
    {
        DMARomToRam((DMA_ADDR_FOREIGN + offset * sizeof(DmaEntry)) | PI_DOM1_ADDR2, tmp, sizeof(tmp));
        offset += ARRAY_SIZE(tmp);
        for (int i = 0; i < ARRAY_SIZE(tmp); ++i)
        {
            e = tmp + i;

            if (rawVromAddr >= e->vstart && rawVromAddr < e->vend)
            {
                /* Found the entry - copy it to the cache */
                index = sDmaCacheIndex;
                sDmaCacheIndex = (sDmaCacheIndex + 1) % ARRAY_SIZE(sDmaCache);
                memcpy(sDmaCache + index, e, sizeof(DmaEntry));
                sDmaCache[index].vstart |= VROM_FOREIGN_OFFSET;
                sDmaCache[index].vend |= VROM_FOREIGN_OFFSET;
                return sDmaCache + index;
            }
        }

        /* Didn't find the entry - try the next table */
        if (offset >= DMA_COUNT_FOREIGN)
            return NULL;
    }
}

const DmaEntry* dmaLookupCustom(u32 vromAddr)
{
    const DmaEntry* e;
    ALIGNED(16) DmaEntry tmp[16];
    u32 offset;
    int index;

    /* Try to load the file from the cache */
    e = dmaLookupCache(vromAddr);
    if (e)
        return e;

    /* Cache miss - load from custom DMA table */
    offset = 0;
    for (;;)
    {
        DMARomToRam((CUSTOM_DMA_ADDR + offset * sizeof(DmaEntry)) | PI_DOM1_ADDR2, tmp, sizeof(tmp));
        offset += ARRAY_SIZE(tmp);
        for (int i = 0; i < ARRAY_SIZE(tmp); ++i)
        {
            e = tmp + i;

            if (vromAddr >= e->vstart && vromAddr < e->vend)
            {
                /* Found the entry - copy it to the cache */
                index = sDmaCacheIndex;
                sDmaCacheIndex = (sDmaCacheIndex + 1) % ARRAY_SIZE(sDmaCache);
                memcpy(sDmaCache + index, e, sizeof(DmaEntry));
                return sDmaCache + index;
            }
        }

        /* Didn't find the entry - try the next table */
        if (offset >= CUSTOM_DMA_SIZE)
            return NULL;
    }
}

static const DmaEntry* dmaLookup(u32 vromAddr)
{
    if (vromAddr & VROM_CUSTOM_OFFSET)
        return dmaLookupCustom(vromAddr);
    else if (vromAddr & VROM_FOREIGN_OFFSET)
        return dmaLookupForeign(vromAddr);
    else
        return dmaLookupNative(vromAddr);
}

void comboDmaLookupForeignId(DmaEntry* dst, int id)
{
    ALIGNED(16) DmaEntry tmp;

    DMARomToRam((DMA_ADDR_FOREIGN + id * sizeof(DmaEntry)) | PI_DOM1_ADDR2, &tmp, sizeof(tmp));
    tmp.vstart |= VROM_FOREIGN_OFFSET;
    tmp.vend |= VROM_FOREIGN_OFFSET;
    memcpy(dst, &tmp, sizeof(tmp));
}

void DmaManagerRunRequest(const DmaRequest* dma)
{
    const DmaEntry* e;
    u32 offset;

    /* Find the file to DMA */
    e = dmaLookup(dma->vromAddr);
    if (!e)
        return;

    if (e->pend == 0)
    {
        /* File is not compressed - load and allow partial loading */
        offset = dma->vromAddr - e->vstart;
        DMARomToRam((e->pstart + offset) | PI_DOM1_ADDR2, dma->dramAddr, dma->size);
    }
    else
    {
        /* File is compressed */
        osSetThreadPri(NULL, 0x0a);
        DmaCompressed(e->pstart, dma->dramAddr, e->pend - e->pstart);
#if defined(GAME_OOT)
        osSetThreadPri(NULL, 0x10);
#else
        osSetThreadPri(NULL, 0x11);
#endif
    }
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
    u32 src = gDmaData[fileIndex].vstart;
    u32 size = gDmaData[fileIndex].vend - src;
    LoadFile(dest, src, size);
    return size;
}

u32 comboLoadForeignFile(void* dest, s32 foreignFileIndex)
{
    ALIGNED(16) DmaEntry tmp;

    DMARomToRam((DMA_ADDR_FOREIGN + foreignFileIndex * sizeof(DmaEntry)) | PI_DOM1_ADDR2, &tmp, sizeof(tmp));
    u32 src = tmp.vstart;
    u32 size = tmp.vend - src;
    LoadFile(dest, src | VROM_FOREIGN_OFFSET, size);
    return size;
}
