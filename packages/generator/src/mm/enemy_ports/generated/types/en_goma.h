#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_GOMA_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_GOMA_H

#include <combo.h>

typedef enum GomaType {
    ENGOMA_NORMAL,
    ENGOMA_EGG,
    ENGOMA_HATCH_DEBRIS,
    ENGOMA_BOSSLIMB
} GomaType;

struct EnGoma;

typedef void (*EnGomaActionFunc)(struct EnGoma*, struct PlayState*);

typedef enum EnGomaLimb {
    GOMA_LIMB_NONE,
    GOMA_LIMB_ROOT1,
    GOMA_LIMB_ROOT2,
    GOMA_LIMB_BODY,
    GOMA_LIMB_ANTENNA_ROOT1,
    GOMA_LIMB_ANTENNA_ROOT2,
    GOMA_LIMB_ANTENNA,
    GOMA_LIMB_EYE_IRIS_ROOT1,
    GOMA_LIMB_EYE_IRIS_ROOT2,
    GOMA_LIMB_EYE_IRIS,
    GOMA_LIMB_L_LEG_ROOT1,
    GOMA_LIMB_L_LEG_ROOT2,
    GOMA_LIMB_L_SHIN_ROOT,
    GOMA_LIMB_L_FOOT_ROOT,
    GOMA_LIMB_L_FOOT,
    GOMA_LIMB_L_SHIN,
    GOMA_LIMB_L_THIGH,
    GOMA_LIMB_R_LEG_ROOT1,
    GOMA_LIMB_R_LEG_ROOT2,
    GOMA_LIMB_R_SHIN_ROOT,
    GOMA_LIMB_R_FOOT_ROOT,
    GOMA_LIMB_R_FOOT,
    GOMA_LIMB_R_SHIN,
    GOMA_LIMB_R_THIGH,
    GOMA_LIMB_MAX
} EnGomaLimb;

typedef struct EnGoma {
    Actor actor;
    SkelAnime skelanime;
    Vec3s jointTable[24];
    Vec3s morphTable[24];
    EnGomaActionFunc actionFunc;
    s16 slopePitch;
    s16 slopeRoll;
    s16 gomaType;
    s16 eyePitch;
    s16 eyeYaw;
    s16 hatchState;
    s16 eggTimer;
    s16 hurtTimer;
    s16 visualState;
    s16 playerDetectionTimer;
    s16 spawnNum;
    s16 invincibilityTimer;
    s16 actionTimer;
    f32 eggScale;
    f32 eggPitch;
    f32 eggSquishAngle;
    f32 eggSquishAccel;
    f32 eyeEnvColor[3];
    f32 eggSquishAmount;
    f32 eggYOffset;
    s32 unk_2F4;
    s16 stunTimer;
    Vec3f shieldKnockbackVel;
    Gfx* bossLimbDL;
    ColliderCylinder colliderCylinder1;
    ColliderCylinder colliderCylinder2;
} EnGoma;

#endif
