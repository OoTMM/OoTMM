#ifndef COMBO_MM_OBJ_TSUBO_H
#define COMBO_MM_OBJ_TSUBO_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_144[0x58];

    /* Extended flags */
    Xflag xflag;
}
Actor_ObjTsubo;

_Static_assert(sizeof(Actor_ObjTsubo) == 0x1a4, "MM Actor_ObjTsubo size is wrong");

#endif
