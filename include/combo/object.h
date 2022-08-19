#ifndef COMBO_OBJECT_H
#define COMBO_OBJECT_H

#include <types.h>

typedef struct PACKED ALIGNED(4)
{
    u32 vromStart;
    u32 vromEnd;
}
ObjectData;

extern const ObjectData kObjectsTable[];
extern const ObjectData kExtraObjectsTable[];

#endif
