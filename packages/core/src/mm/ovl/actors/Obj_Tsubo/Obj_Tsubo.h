#ifndef COMBO_OVL_OBJ_TSUBO_H
#define COMBO_OVL_OBJ_TSUBO_H

#include <combo/xflags.h>
#include <combo/actor.h>

struct Actor_ObjTsubo;

typedef void (*Actor_ObjTsubo_ActionFunc)(struct Actor_ObjTsubo*, GameState_Play*);

typedef struct Actor_ObjTsubo
{
    Actor                       actor;
    Actor_ObjTsubo_ActionFunc   actionFunc;
    ColliderCylinder            collider;

#if defined(GAME_MM)
    s8 unk_194;
    s8 unk_195;
    s8 homeRoom;
    s8 unk_197;
    s8 unk_198;
#endif

    s8                          requiredObjectSlot;

#if defined(GAME_MM)
    s8 unk_19a;
    u8 unk_19b;
#endif

    /* Extended flags */
    Xflag   xflag;
    u8      isExtended;
}
Actor_ObjTsubo;

#endif
