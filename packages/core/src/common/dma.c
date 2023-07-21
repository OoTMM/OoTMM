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

        if (vromAddr == e->vstart || (vromAddr > e->vstart && vromAddr < e->vend))
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

        if (vromAddr == e->vstart || (vromAddr > e->vstart && vromAddr < e->vend))
            return e;
    }

    return NULL;
}

ALIGNED(16) DmaEntry sTmpDmaEntries[16];

static const DmaEntry* dmaLookupAlt(u32 dmaAddr, u32 dmaCount, u32 dmaFlag, u32 vromAddr)
{
    const DmaEntry* e;
    u32 rawVromAddr;
    u32 offset;
    int index;

    /* Try to load the file from the cache */
    e = dmaLookupCache(vromAddr);
    if (e)
        return e;

    /* Cache miss - load from foreign DMA table */
    rawVromAddr = vromAddr & ~dmaFlag;
    offset = 0;
    for (;;)
    {
        DMARomToRam((dmaAddr + offset * sizeof(DmaEntry)) | PI_DOM1_ADDR2, sTmpDmaEntries, sizeof(sTmpDmaEntries));
        offset += ARRAY_SIZE(sTmpDmaEntries);
        for (int i = 0; i < ARRAY_SIZE(sTmpDmaEntries); ++i)
        {
            e = sTmpDmaEntries + i;

            if (rawVromAddr == e->vstart || (rawVromAddr > e->vstart && rawVromAddr < e->vend))
            {
                /* Found the entry - copy it to the cache */
                index = sDmaCacheIndex;
                sDmaCacheIndex = (sDmaCacheIndex + 1) % ARRAY_SIZE(sDmaCache);
                memcpy(sDmaCache + index, e, sizeof(DmaEntry));
                sDmaCache[index].vstart |= dmaFlag;
                sDmaCache[index].vend |= dmaFlag;
                return sDmaCache + index;
            }
        }

        /* Didn't find the entry - try the next table */
        if (offset >= dmaCount)
            return NULL;
    }
}

static const DmaEntry* dmaLookup(u32 vromAddr)
{
    u32 extraDmaAddr;
    u32 extraDmaCount;
    u32* meta;

    if (vromAddr >= 0x08000000)
    {
        /* Extra DMA */
        meta = (u32*)(0xb0000000 | COMBO_META_ROM);
        extraDmaAddr = meta[0];
        extraDmaCount = meta[1];

        return dmaLookupAlt(extraDmaAddr, extraDmaCount, 0, vromAddr);
    }
    else if (vromAddr & VROM_FOREIGN_OFFSET)
        return dmaLookupAlt(DMA_ADDR_FOREIGN, DMA_COUNT_FOREIGN, VROM_FOREIGN_OFFSET, vromAddr);
    else
        return dmaLookupNative(vromAddr);
}

void comboDmaLookupForeignId(DmaEntry* dst, int id)
{
    DMARomToRam((DMA_ADDR_FOREIGN + id * sizeof(DmaEntry)) | PI_DOM1_ADDR2, sTmpDmaEntries, sizeof(sTmpDmaEntries[0]));
    sTmpDmaEntries[0].vstart |= VROM_FOREIGN_OFFSET;
    sTmpDmaEntries[1].vend |= VROM_FOREIGN_OFFSET;
    memcpy(dst, sTmpDmaEntries, sizeof(sTmpDmaEntries[0]));
}

void DmaManagerRunRequest(const DmaRequest* dma)
{
    const DmaEntry* e;
    u32 offset;

    /* Find the file to DMA */
    e = dmaLookup(dma->vromAddr);
    if (!e)
        return;
    if (e->pend == 0xffffffff)
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

static u32 comboDmaLoadFilePartialImpl(void* dst, const DmaEntry* e, u32 vrom, u32 offset, s32 size)
{
    if (!e)
        for (;;) {}
    if (e->pend == 0xffffffff)
        return 0;
    if (size + offset > e->vend - e->vstart)
        size = e->vend - e->vstart - offset;
    if (size < 0)
        size = 0;
    if (dst)
        LoadFile(dst, e->vstart + offset, (u32)size);
    return size;
}

u32 comboDmaLoadFile(void* dst, u32 vrom)
{
    const DmaEntry* e;
    u32 size;

    e = dmaLookup(vrom);
    if (e)
        size = e->vend - e->vstart;
    else
        size = 0;
    return comboDmaLoadFilePartialImpl(dst, e, vrom, 0, size);
}

u32 comboDmaLoadFilePartial(void* dst, u32 vrom, u32 offset, s32 size)
{
    return comboDmaLoadFilePartialImpl(dst, dmaLookup(vrom), vrom, offset, size);
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
    DMARomToRam((DMA_ADDR_FOREIGN + foreignFileIndex * sizeof(DmaEntry)) | PI_DOM1_ADDR2, sTmpDmaEntries, sizeof(sTmpDmaEntries[0]));
    u32 src = sTmpDmaEntries[0].vstart;
    u32 size = sTmpDmaEntries[0].vend - src;
    LoadFile(dest, src | VROM_FOREIGN_OFFSET, size);
    return size;
}
