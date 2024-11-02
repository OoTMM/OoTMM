#ifndef COMMON_GAME_STATE_H
#define COMMON_GAME_STATE_H

#include <combo/tha.h>
#include <combo/controller.h>
#include <combo/gfx.h>

typedef struct GameState
{
    GraphicsContext*        gfxCtx;
    void*                   main;
    void*                   destroy;
    void*                   nextGameStateInit;
    u32                     nextGameStateSize;
    Input                   input[4];
    TwoHeadArena            tha;
    char                    unk_84[0x17];
    u8                      running;
    u32                     frameCount;
    char                    unk_a0[0x4];
}
GameState;

_Static_assert(sizeof(GameState) == 0xa4, "GameState size is wrong");

#endif
