#ifndef COMMON_GAME_STATE_H
#define COMMON_GAME_STATE_H

#include <combo/tha.h>
#include <combo/controller.h>
#include <combo/gfx.h>

typedef struct GameState GameState;
typedef struct GameStateOverlay GameStateOverlay;

struct GameState
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
};

_Static_assert(sizeof(GameState) == 0xa4, "GameState size is wrong");

struct GameStateOverlay
{
    void* ram;
    u32   vromStart;
    u32   vromEnd;
    u32   vramStart;
    u32   vramEnd;
    u32   unk_14;
    void* ctor;
    void* dtor;
    char  unk_20[0xc];
    u32   size;
};

_Static_assert(sizeof(GameStateOverlay) == 0x30, "GameStateOverlay size is wrong");

extern GameStateOverlay gGameStateOverlayTable[];

#endif
