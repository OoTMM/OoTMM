#ifndef COMBO_MM_ACTOR_ENTRT_H
#define COMBO_MM_ACTOR_ENTRT_H

#include <combo/common/actor.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef struct
{
    Actor           base;
    char            unk_144[0x1fc];
    Actor_EnGirlA*  items[3];
    u8              itemIndex;
    char            unk_34d[0x0e3];
}
Actor_EnTrt;

_Static_assert(sizeof(Actor_EnTrt) == 0x430, "Actor_EnTrt size is wrong");
ASSERT_OFFSET(Actor_EnTrt, items,           0x340);
ASSERT_OFFSET(Actor_EnTrt, itemIndex,       0x34c);

#endif
