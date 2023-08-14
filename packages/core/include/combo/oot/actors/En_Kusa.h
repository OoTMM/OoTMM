#ifndef COMBO_OOT_EN_KUSA_H
#define COMBO_OOT_EN_KUSA_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_13c[0x54];

    /* Extended flags */
    Xflag xflag;
}
Actor_EnKusa;

_Static_assert(sizeof(Actor_EnKusa) == 0x198, "OoT Actor_EnKusa size is wrong");

#endif
