#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_BEE_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_BEE_H

#include <combo.h>

#ifndef OBJECT_BEE_LIMB_ANTENNAE
#define OBJECT_BEE_LIMB_ANTENNAE 8
#endif
#ifndef OBJECT_BEE_LIMB_BODY
#define OBJECT_BEE_LIMB_BODY 7
#endif
#ifndef OBJECT_BEE_LIMB_HEAD
#define OBJECT_BEE_LIMB_HEAD 9
#endif
#ifndef OBJECT_BEE_LIMB_LEFT_WING
#define OBJECT_BEE_LIMB_LEFT_WING 4
#endif
#ifndef OBJECT_BEE_LIMB_LEFT_WING_ROOT
#define OBJECT_BEE_LIMB_LEFT_WING_ROOT 3
#endif
#ifndef OBJECT_BEE_LIMB_MAX
#define OBJECT_BEE_LIMB_MAX 10
#endif
#ifndef OBJECT_BEE_LIMB_NONE
#define OBJECT_BEE_LIMB_NONE 0
#endif
#ifndef OBJECT_BEE_LIMB_RIGHT_WING
#define OBJECT_BEE_LIMB_RIGHT_WING 6
#endif
#ifndef OBJECT_BEE_LIMB_RIGHT_WING_ROOT
#define OBJECT_BEE_LIMB_RIGHT_WING_ROOT 5
#endif
#ifndef OBJECT_BEE_LIMB_ROOT
#define OBJECT_BEE_LIMB_ROOT 1
#endif
#ifndef OBJECT_BEE_LIMB_WINGS_ROOT
#define OBJECT_BEE_LIMB_WINGS_ROOT 2
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_ObjectBeeLimb
#define COMBO_ENEMY_ASSET_ENUM_ObjectBeeLimb
typedef s32 ObjectBeeLimb;
#endif
struct EnBee;

typedef void (*EnBeeActionFunc)(struct EnBee*, PlayState*);

typedef enum {
    BEE_BEHAVIOR_IDLE,
    BEE_BEHAVIOR_ATTACK
} BeeBehaviorType;

typedef struct EnBee {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[OBJECT_BEE_LIMB_MAX];
    Vec3s morphTable[OBJECT_BEE_LIMB_MAX];
    EnBeeActionFunc actionFunc;
    s16 attackDelayTimer;
    s16 sfxHitTimer;
    s16 isHostile;
    s32 targetYaw;
    s32 flightHoverOffset;
    s32 posIndex;
    s32 instanceId;
    Vec3f targetPos[2];
    s8 pad234[8];
    ColliderCylinder collider;
} EnBee;

#endif
