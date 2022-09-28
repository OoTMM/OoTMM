#ifndef COMBO_MM_ACTOR_ENGIRLA_H
#define COMBO_MM_ACTOR_ENGIRLA_H

#include <combo/common/actor.h>
#include <combo/util.h>
#include <combo/types.h>

typedef struct
{
    Actor   base;
    char    unk_144[0x58];
    s32     gi;
    char    unk_1a0[0x10];
    u16     angle;
    char    unk_1b2[0x1a];
}
Actor_EnGirlA;

_Static_assert(sizeof(Actor_EnGirlA) == 0x1cc, "Actor_EnGirlA size is wrong");

#endif
