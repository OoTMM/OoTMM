#ifndef MM_GAME_STATE_H
#define MM_GAME_STATE_H

#include <mm/gfx.h>

typedef struct PACKED
{
    GfxContext*     gfx;
    char            unk_4[0x4];
    void*           destroy;
    void*           nextGameStateInit;
    u32             nextGameStateSize;
    char            unk_14[0x87];
    u8              running;
    char            unk_9c[0x8];
}
GameState;

_Static_assert(sizeof(GameState) == 0xa4, "MM GameState size is wrong");

#endif
