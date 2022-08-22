#include <combo.h>
#include <combo/custom.h>

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

void comboInitObjects(void)
{
    comboDma(kCustomObjectsTable, CUSTOM_OBJECTS_ADDR, CUSTOM_OBJECTS_SIZE * sizeof(ObjectData));
}
