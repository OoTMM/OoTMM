#ifndef COMBO_OVL_OBJ_TSUBO_H
#define COMBO_OVL_OBJ_TSUBO_H

#include <combo/xflags.h>
#include <combo/actor.h>

struct Actor_ObjTsubo;

typedef void (*Actor_ObjTsubo_ActionFunc)(struct Actor_ObjTsubo*, PlayState*);

typedef struct Actor_ObjTsubo
{
    Actor                       actor;
    Actor_ObjTsubo_ActionFunc   actionFunc;
    ColliderCylinder            collider;
    s8                          unk_194;
    s8                          unk_195;
    s8                          homeRoom;
    s8                          unk_197;
    s8                          unk_198;
    s8                          requiredObjectSlot;
    s8                          unk_19a;
    u8                          unk_19b;

    /* Extended flags */
    XflagID xflag;
}
Actor_ObjTsubo;

#endif
