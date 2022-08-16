#ifndef OOT_GAME_STATE_H
#define OOT_GAME_STATE_H

#include <types.h>

typedef struct PACKED ALIGNED(0x4)
{
    char unk[0xa4];
}
GameState;

_Static_assert(sizeof(GameState) == 0xa4, "OoT GameState size is wrong");

#endif
