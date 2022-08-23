#include <combo.h>

void RequestDma(void*, void*, u32, u32, int, void*, int);

void hook_LoadObjectGi(void* player, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;

    if (objectId)
    {
        *((u8*)player + 0x160) = 1;

        if (objectId & 0x2000)
        {
            table = kCustomObjectsTable;
        }
        else if (objectId & 0x1000)
        {
            table = kExtraObjectsTable;
        }
        else
        {
            table = kObjectsTable;
        }

        objectId &= 0xfff;
        vromStart = table[objectId].vromStart;
        vromEnd = table[objectId].vromEnd;

        osCreateMesgQueue((OSMesgQueue*)((char*)player + 0x184), (OSMesg*)((char*)player + 0x19c), 1);
        RequestDma(
            (char*)player + 0x164,
            *(void**)((char*)player + 0x1a0),
            vromStart,
            vromEnd - vromStart,
            0,
            (char*)player + 0x184,
            0
        );
    }
}
