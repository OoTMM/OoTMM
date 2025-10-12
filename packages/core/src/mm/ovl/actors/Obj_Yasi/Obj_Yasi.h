#ifndef OVL_OBJ_YASI_H
#define OVL_OBJ_YASI_H

#include <combo.h>
#include <combo/xflags.h>

struct ObjYasi;

typedef struct ObjYasi {
    /* 0x000 */ DynaPolyActor dyna;
    Xflag xflag;
} ObjYasi; // size = 0x15C

#define PALM_TREE_IS_WIDE(thisx) ((thisx)->params & 1)

#endif
