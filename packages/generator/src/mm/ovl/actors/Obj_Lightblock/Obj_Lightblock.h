#ifndef Z_OBJ_LIGHTBLOCK_H
#define Z_OBJ_LIGHTBLOCK_H

#include <combo.h>

#define LIGHTBLOCK_TYPE(thisx) ((thisx)->params & 1)
#define LIGHTBLOCK_GET_DESTROYED_SWITCH_FLAG(thisx) (((thisx)->params >> 8) & 0x7F)

struct Actor_ObjLightblock;

typedef void (*Actor_ObjLightblockActionFunc)(struct Actor_ObjLightblock*, PlayState*);

typedef struct Actor_ObjLightblock {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    Actor_ObjLightblockActionFunc actionFunc;
    s8 timer;
    u8 alpha;
    s8 collisionCounter;
} Actor_ObjLightblock;

#endif
