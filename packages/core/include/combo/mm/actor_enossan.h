#ifndef COMBO_MM_ACTOR_ENOSSAN_H
#define COMBO_MM_ACTOR_ENOSSAN_H

#include <combo/common/actor.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef struct
{
    Actor           base;
    char            unk_144[0x0a4];
    Actor_EnGirlA*  items[8];
    char            unk_208[0x02e];
    u8              itemIndex;
    char            unk_237[0x1d5];
}
Actor_EnOssan;

_Static_assert(sizeof(Actor_EnOssan) == 0x40c, "Actor_EnOssan size is wrong");

ASSERT_OFFSET(Actor_EnOssan, items,         0x1e8);
ASSERT_OFFSET(Actor_EnOssan, itemIndex,     0x236);

#endif
