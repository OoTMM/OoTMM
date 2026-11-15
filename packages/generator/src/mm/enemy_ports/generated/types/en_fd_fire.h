#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FD_FIRE_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FD_FIRE_H

#include <combo.h>

struct EnFdFire;

typedef void (*EnFdFireActionFunc)(struct EnFdFire* this, struct PlayState* play);

typedef struct EnFdFire {
    Actor actor;
    EnFdFireActionFunc actionFunc;
    ColliderCylinder collider;
    f32 spawnRadius;
    f32 scale;
    char unk_1A4[0x2];
    s16 deathTimer;
    s16 tile2Y;
} EnFdFire;

#endif
