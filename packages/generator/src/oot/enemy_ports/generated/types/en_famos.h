#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_FAMOS_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_FAMOS_H

#include <combo.h>

#ifndef FAMOS_LIMB_BODY
#define FAMOS_LIMB_BODY 1
#endif
#ifndef FAMOS_LIMB_EMBLEM
#define FAMOS_LIMB_EMBLEM 2
#endif
#ifndef FAMOS_LIMB_HEAD
#define FAMOS_LIMB_HEAD 5
#endif
#ifndef FAMOS_LIMB_MAX
#define FAMOS_LIMB_MAX 6
#endif
#ifndef FAMOS_LIMB_NONE
#define FAMOS_LIMB_NONE 0
#endif
#ifndef FAMOS_LIMB_SHIELD
#define FAMOS_LIMB_SHIELD 4
#endif
#ifndef FAMOS_LIMB_SWORD
#define FAMOS_LIMB_SWORD 3
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_FamosLimb
#define COMBO_ENEMY_ASSET_ENUM_FamosLimb
typedef s32 FamosLimb;
#endif
struct EnFamos;

typedef void (*EnFamosActionFunc)(struct EnFamos*, PlayState*);

typedef struct EnFamosRock {
    Vec3f pos;
    Vec3f velocity;
    Vec3s rot;
    f32 scale;
} EnFamosRock;

#define FAMOS_GET_PATH_INDEX(thisx) ((thisx)->params)
#define FAMOS_GET_AGGRO_DISTANCE(thisx) ((thisx)->shape.rot.x)

#define FAMOS_PATH_INDEX_NONE 0xFF

typedef struct EnFamos {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[FAMOS_LIMB_MAX];
    Vec3s morphTable[FAMOS_LIMB_MAX];
    EnFamosActionFunc actionFunc;
    u8 animatedMaterialIndex;
    u8 hasFinishedRotating;
    u8 pathCount;
    u8 waypointIndex;
    u8 isCalm;
    s16 hoverTimer;
    s16 stateTimer;
    s16 debrisTimer;
    s16 flippedTimer;
    s16 cratorDespawnTimer;
    s16 targetYaw;
    s16 flipRot;
    Vec3s* pathPoints;
    f32 baseHeight;
    f32 aggroDistance;
    Vec3f targetDest;
    Vec3f calmPos;
    ColliderCylinder collider1;
    ColliderCylinder collider2;
    ColliderJntSph   emblemCollider;
    ColliderJntSphElement emblemColliderElements[2];
    EnFamosRock rocks[20];
} EnFamos;

#endif
