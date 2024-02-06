#ifndef COMBO_COMMON_ACTOR_OBJ_MURE3_H
#define COMBO_COMMON_ACTOR_OBJ_MURE3_H

#include <combo/xflags.h>
#include <combo/common/actor.h>
#include <combo/common/actors/En_Item00.h>

typedef struct ALIGNED(4)
{
    Actor               base;
    void*               handler;
    Actor_EnItem00*     children[7];
    char                unk_15c[4];

    /* Extended */
    u8              actorIndex;
}
Actor_ObjMure3;

_Static_assert(sizeof(Actor_ObjMure3) == (sizeof(Actor) + 0x28), "Actor_ObjMure3 size is wrong");

#endif
