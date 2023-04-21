#ifndef COMBO_MM_ACTOR_ENSOB1_H
#define COMBO_MM_ACTOR_ENSOB1_H

#include <combo/common/actor.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef struct
{
    Actor           base;
    char            unk_144[0x1a8];
    Actor_EnGirlA*  items[3];
    char            unk_2f8[0x2a];
    u8              itemIndex;
    char            unk_323[0xad];
}
Actor_EnSob1;

_Static_assert(sizeof(Actor_EnSob1) == 0x3d0, "Actor_EnSob1 size is wrong");

#endif
