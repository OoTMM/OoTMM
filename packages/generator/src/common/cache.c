#include <combo.h>
#include <combo/dma.h>

#define CACHE_COUNT    64
#define CACHE_TTL      6

static u32   sCacheVroms[CACHE_COUNT];
static s32   sCacheSizes[CACHE_COUNT];
static void* sCacheAddrs[CACHE_COUNT];
static u8    sCacheTTLs[CACHE_COUNT];

void comboCacheClear(void)
{
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        free(sCacheAddrs[i]);
        sCacheAddrs[i] = NULL;
        sCacheVroms[i] = 0;
        sCacheSizes[i] = 0;
        sCacheTTLs[i] = 0;
    }
}

void comboCacheGarbageCollect(void)
{
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        if (sCacheTTLs[i])
        {
            --sCacheTTLs[i];
            if (!sCacheTTLs[i])
            {
                free(sCacheAddrs[i]);
                sCacheAddrs[i] = NULL;
                sCacheVroms[i] = 0;
            }
        }
    }
}

static void* loadCacheFile(u32 vrom, u32 offset, s32 size)
{
    size_t sz;
    void* addr;
    int slot;

    slot = -1;
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        if (sCacheAddrs[i] == NULL)
        {
            slot = i;
            break;
        }
    }

    if (slot == -1)
        return NULL;

    if (size == -1)
        sz = comboDmaLoadFile(NULL, vrom);
    else
        sz = (size_t)size;
    addr = malloc(sz);
    if (!addr)
        return NULL;
    comboDmaLoadFilePartial(addr, vrom, offset, sz);
    sCacheVroms[slot] = vrom + offset;
    sCacheSizes[slot] = size;
    sCacheAddrs[slot] = addr;
    sCacheTTLs[slot] = CACHE_TTL;

    return addr;
}

void* comboCacheGetFilePartial(u32 vrom, u32 offset, s32 size)
{
    void* addr;

    addr = NULL;
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        if (sCacheVroms[i] == (vrom + offset) && sCacheSizes[i] == size)
        {
            sCacheTTLs[i] = CACHE_TTL;
            addr = sCacheAddrs[i];
            break;
        }
    }
    if (!addr)
        addr = loadCacheFile(vrom, offset, size);

    return addr;
}

void* comboCacheGetFile(u32 vrom)
{
    return comboCacheGetFilePartial(vrom, 0, -1);
}
