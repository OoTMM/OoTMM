#ifndef COMBO_COMMON_ACTORS_EN_TUBO_TRAP_H
#define COMBO_COMMON_ACTORS_EN_TUBO_TRAP_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    char            unk_13c[0x60];

    /* Extended flags */
    Xflag xflag;
    u8    isExtended;
}
Actor_EnTuboTrap;

_Static_assert(sizeof(Actor_EnTuboTrap) == (sizeof(Actor) + 0x68), "Actor_EnTuboTrap size is wrong");

int EnTuboTrap_IsShuffled(Actor_EnTuboTrap* this);

#endif
