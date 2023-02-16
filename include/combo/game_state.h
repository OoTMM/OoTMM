#ifndef COMMON_GAME_STATE_H
#define COMMON_GAME_STATE_H

#include <combo/gfx.h>

typedef struct PACKED
{
    u16 buttons;
    s8  x;
    s8  y;
    u16 zero;
}
ControllerInputState;

typedef struct PACKED
{
    ControllerInputState current;
    ControllerInputState previous;
    ControllerInputState pressed;
    ControllerInputState released;
}
ControllerInput;

typedef struct PACKED ALIGNED(4)
{
    GfxContext*             gfx;
    char                    unk_4[0x4];
    void*                   destroy;
    void*                   nextGameStateInit;
    u32                     nextGameStateSize;
    ControllerInput         input[4];
    char                    unk_74[0x27];
    u8                      running;
    u32                     frameCount;
    char                    unk_a0[0x4];
}
GameState;

_Static_assert(sizeof(GameState) == 0xa4, "GameState size is wrong");

#endif
