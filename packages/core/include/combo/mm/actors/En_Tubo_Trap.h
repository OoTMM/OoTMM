#ifndef COMBO_MM_EN_TUBO_TRAP_H
#define COMBO_MM_EN_TUBO_TRAP_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_144[0x60];

    /* Extended flags */
    Xflag xflag;
}
Actor_EnTuboTrap;

_Static_assert(sizeof(Actor_EnTuboTrap) == 0x1ac, "MM Actor_EnTuboTrap size is wrong");

#endif
