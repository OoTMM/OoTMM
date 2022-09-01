#include <combo.h>
#include <combo/custom.h>

#define OBJECT_COUNT    8

static u16   sObjectsIds[OBJECT_COUNT];
static void* sObjectsAddr[OBJECT_COUNT];

const ObjectData kExtraObjectsTable[] = {
#define X(a, b) { Y(a), Y(b) }
#define Y(x)    ((x) | 0x04000000)

#if defined(GAME_OOT)
# include "data/mm/objects.inc"
#endif

#if defined(GAME_MM)
# include "data/oot/objects.inc"
#endif

#undef X
#undef Y
};

ObjectData kCustomObjectsTable[CUSTOM_OBJECTS_SIZE];

void comboPatchForeignObject(void* buffer, u16 objectId);

void comboInitObjects(void)
{
    comboDma(kCustomObjectsTable, CUSTOM_OBJECTS_ADDR, CUSTOM_OBJECTS_SIZE * sizeof(ObjectData));
}

u32 comboLoadObject(void* buffer, u16 objectId)
{
    u32 vromStart;
    u32 vromEnd;
    const ObjectData* table;
    int isForeignObject;

    isForeignObject = 0;
    if (objectId & 0x2000)
    {
        table = kCustomObjectsTable;
    }
    else if (objectId & 0x1000)
    {
        table = kExtraObjectsTable;
        isForeignObject = 1;
    }
    else
    {
        table = kObjectsTable;
    }

    objectId &= 0xfff;
    vromStart = table[objectId].vromStart;
    vromEnd = table[objectId].vromEnd;

    if (buffer)
    {
        LoadFile(buffer, vromStart, vromEnd - vromStart);
        if (isForeignObject)
            comboPatchForeignObject(buffer, objectId);
    }
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
