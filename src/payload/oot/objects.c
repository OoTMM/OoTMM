#include <combo.h>

#define OBJECT_COUNT    8

static u16   sObjectsIds[OBJECT_COUNT];
static void* sObjectsAddr[OBJECT_COUNT];

u32 comboLoadObject(void* buffer, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;

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

    if (buffer)
        LoadFile(buffer, vromStart, vromEnd - vromStart);
    return vromEnd - vromStart;
}

void* comboGetObject(u16 objectId)
{
    u32 size;
    void* addr;

    if (!objectId)
        return NULL;

    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
        /* Already loaded the object */
        if (sObjectsIds[i] == objectId)
            return sObjectsAddr[i];

        /* Free slot */
        if (sObjectsIds[i] == 0)
        {
            size = comboLoadObject(NULL, objectId);
            addr = malloc(size);
            comboLoadObject(addr, objectId);
            sObjectsIds[i] = objectId;
            sObjectsAddr[i] = addr;
            return addr;
        }
    }
    return NULL;
}

void comboObjectsReset(void)
{
    for (int i = 0; i < OBJECT_COUNT; ++i)
    {
        sObjectsIds[i] = 0;
        sObjectsAddr[i] = NULL;
    }
}
