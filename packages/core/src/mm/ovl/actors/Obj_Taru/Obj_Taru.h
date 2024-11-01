#ifndef OVL_OBJ_TARU_H
#define OVL_OBJ_TARU_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_ObjTaru Actor_ObjTaru;

typedef void (*Actor_ObjTaruFunc)(Actor_ObjTaru*, PlayState*);

struct Actor_ObjTaru
{
    DynaPolyActor       dyna;
    ColliderCylinder    collider;
    Actor_ObjTaruFunc   actionFunc;
    s8                  unk_1AC;
    Xflag               xflag;
    u8                  isExtended;
};

#endif

