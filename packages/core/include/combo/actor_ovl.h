#ifndef COMBO_ACTOR_OVL_H
#define COMBO_ACTOR_OVL_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(0x4)
{
    u32     vromStart;
    u32     vromEnd;
    u32     vramStart;
    u32     vramEnd;
    void*   data;
    void*   dataInit;
    void*   filename;
    u16     type;
    u8      count;
    u8      zero;
}
ActorOvl;

extern ActorOvl gActorOvl[];

_Static_assert(sizeof(ActorOvl) == 0x20, "ActorOvl size is wrong");

#endif
