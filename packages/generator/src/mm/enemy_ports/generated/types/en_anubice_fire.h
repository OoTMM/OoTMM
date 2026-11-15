#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_FIRE_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_FIRE_H

#include <combo.h>

struct EnAnubiceFire;

typedef void (*EnAnubiceFireActionFunc)(struct EnAnubiceFire*, struct PlayState*);

typedef struct EnAnubiceFire {
    Actor actor;
    EnAnubiceFireActionFunc actionFunc;
    f32 scale;
    f32 unk_154;
    s16 unused;
    s16 unk_15A;
    s16 unk_15C;
    s16 unk_15E;
    Vec3f unk_160[6];
    ColliderCylinder colliderCylinder;
} EnAnubiceFire;

#endif
