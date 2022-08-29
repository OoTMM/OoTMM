#include <combo.h>

#define OBJECT_COUNT    8

static u16   sObjectsIds[OBJECT_COUNT];
static void* sObjectsAddr[OBJECT_COUNT];

void* comboLoadObject(u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;
    void* buffer;

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

    buffer = malloc(vromEnd - vromStart);
    LoadFile(buffer, vromStart, vromEnd - vromStart);
    return buffer;
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
            addr = comboLoadObject(objectId);
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
