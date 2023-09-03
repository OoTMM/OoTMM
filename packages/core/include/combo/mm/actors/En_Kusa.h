#ifndef COMBO_MM_EN_KUSA_H
#define COMBO_MM_EN_KUSA_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_144[0x58];

    /* Extended flags */
    Xflag xflag;
}
Actor_EnKusa;

_Static_assert(sizeof(Actor_EnKusa) == 0x1a4, "MM Actor_EnKusa size is wrong");

#endif
