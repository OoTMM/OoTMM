#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_BAGUO_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_BAGUO_H

#include <combo.h>

#ifndef NEJIRON_LIMB_BODY
#define NEJIRON_LIMB_BODY 1
#endif
#ifndef NEJIRON_LIMB_EYES
#define NEJIRON_LIMB_EYES 2
#endif
#ifndef NEJIRON_LIMB_MAX
#define NEJIRON_LIMB_MAX 3
#endif
#ifndef NEJIRON_LIMB_NONE
#define NEJIRON_LIMB_NONE 0
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_NejironLimb
#define COMBO_ENEMY_ASSET_ENUM_NejironLimb
typedef s32 NejironLimb;
#endif
struct EnBaguo;

typedef void (*EnBaguoActionFunc)(struct EnBaguo*, PlayState*);

typedef struct NejironEffect {
    u8 isEnabled;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Vec3s rot;
    f32 scale;
    s16 timer;
} NejironEffect;

#define EN_BAGUO_EFFECT_COUNT 30

typedef struct EnBaguo {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[NEJIRON_LIMB_MAX];
    Vec3s morphTable[NEJIRON_LIMB_MAX];
    EnBaguoActionFunc actionFunc;
    s16 eyeIndex;
    s16 blinkTimer;
    s16 timer;
    s16 action;
    s16 zRollDirection;
    f32 maxDistanceFromHome;
    u8 bouncedFlag;
    Vec3f currentRotation;
    Vec3f targetRotation;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    NejironEffect effects[EN_BAGUO_EFFECT_COUNT];
} EnBaguo;

#endif
