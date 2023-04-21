#ifndef COMBO_MM_ACTOR_ENFSN_H
#define COMBO_MM_ACTOR_ENFSN_H

#include <combo/common/actor.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef struct
{
    Actor           base;
    char            unk_144[0x234];
    u8              mode;
    u8              itemIndex;
    char            unk_37a[0x016];
    Actor_EnGirlA*  items[4];
    char            unk_3a0[0x0b0];
}
Actor_EnFsn;

_Static_assert(sizeof(Actor_EnFsn) == 0x450, "Actor_EnFsn size is wrong");

#endif
