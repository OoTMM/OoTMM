#include <combo.h>

void RequestDma(void*, void*, u32, u32, int, void*, int);

void hook_LoadObjectGi(void* player, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;

    if (objectId)
    {
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
