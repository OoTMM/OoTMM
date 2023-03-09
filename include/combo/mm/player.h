#ifndef COMBO_MM_PLAYER_H
#define COMBO_MM_PLAYER_H

#include <ultra64.h>
#include <combo/common/actor.h>

typedef struct PACKED ALIGNED(4)
{
    Actor       base;
    char        unk_144[0x98];
    OSMesgQueue objMsgQueue;
    OSMesg      objMsg;
    void*       objBuffer;
    char        unk_1fc[0x198];
    u8          csMode;
    char        unk_395[0x6D7];
    u32         state;
    u32         state2;
    u32         state3;
    char        unk_a78[0x300];
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xd78, "MM Actor_Player size is wrong");

#endif
