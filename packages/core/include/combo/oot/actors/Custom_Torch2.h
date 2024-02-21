#ifndef Z_EN_TORCH2_H
#define Z_EN_TORCH2_H

#include <combo/common/actor.h>

struct EnTorch2;

typedef struct EnTorch2 {
    /* 0x000 */ Actor base;
    /* 0x13c */ ColliderCylinder collider;
    /* 0x188 */ u8 state;
    /* 0x189 */ u8 framesUntilNextState;
    /* 0x18a */ s16 alpha;
} EnTorch2; // size = 0x18c

typedef enum {
    /* 0 */ TORCH2_PARAM_HUMAN,
    /* 1 */ TORCH2_PARAM_GORON,
    /* 2 */ TORCH2_PARAM_ZORA,
    /* 3 */ TORCH2_PARAM_DEKU,
    /* 4 */ TORCH2_PARAM_FIERCE_DEITY
} EnTorch2Param;

typedef enum {
    /* 0 */ TORCH2_STATE_INITIALIZED,
    /* 1 */ TORCH2_STATE_FADING_IN,
    /* 2 */ TORCH2_STATE_SOLID,
    /* 3 */ TORCH2_STATE_IDLE,
    /* 4 */ TORCH2_STATE_DYING
} EnTorch2State;

#endif // Z_EN_TORCH2_H
