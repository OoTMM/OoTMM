#ifndef COMBO_OOT_EN_TUBO_TRAP_H
#define COMBO_OOT_EN_TUBO_TRAP_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_13c[0x60];

    /* Extended flags */
    Xflag xflag;
}
Actor_EnTuboTrap;

_Static_assert(sizeof(Actor_EnTuboTrap) == 0x1a0, "OoT Actor_ObjTsubo size is wrong");

#endif
