#ifndef COMBO_OOT_PLAYER_H
#define COMBO_OOT_PLAYER_H

#include <ultra64.h>
#include <combo/common/actor.h>

typedef struct PACKED ALIGNED(4) Actor_Player
{
    Actor           base;
    char            unk_13c[0x48];
    OSMesgQueue     objMsgQueue;
    OSMesg          objMsg;
    void*           objBuffer;
    char            unk_1a4[0x4c8];
    u32             state;
    u32             state2;
    char            unk_674[0x3f8];
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xa6c, "Actor_Player size is wrong");

#endif
