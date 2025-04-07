#include <ultra64.h>
#include <combo.h>

extern OSPiHandle* gCartHandle;

u32 File_Offset(int index)
{
    u32 data;

    osEPiReadIo(gCartHandle, kLoaderFileConfig.tableOffset + index * 16 + 4, &data);
    return data;
}

u32 File_Size(int index)
{
    u32 data;

    osEPiReadIo(gCartHandle, kLoaderFileConfig.tableOffset + index * 16 + 8, &data);
    return data;
}

u32 File_Flags(int index)
{
    u32 data;

    osEPiReadIo(gCartHandle, kLoaderFileConfig.tableOffset + index * 16 + 12, &data);
    return data;
}

int File_IndexFromID(u32 id)
{
    u32 tmp;
    int min;
    int max;
    int index;

    min = 0;
    max = kLoaderFileConfig.tableEntries - 1;

    for (;;)
    {
        if (min > max || min < 0 || max < 0 || min >= (int)kLoaderFileConfig.tableEntries || max >= (int)kLoaderFileConfig.tableEntries)
        {
            return -1;
        }

        index = (min + max) >> 1;
        osEPiReadIo(gCartHandle, kLoaderFileConfig.tableOffset + index * 16, &tmp);

        if (tmp == id)
        {
            return index;
        }

        if (tmp > id)
        {
            max = index - 1;
        }
        else
        {
            min = index + 1;
        }
    }
}

int File_DmaData(u32 dmaId, u32 vaddr, FileDmaData* out)
{
    u32 count;
    int dmaIndex;
    u32 dmaOffset;
    u32 vstart;
    u32 vsize;
    u32 id;
    u32 i;

    dmaIndex = File_IndexFromID(dmaId);
    dmaOffset = File_Offset(dmaIndex);
    osEPiReadIo(gCartHandle, dmaOffset, &count);
    dmaOffset += 4;

    for (i = 0; i < count; ++i)
    {
        /* Read the DMA entry */
        osEPiReadIo(gCartHandle, dmaOffset, &vstart);
        osEPiReadIo(gCartHandle, dmaOffset + 4, &vsize);
        osEPiReadIo(gCartHandle, dmaOffset + 8, &id);
        dmaOffset += 12;

        if (vaddr >= vstart && vaddr < vstart + vsize)
        {
            out->vstart = vstart;
            out->vsize = vsize;
            out->id = id;
            return 1;
        }
    }

    return 0;
}

#define FILE_CACHE_TTL      4
#define FILE_CACHE_ENTRIES  128

static u32 sFileCacheId[FILE_CACHE_ENTRIES];
static void* sFileCacheData[FILE_CACHE_ENTRIES];
static u8 sFileCacheTtl[FILE_CACHE_ENTRIES];

void File_CacheReset(void)
{
    for (int i = 0; i < FILE_CACHE_ENTRIES; ++i)
    {
        sFileCacheId[i] = 0;
        CustomHeap_Free(sFileCacheData[i]);
        sFileCacheData[i] = NULL;
        sFileCacheTtl[i] = 0;
    }
}

void File_CacheTick(void)
{
    for (int i = 0; i < FILE_CACHE_ENTRIES; ++i)
    {
        if (sFileCacheTtl[i] > 0)
        {
            sFileCacheTtl[i]--;
        }
        else if (sFileCacheData[i] != NULL)
        {
            CustomHeap_Free(sFileCacheData[i]);
            sFileCacheData[i] = NULL;
            sFileCacheId[i] = 0;
        }
    }
}

int DmaMgr_DmaRomToRam(u32 romAddr, void* ramAddr, u32 size);

void* File_CacheLoad(u32 id)
{
    int firstEmpty;
    void* data;
    int fileIndex;
    u32 fileSize;

    firstEmpty = -1;
    for (int i = 0; i < FILE_CACHE_ENTRIES; ++i)
    {
        if (sFileCacheData[i] == NULL && firstEmpty == -1)
            firstEmpty = i;
        else if (sFileCacheId[i] == id)
        {
            sFileCacheTtl[i] = FILE_CACHE_TTL;
            return sFileCacheData[i];
        }
    }

    /* No hit and no space left */
    if (firstEmpty == -1)
        return NULL;

    /* Load the file */
    fileIndex = File_IndexFromID(id);
    if (fileIndex < 0)
        return NULL;
    data = CustomHeap_Alloc(File_Size(fileIndex));
    if (data == NULL)
        return NULL;

    fileSize = File_Size(fileIndex);
    DmaMgr_DmaRomToRam(File_Offset(fileIndex), data, fileSize);

    sFileCacheId[firstEmpty] = id;
    sFileCacheData[firstEmpty] = data;
    sFileCacheTtl[firstEmpty] = FILE_CACHE_TTL;

    return data;
}
