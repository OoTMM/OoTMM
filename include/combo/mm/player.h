#ifndef COMBO_MM_PLAYER_H
#define COMBO_MM_PLAYER_H

#include <ultra64.h>
#include <combo/mm/actor.h>

typedef struct PACKED ALIGNED(4)
{
    Actor       base;
    char        unk_144[0x98];
    OSMesgQueue objMsgQueue;
    OSMesg      objMsg;
    void*       objBuffer;
    char        unk_1fc[0xb7c];

}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xd78, "MM Actor_Player size is wrong");

#endif
