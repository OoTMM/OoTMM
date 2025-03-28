#ifndef OVL_OBJ_MURE2_H
#define OVL_OBJ_MURE2_H

#include <combo.h>
#include <combo/xflags.h>

typedef struct Actor_ObjMure2 Actor_ObjMure2;

typedef void (*Actor_ObjMure2Func)(Actor_ObjMure2*, PlayState*);

#define OBJ_MURE2_CHILD_MAX                     12
#define OBJ_MURE2_GET_CHILD_TYPE(thisx)         ((thisx)->params & 3)
#define OBJ_MURE2_GET_CHILD_UPPER_PARAMS(thisx) ((((thisx)->params) >> 8) & 0x1F)

struct Actor_ObjMure2
{
    Actor               actor;
    Actor_ObjMure2Func  actionFunc;
    Actor*              actors[OBJ_MURE2_CHILD_MAX];
    u16                 spawnFlags;
    float               rangeMultiplier;
    Xflag               xflag;
};

#endif
