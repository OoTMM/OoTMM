#ifndef MM_GAME_STATE_H
#define MM_GAME_STATE_H

#include <types.h>

typedef struct PACKED
{
    char    unk_0[0x8];
    void*   destroy;
    void*   nextGameStateInit;
    u32     nextGameStateSize;
    char    unk_14[0x87];
    u8      running;
}
MmGameState;

_Static_assert(sizeof(MmGameState) == 0x9c, "MmGameState size is wrong");

#endif
