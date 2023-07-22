#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>

static int dmaLookupNative(DmaEntry* buf, u32 vromAddr)
{
    DmaEntry* e;

    for (int i = 0; i < ARRAY_SIZE(gDmaData); ++i)
    {
        e = gDmaData + i;

        if (vromAddr == e->vstart || (vromAddr > e->vstart && vromAddr < e->vend))
        {
            memcpy(buf, e, sizeof(DmaEntry));
            return 1;
        }
    }

    return 0;
}

static int dmaLookupAlt(DmaEntry* buf, u32 dmaAddr, u32 dmaCount, u32 dmaFlag, u32 vromAddr)
{
    const DmaEntry* e;
    ALIGNED(16) DmaEntry sTmpDmaEntries[16];
    u32 rawVromAddr;
    u32 offset;
    int index;

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
                memcpy(buf, e, sizeof(DmaEntry));
                buf->vstart |= dmaFlag;
                buf->vend |= dmaFlag;
                return 1;
            }
        }

        /* Didn't find the entry - try the next table */
        if (offset >= dmaCount)
            return 0;
    }
}

static int dmaLookup(DmaEntry* buf, u32 vromAddr)
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

        return dmaLookupAlt(buf, extraDmaAddr, extraDmaCount, 0, vromAddr);
    }
    else if (vromAddr & VROM_FOREIGN_OFFSET)
        return dmaLookupAlt(buf, DMA_ADDR_FOREIGN, DMA_COUNT_FOREIGN, VROM_FOREIGN_OFFSET, vromAddr);
    else
        return dmaLookupNative(buf, vromAddr);
}

void comboDmaLookupForeignId(DmaEntry* dst, int id)
{
    ALIGNED(16) DmaEntry tmp;

    DMARomToRam((DMA_ADDR_FOREIGN + id * sizeof(DmaEntry)) | PI_DOM1_ADDR2, &tmp, sizeof(tmp));
    memcpy(dst, &tmp, sizeof(tmp));
    dst->vstart |= VROM_FOREIGN_OFFSET;
    dst->vend |= VROM_FOREIGN_OFFSET;
}

void DmaManagerRunRequest(const DmaRequest* dma)
{
    DmaEntry e;
    u32 offset;
    int ret;

    /* Find the file to DMA */
    ret = dmaLookup(&e, dma->vromAddr);
    if (!ret)
        return;
    if (e.pend == 0xffffffff)
        return;

    if (e.pend == 0)
    {
        /* File is not compressed - load and allow partial loading */
        offset = dma->vromAddr - e.vstart;
        DMARomToRam((e.pstart + offset) | PI_DOM1_ADDR2, dma->dramAddr, dma->size);
    }
    else
    {
        /* File is compressed */
        osSetThreadPri(NULL, 0x0a);
        DmaCompressed(e.pstart, dma->dramAddr, e.pend - e.pstart);
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
    DmaEntry e;
    u32 size;
    int ret;

    ret = dmaLookup(&e, vrom);
    if (ret)
        return comboDmaLoadFilePartialImpl(dst, &e, vrom, 0, e.vend - e.vstart);
    else
        return comboDmaLoadFilePartialImpl(dst, NULL, vrom, 0, 0);
}

u32 comboDmaLoadFilePartial(void* dst, u32 vrom, u32 offset, s32 size)
{
    DmaEntry e;
    int ret;

    ret = dmaLookup(&e, vrom);
    if (ret)
        return comboDmaLoadFilePartialImpl(dst, &e, vrom, offset, size);
    else
        return comboDmaLoadFilePartialImpl(dst, NULL, vrom, offset, size);
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
    ALIGNED(16) DmaEntry e;

    DMARomToRam((DMA_ADDR_FOREIGN + foreignFileIndex * sizeof(DmaEntry)) | PI_DOM1_ADDR2, &e, sizeof(e));
    u32 src = e.vstart;
    u32 size = e.vend - src;
    LoadFile(dest, src | VROM_FOREIGN_OFFSET, size);
    return size;
}
