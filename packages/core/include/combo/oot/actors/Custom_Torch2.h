#ifndef Z_EN_TORCH2_H
#define Z_EN_TORCH2_H

#include <combo/common/actor.h>

typedef struct Actor_CustomEnTorch2 {
    /* 0x000 */ Actor base;
    /* 0x13c */ ColliderCylinder collider;
    /* 0x188 */ u8 state;
    /* 0x189 */ u8 framesUntilNextState;
    /* 0x18a */ s16 alpha;
} Actor_CustomEnTorch2; // size = 0x18c

#define TORCH2_PARAM_HUMAN          0
#define TORCH2_PARAM_GORON          1
#define TORCH2_PARAM_ZORA           2
#define TORCH2_PARAM_DEKU           3
#define TORCH2_PARAM_FIERCE_DEITY   4

#define TORCH2_STATE_INITIALIZED    0
#define TORCH2_STATE_FADING_IN      1
#define TORCH2_STATE_SOLID          2
#define TORCH2_STATE_IDLE           3
#define TORCH2_STATE_DYING          4

#endif // Z_EN_TORCH2_H
