#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_TANRON3_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_TANRON3_H

#include <combo.h>
#include <combo/enemy_native.h>

#ifndef GYORG_LIMB_BODY_ROOT
#define GYORG_LIMB_BODY_ROOT 3
#endif
#ifndef GYORG_LIMB_HEAD
#define GYORG_LIMB_HEAD 2
#endif
#ifndef GYORG_LIMB_JAW
#define GYORG_LIMB_JAW 14
#endif
#ifndef GYORG_LIMB_JAW_ROOT
#define GYORG_LIMB_JAW_ROOT 13
#endif
#ifndef GYORG_LIMB_LEFT_FIN_ROOT
#define GYORG_LIMB_LEFT_FIN_ROOT 10
#endif
#ifndef GYORG_LIMB_LOWER_LEFT_FIN
#define GYORG_LIMB_LOWER_LEFT_FIN 12
#endif
#ifndef GYORG_LIMB_LOWER_RIGHT_FIN
#define GYORG_LIMB_LOWER_RIGHT_FIN 9
#endif
#ifndef GYORG_LIMB_LOWER_TRUNK
#define GYORG_LIMB_LOWER_TRUNK 5
#endif
#ifndef GYORG_LIMB_MAX
#define GYORG_LIMB_MAX 15
#endif
#ifndef GYORG_LIMB_NONE
#define GYORG_LIMB_NONE 0
#endif
#ifndef GYORG_LIMB_RIGHT_FIN_ROOT
#define GYORG_LIMB_RIGHT_FIN_ROOT 7
#endif
#ifndef GYORG_LIMB_ROOT
#define GYORG_LIMB_ROOT 1
#endif
#ifndef GYORG_LIMB_TAIL
#define GYORG_LIMB_TAIL 6
#endif
#ifndef GYORG_LIMB_UPPER_LEFT_FIN
#define GYORG_LIMB_UPPER_LEFT_FIN 11
#endif
#ifndef GYORG_LIMB_UPPER_RIGHT_FIN
#define GYORG_LIMB_UPPER_RIGHT_FIN 8
#endif
#ifndef GYORG_LIMB_UPPER_TRUNK
#define GYORG_LIMB_UPPER_TRUNK 4
#endif
#ifndef GYORG_SMALL_FISH_LIMB_BODY_ROOT
#define GYORG_SMALL_FISH_LIMB_BODY_ROOT 2
#endif
#ifndef GYORG_SMALL_FISH_LIMB_DORSAL_FIN
#define GYORG_SMALL_FISH_LIMB_DORSAL_FIN 7
#endif
#ifndef GYORG_SMALL_FISH_LIMB_HEAD
#define GYORG_SMALL_FISH_LIMB_HEAD 9
#endif
#ifndef GYORG_SMALL_FISH_LIMB_LEFT_FIN
#define GYORG_SMALL_FISH_LIMB_LEFT_FIN 6
#endif
#ifndef GYORG_SMALL_FISH_LIMB_MAX
#define GYORG_SMALL_FISH_LIMB_MAX 10
#endif
#ifndef GYORG_SMALL_FISH_LIMB_NONE
#define GYORG_SMALL_FISH_LIMB_NONE 0
#endif
#ifndef GYORG_SMALL_FISH_LIMB_RIGHT_FIN
#define GYORG_SMALL_FISH_LIMB_RIGHT_FIN 8
#endif
#ifndef GYORG_SMALL_FISH_LIMB_ROOT
#define GYORG_SMALL_FISH_LIMB_ROOT 1
#endif
#ifndef GYORG_SMALL_FISH_LIMB_TAIL_FIN
#define GYORG_SMALL_FISH_LIMB_TAIL_FIN 4
#endif
#ifndef GYORG_SMALL_FISH_LIMB_TRUNK
#define GYORG_SMALL_FISH_LIMB_TRUNK 5
#endif
#ifndef GYORG_SMALL_FISH_LIMB_TRUNK_ROOT
#define GYORG_SMALL_FISH_LIMB_TRUNK_ROOT 3
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_GyorgLimb
#define COMBO_ENEMY_ASSET_ENUM_GyorgLimb
typedef s32 GyorgLimb;
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_GyorgSmallFishLimb
#define COMBO_ENEMY_ASSET_ENUM_GyorgSmallFishLimb
typedef s32 GyorgSmallFishLimb;
#endif
struct EnTanron3;

typedef void (*EnTanron3ActionFunc)(struct EnTanron3*, PlayState*);

#define TANRON3_WORK_TIMER_MAX 3

typedef struct EnTanron3 {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[GYORG_SMALL_FISH_LIMB_MAX];
    Vec3s morphTable[GYORG_SMALL_FISH_LIMB_MAX];
    s16 timer;
    u8 isNonHostile;
    u8 isBeached;
    s16 workTimer[TANRON3_WORK_TIMER_MAX];
    s16 deathTimer;
    s16 fogTimer;
    Vec3f targetPosWithDeviation;
    Vec3f targetPos;
    Vec3f deviation;
    s16 rotationStep;
    s16 targetRotationStep;
    s16 rotationScale;
    f32 targetSpeedXZ;
    f32 speedMaxStep;
    f32 waterSurfaceYPos;
    Vec3s targetShapeRotation;
    s32 currentRotationAngle;
    s32 nextRotationAngle;
    s16 trunkRotation;
    s16 tailRotation;
    s16 bodyRotation;
    ColliderCylinder atCollider;
    ColliderCylinder acCollider;
    EnTanron3ActionFunc actionFunc;
    f32 standaloneRadius;
    u8 bossOwned;
    EnemyFamily swarm;
    u8 swarmOwner, swarmSpawned, swarmRetry;
} EnTanron3;

#endif
