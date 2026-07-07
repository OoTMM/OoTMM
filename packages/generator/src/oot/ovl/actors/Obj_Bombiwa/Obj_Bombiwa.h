#ifndef OVL_BOMBIWA_H
#define OVL_BOMBIWA_H

#include <combo.h>
#include <combo/xflags.h>

struct ObjBombiwa;

typedef struct ObjBombiwa {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderCylinder collider;
    Xflag xflag;
} ObjBombiwa; // size = 0x0198

#endif
