#ifndef COMBO_OOT_OBJ_TSUBO_H
#define COMBO_OOT_OBJ_TSUBO_H

#include <combo/common/actor.h>

typedef struct
{
    Actor           base;
    char            unk_13c[0x54];

    /* Extended flags */
    u8              extendedRoomId;
    u8              extendedId;
    u8              pad0[2];
}
Actor_ObjTsubo;

_Static_assert(sizeof(Actor_ObjTsubo) == 0x194, "OoT Actor_ObjTsubo size is wrong");

#endif
