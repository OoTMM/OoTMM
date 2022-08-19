#ifndef COMMON_GAME_STATE_H
#define COMMON_GAME_STATE_H

#if defined (GAME_OOT)
# include <oot/gfx.h>
#endif

#if defined (GAME_MM)
# include <mm/gfx.h>
#endif

typedef struct PACKED
{
    GfxContext*     gfx;
    char            unk_4[0x4];
    void*           destroy;
    void*           nextGameStateInit;
    u32             nextGameStateSize;
    char            unk_14[0x87];
    u8              running;
    s32             frameCount;
    char            unk_a0[0x4];
}
GameState;

_Static_assert(sizeof(GameState) == 0xa4, "GameState size is wrong");

#endif
