#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>
#include <combo/io.h>

#if defined(GAME_OOT)
/* OoT doesn't have reentrancy in Yaz0_Decompress, implement it */
void Yaz0_Decompress(u32 pstart, void* dst, u32 size)
{
    static volatile u32 sDecompressInProgress;

    for (;;)
    {
        if (sDecompressInProgress == 0)
            break;
        Sleep_Usec(10);
    }
    sDecompressInProgress = 1;
    _Yaz0_Decompress(pstart, dst, size);
    sDecompressInProgress = 0;
}
#endif

static int dmaLookupNative(DmaEntry* buf, u32 vromAddr)
{
    DmaEntry* e;

    for (int i = 0; i < ARRAY_COUNT(gDmaData); ++i)
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

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);

#define ALT_DMA_CACHE_COUNT 8
static struct {
    DmaEntry entry;
    u32 table;
    u32 count;
    u32 flag;
} sAltDmaCache[ALT_DMA_CACHE_COUNT];
static u32 sAltDmaCacheNext;

static int dmaLookupAltCached(DmaEntry* out, u32 table, u32 count, u32 flag, u32 vrom)
{
    u32 mask = __osDisableInt();
    for (u32 i = 0; i < ALT_DMA_CACHE_COUNT; ++i)
    {
        if (sAltDmaCache[i].count == count && count && sAltDmaCache[i].table == table &&
            sAltDmaCache[i].flag == flag && vrom >= sAltDmaCache[i].entry.vstart &&
            vrom < sAltDmaCache[i].entry.vend)
        {
            *out = sAltDmaCache[i].entry;
            __osRestoreInt(mask);
            return 1;
        }
    }
    __osRestoreInt(mask);
    return 0;
}

static void dmaLookupAltRemember(const DmaEntry* entry, u32 table, u32 count, u32 flag)
{
    u32 mask = __osDisableInt();
    u32 slot = sAltDmaCacheNext++ % ALT_DMA_CACHE_COUNT;
    sAltDmaCache[slot].entry = *entry;
    sAltDmaCache[slot].table = table;
    sAltDmaCache[slot].count = count;
    sAltDmaCache[slot].flag = flag;
    __osRestoreInt(mask);
}

static int dmaLookupAlt(DmaEntry* buf, u32 dmaAddr, u32 dmaCount, u32 dmaFlag, u32 vromAddr)
{
    const DmaEntry* e;
    ALIGNED(16) DmaEntry sTmpDmaEntries[16];
    u32 rawVromAddr;
    u32 offset;

    if (dmaLookupAltCached(buf, dmaAddr, dmaCount, dmaFlag, vromAddr))
        return 1;
    if (!dmaCount)
        return 0;
    rawVromAddr = vromAddr & ~dmaFlag;
    offset = 0;
    for (;;)
    {
        DMARomToRam((dmaAddr + offset * sizeof(DmaEntry)) | PI_DOM1_ADDR2, sTmpDmaEntries, sizeof(sTmpDmaEntries));
        offset += ARRAY_COUNT(sTmpDmaEntries);
        for (int i = 0; i < ARRAY_COUNT(sTmpDmaEntries); ++i)
        {
            e = sTmpDmaEntries + i;

            if (rawVromAddr == e->vstart || (rawVromAddr > e->vstart && rawVromAddr < e->vend))
            {
                /* Found the entry - copy it to the cache */
                memcpy(buf, e, sizeof(DmaEntry));
                buf->vstart |= dmaFlag;
                buf->vend |= dmaFlag;
                dmaLookupAltRemember(buf, dmaAddr, dmaCount, dmaFlag);
                return 1;
            }
        }

        /* Didn't find the entry - try the next table */
        if (offset >= dmaCount)
            return 0;
    }
}

int comboDmaLookup(DmaEntry* buf, u32 vromAddr)
{
    u32 extraDmaAddr;
    u32 extraDmaCount;

    if (vromAddr >= 0x08000000)
    {
        /* Extra DMA */
        extraDmaAddr = IO_ReadPhysU32(COMBO_META_ROM + 0x00);
        extraDmaCount = IO_ReadPhysU32(COMBO_META_ROM + 0x04);
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

static void dmaReadRaw(u32 source, void* destination, u32 size)
{
    ALIGNED(16) u8 scratch[256];
    u8* out = destination;
    if (!size)
        return;
    if (!(((u32)destination & 7) | (source & 1) | (size & 1)))
    {
        DMARomToRam(source | PI_DOM1_ADDR2, destination, size);
        return;
    }
    while (size)
    {
        u32 prefix = source & 7;
        u32 amount = MIN(size, sizeof(scratch) - prefix);
        u32 transfer = (prefix + amount + 1) & ~1u;
        DMARomToRam((source - prefix) | PI_DOM1_ADDR2, scratch, transfer);
        memcpy(out, scratch + prefix, amount);
        source += amount;
        out += amount;
        size -= amount;
    }
}

typedef struct
{
    u32 start;
    u32 size;
} AnimationDmaBank;

typedef struct
{
    u32 map;
    u32 count;
    AnimationDmaBank banks[2];
} AnimationDmaConfig;

typedef struct
{
    u32 logical;
    u32 delta;
} AnimationDmaRange;

static AnimationDmaConfig sAnimationDmaConfig __attribute__((section(".data"))) = { 0 };
EXPORT_SYMBOL(ANIMATION_DMA_MAP, sAnimationDmaConfig);

static AnimationDmaRange* sAnimationDmaRanges;

extern void Fault_AddHungupAndCrashImpl(const char*, const char*);

void comboAnimationDmaInit(void)
{
    if (!sAnimationDmaConfig.map)
        return;
    u32 bytes = (sAnimationDmaConfig.count * 8 + 15) & ~15u;
    sAnimationDmaRanges = malloc(bytes);
    if (!sAnimationDmaRanges)
    {
        Fault_AddHungupAndCrashImpl("Animation DMA", "Out of memory");
        for (;;) {}
    }
    DMARomToRam(sAnimationDmaConfig.map | PI_DOM1_ADDR2, sAnimationDmaRanges, bytes);
}

static u32 animationDmaEnd(u32 vrom)
{
#if defined(GAME_MM)
    vrom ^= VROM_FOREIGN_OFFSET;
#endif
    u32 bank = vrom >> 26;
    if (bank >= 2 || !sAnimationDmaConfig.map)
        return 0;
    const AnimationDmaBank* info = &sAnimationDmaConfig.banks[bank];
    return vrom - info->start < info->size ? info->start + info->size : 0;
}

static int animationDmaRead(const DmaRequest* dma)
{
    u32 end = animationDmaEnd(dma->vromAddr);
    if (!end)
        return 0;
    if (!sAnimationDmaRanges)
        return 1;
    u32 logical = dma->vromAddr;
#if defined(GAME_MM)
    logical ^= VROM_FOREIGN_OFFSET;
#endif
    u32 size = MIN(dma->size, end - logical);
    u8* out = dma->dramAddr;
    while (size)
    {
        const AnimationDmaRange* lo = sAnimationDmaRanges;
        const AnimationDmaRange* hi = lo + sAnimationDmaConfig.count;
        while (lo + 1 < hi)
        {
            const AnimationDmaRange* mid = lo + (hi - lo) / 2;
            if (mid->logical <= logical)
                lo = mid;
            else
                hi = mid;
        }
        u32 amount = MIN(size, lo[1].logical - logical);
        dmaReadRaw(lo->delta + logical, out, amount);
        logical += amount;
        out += amount;
        size -= amount;
    }
    return 1;
}

void DmaManagerRunRequest(const DmaRequest* dma)
{
    DmaEntry e;
    u32 offset;
    int ret;

    if (animationDmaRead(dma))
        return;

    /* Find the file to DMA */
    ret = comboDmaLookup(&e, dma->vromAddr);
    if (!ret)
        return;
    if (e.pend == 0xffffffff)
        return;

    if (e.pend == 0)
    {
        /* File is not compressed - load and allow partial loading */
        offset = dma->vromAddr - e.vstart;
        dmaReadRaw(e.pstart + offset, dma->dramAddr, dma->size);
    }
    else
    {
        /* File is compressed */
        osSetThreadPri(NULL, 0x0a);
        Yaz0_Decompress(e.pstart, dma->dramAddr, e.pend - e.pstart);
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
    if (e->pend == 0xffffffff && !animationDmaEnd(e->vstart))
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
    int ret;

    ret = comboDmaLookup(&e, vrom);
    if (ret)
        return comboDmaLoadFilePartialImpl(dst, &e, vrom, 0, e.vend - e.vstart);
    else
        return comboDmaLoadFilePartialImpl(dst, NULL, vrom, 0, 0);
}

u32 comboDmaLoadFilePartial(void* dst, u32 vrom, u32 offset, s32 size)
{
    DmaEntry e;
    int ret;

    ret = comboDmaLookup(&e, vrom);
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
