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

typedef struct {
    /* 0x00 */ void* loadedRamAddr;
    /* 0x04 */ u32 vromStart;
    /* 0x08 */ u32 vromEnd;
    /* 0x0C */ u32 vramStart;
    /* 0x10 */ u32 vramEnd;
    /* 0x14 */ u8 pad14[0x4];
    /* 0x18 */ char* filename;
} PlayerOverlay; // size = 0x1C

_Static_assert(sizeof(PlayerOverlay) == 0x1C, "PlayerOverlay size is wrong");

#endif
