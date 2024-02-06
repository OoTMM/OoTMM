#include <combo.h>
#include <combo/dma.h>

#define CACHE_COUNT    64
#define CACHE_TTL      6

static u32   sCacheVroms[CACHE_COUNT];
static void* sCacheAddrs[CACHE_COUNT];
static u8    sCacheTTLs[CACHE_COUNT];

void comboCacheClear(void)
{
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        free(sCacheAddrs[i]);
        sCacheAddrs[i] = NULL;
        sCacheVroms[i] = 0;
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

static void* loadCacheFile(u32 vrom)
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

    sz = comboDmaLoadFile(NULL, vrom);
    addr = malloc(sz);
    if (!addr)
        return NULL;
    comboDmaLoadFile(addr, vrom);
    sCacheVroms[slot] = vrom;
    sCacheAddrs[slot] = addr;
    sCacheTTLs[slot] = CACHE_TTL;

    return addr;
}

void* comboCacheGetFile(u32 vrom)
{
    void* addr;

    addr = NULL;
    for (int i = 0; i < CACHE_COUNT; ++i)
    {
        if (sCacheVroms[i] == vrom)
        {
            sCacheTTLs[i] = CACHE_TTL;
            addr = sCacheAddrs[i];
            break;
        }
    }
    if (!addr)
        addr = loadCacheFile(vrom);

    return addr;
}
