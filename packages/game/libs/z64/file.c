#include <ultra64.h>
#include <loader.h>

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

    dmaIndex = File_IndexFromID(dmaId);
    dmaOffset = File_Offset(dmaIndex);
    osEPiReadIo(gCartHandle, dmaOffset, &count);
    dmaOffset += 4;

    for (u32 i = 0; i < count; ++i)
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
