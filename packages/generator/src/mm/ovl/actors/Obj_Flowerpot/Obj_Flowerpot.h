#ifndef OVL_OBJ_FLOWERPOT_H
#define OVL_OBJ_FLOWERPOT_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_ObjFlowerpot Actor_ObjFlowerpot;

typedef void (*Actor_ObjFlowerpotFunc)(Actor_ObjFlowerpot*, PlayState*);

struct Actor_ObjFlowerpot
{
    Actor                   actor;
    ColliderJntSph          collider;
    ColliderJntSphElement   colliderElements[2];
    Actor_ObjFlowerpotFunc  actionFunc;
    s16                     unk_1E8;
    u8                      unk_1EA;
    s8                      unk_1EB;
    s8                      unk_1EC;

    Xflag  xflags;
    u8     isExtendedFlags;
};

#endif
