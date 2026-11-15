#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_ESTONE_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_ESTONE_H

#include <combo.h>

struct EnEstone;

typedef void (*EnEstoneActionFunc)(struct EnEstone*, PlayState*);

#define ENESTONE_EFFECT_COUNT 9

typedef enum {
    ENESTONE_TYPE_LARGE,
    ENESTONE_TYPE_SMALL
} EnEstoneType;

typedef struct {
    u8 isActive;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Vec3s rot;
    f32 scale;
    s16 timer;
} EnEstoneEffect;

typedef struct EnEstone {
    Actor actor;
    EnEstoneActionFunc actionFunc;
    f32 scale;
    Vec3f rot;
    Vec3f rotVel;
    s16 timer;
    s16 inactive;
    ColliderCylinder collider;
    EnEstoneEffect effects[ENESTONE_EFFECT_COUNT];
    u16 standaloneLifetime;
} EnEstone;

#endif
