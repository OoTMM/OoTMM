#include <combo.h>

void RequestDma(void*, void*, u32, u32, int, void*, int);

void hook_LoadObjectGi(void* player, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;

    if (objectId)
    {
        *((u8*)player + 0x1b8) = 1;

        if (objectId & 0x1000)
        {
            table = kExtraObjectsTable;
            objectId &= 0xfff;
        }
        else
        {
            table = kObjectsTable;
        }

        vromStart = table[objectId].vromStart;
        vromEnd = table[objectId].vromEnd;

        osCreateMesgQueue((OSMesgQueue*)((char*)player + 0x1dc), (OSMesg*)((char*)player + 0x1f4), 1);
        RequestDma(
            (char*)player + 0x1bc,
            *(void**)((char*)player + 0x1f8),
            vromStart,
            vromEnd - vromStart,
            0,
            (char*)player + 0x1dc,
            0
        );
    }
}
