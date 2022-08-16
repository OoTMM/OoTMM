#ifndef OOT_ACTOR_H
#define OOT_ACTOR_H

#include <types.h>

typedef struct PACKED ALIGNED(0x4)
{
    u16     id;
    u8      type;
    u8      room;
    s32     flags;
    char    unk[0x14];
    u16     variable;
    char    unk2[0x11e];
}
Actor;

_Static_assert(sizeof(Actor) == 0x13c, "OoT Actor size is wrong");

#endif /* OOT_ACTOR_H */
