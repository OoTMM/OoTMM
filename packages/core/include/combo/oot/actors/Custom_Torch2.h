#ifndef COMBO_OOT_CUSTOM_TORCH2_H
#define COMBO_OOT_CUSTOM_TORCH2_H

#include <combo/common/actor.h>

typedef struct Actor_CustomEnTorch2
{
    Actor base;
    ColliderCylinder collider;
    u8 state;
    u8 framesUntilNextState;
    s16 alpha;
} Actor_CustomEnTorch2;

#define TORCH2_PARAM_HUMAN 0
#define TORCH2_PARAM_GORON 1
#define TORCH2_PARAM_ZORA 2
#define TORCH2_PARAM_DEKU 3
#define TORCH2_PARAM_FIERCE_DEITY 4

#define TORCH2_STATE_INITIALIZED 0
#define TORCH2_STATE_FADING_IN 1
#define TORCH2_STATE_SOLID 2
#define TORCH2_STATE_IDLE 3
#define TORCH2_STATE_DYING 4

#endif /* COMBO_OOT_CUSTOM_TORCH2_H */
