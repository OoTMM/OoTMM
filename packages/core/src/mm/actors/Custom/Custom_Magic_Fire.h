#ifndef Z_MAGIC_FIRE_H
#define Z_MAGIC_FIRE_H

#include <combo.h>

struct MagicFire;

typedef struct MagicFire {
    /* 0x0000 */ Actor actor;
    /* 0x013C */ ColliderCylinder collider;
    /* 0x0188 */ f32 alphaMultiplier;
    /* 0x018C */ f32 screenTintIntensity;
    /* 0x0190 */ f32 scalingSpeed;
    /* 0x0194 */ s16 action;
    /* 0x0196 */ s16 screenTintBehaviour;
    /* 0x0198 */ s16 actionTimer;
    /* 0x019A */ s16 screenTintBehaviourTimer;
} MagicFire; // size = 0x019C

#endif
