#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_EGOL_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_EGOL_H

#include <combo.h>

#ifndef EYEGORE_LIMB_EYEBALL
#define EYEGORE_LIMB_EYEBALL 15
#endif
#ifndef EYEGORE_LIMB_EYE_COLLIDER
#define EYEGORE_LIMB_EYE_COLLIDER 14
#endif
#ifndef EYEGORE_LIMB_FAULDS
#define EYEGORE_LIMB_FAULDS 23
#endif
#ifndef EYEGORE_LIMB_HEAD
#define EYEGORE_LIMB_HEAD 3
#endif
#ifndef EYEGORE_LIMB_HIPS
#define EYEGORE_LIMB_HIPS 16
#endif
#ifndef EYEGORE_LIMB_LASER_ATTACH
#define EYEGORE_LIMB_LASER_ATTACH 13
#endif
#ifndef EYEGORE_LIMB_LEFT_ARM
#define EYEGORE_LIMB_LEFT_ARM 5
#endif
#ifndef EYEGORE_LIMB_LEFT_FOOT
#define EYEGORE_LIMB_LEFT_FOOT 19
#endif
#ifndef EYEGORE_LIMB_LEFT_HAND
#define EYEGORE_LIMB_LEFT_HAND 6
#endif
#ifndef EYEGORE_LIMB_LEFT_SHIN
#define EYEGORE_LIMB_LEFT_SHIN 18
#endif
#ifndef EYEGORE_LIMB_LEFT_SHOULDER
#define EYEGORE_LIMB_LEFT_SHOULDER 4
#endif
#ifndef EYEGORE_LIMB_LEFT_THIGH
#define EYEGORE_LIMB_LEFT_THIGH 17
#endif
#ifndef EYEGORE_LIMB_LOWER_EYELID
#define EYEGORE_LIMB_LOWER_EYELID 11
#endif
#ifndef EYEGORE_LIMB_MAX
#define EYEGORE_LIMB_MAX 24
#endif
#ifndef EYEGORE_LIMB_NONE
#define EYEGORE_LIMB_NONE 0
#endif
#ifndef EYEGORE_LIMB_PUPIL
#define EYEGORE_LIMB_PUPIL 12
#endif
#ifndef EYEGORE_LIMB_RIGHT_ARM
#define EYEGORE_LIMB_RIGHT_ARM 8
#endif
#ifndef EYEGORE_LIMB_RIGHT_FOOT
#define EYEGORE_LIMB_RIGHT_FOOT 22
#endif
#ifndef EYEGORE_LIMB_RIGHT_HAND
#define EYEGORE_LIMB_RIGHT_HAND 9
#endif
#ifndef EYEGORE_LIMB_RIGHT_SHIN
#define EYEGORE_LIMB_RIGHT_SHIN 21
#endif
#ifndef EYEGORE_LIMB_RIGHT_SHOULDER
#define EYEGORE_LIMB_RIGHT_SHOULDER 7
#endif
#ifndef EYEGORE_LIMB_RIGHT_THIGH
#define EYEGORE_LIMB_RIGHT_THIGH 20
#endif
#ifndef EYEGORE_LIMB_ROOT
#define EYEGORE_LIMB_ROOT 1
#endif
#ifndef EYEGORE_LIMB_TORSO
#define EYEGORE_LIMB_TORSO 2
#endif
#ifndef EYEGORE_LIMB_UPPER_EYELID
#define EYEGORE_LIMB_UPPER_EYELID 10
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_EyegoreLimb
#define COMBO_ENEMY_ASSET_ENUM_EyegoreLimb
typedef s32 EyegoreLimb;
#endif
typedef struct
{
    s16 r, g, b;
} EnemyEyegoreColorRgb16;

struct EnEgol;

typedef void (*EnEgolActionFunc)(struct EnEgol*, PlayState*);

#define EYEGORE_EFFECT_COUNT 100

typedef struct {
    u8 isActive;
    Vec3f pos;
    s16 alpha;
    s16 timer;
    s16 type;
    Vec3f velocity;
    Vec3f accel;
    Vec3s rot;
    f32 scale;
} EnEgolEffect;

typedef enum EyegoreBodyPart {
    EYEGORE_BODYPART_0,
    EYEGORE_BODYPART_1,
    EYEGORE_BODYPART_2,
    EYEGORE_BODYPART_3,
    EYEGORE_BODYPART_4,
    EYEGORE_BODYPART_5,
    EYEGORE_BODYPART_6,
    EYEGORE_BODYPART_7,
    EYEGORE_BODYPART_8,
    EYEGORE_BODYPART_9,
    EYEGORE_BODYPART_10,
    EYEGORE_BODYPART_11,
    EYEGORE_BODYPART_MAX
} EyegoreBodyPart;

typedef struct EnEgol {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[EYEGORE_LIMB_MAX];
    Vec3s morphTable[EYEGORE_LIMB_MAX];
    EnEgolActionFunc actionFunc;
    Path* path;
    s16 waypoint;
    s16 action;
    s16 waitTimer;
    s16 eyeShutTimer;
    s16 headRot;
    s16 switchFlag;
    s16 dmgEffectTimer;
    Vec3f bodyPartsPos[EYEGORE_BODYPART_MAX];
    s16 bodyPartIndex;
    s16 pathIndex;
    s32 animIndex;
    u8 isRetreating;
    f32 animEndFrame;
    f32 wakeupRange;
    f32 minLaserRange;
    s8 unk_368[4];
    f32 laserLightScale;
    f32 chargeLightScale;
    s16 eyelidRot;
    s16 eyelidRotTarget;
    s16 actionTimer;
    s32 laserCount;
    s32 chargingLaser;
    s16 chargeLightRot;
    s32 chargeLevel;
    s32 hitPlayer;
    s8 unk_390[0xC];
    Vec3f eyePos;
    Vec3f laserBase;
    Vec3f laserScale;
    Vec3f laserScaleTarget;
    s8 unk_3CC[0xC];
    Vec3f leftFootPos;
    Vec3f rightFootPos;
    Vec3f leftHandPos;
    Vec3f rightHandPos;
    Vec3f waypointPos;
    Vec3s pupilRot;
    Vec3s laserRot;
    EnemyEyegoreColorRgb16 eyePrimColor;
    EnemyEyegoreColorRgb16 eyeEnvColor;
    s16 laserState;
    s32 texScroll;
    EnEgolEffect effects[EYEGORE_EFFECT_COUNT];
    ColliderJntSph bodyCollider;
    ColliderJntSphElement bodyElements[6];
    ColliderJntSph eyeCollider;
    ColliderJntSphElement eyeElements[1];
    ColliderQuad laserCollider;
    s8 unk_1E24[4];
    s16 subCamId;
    f32 subCamFov;
    f32 subCamFovTarget;
    Vec3f subCamAt;
    Vec3f subCamEye;
    s8 unk_1E4C[0x24];
} EnEgol;

#endif
