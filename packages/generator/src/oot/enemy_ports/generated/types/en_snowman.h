#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_SNOWMAN_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_SNOWMAN_H

#include <combo.h>

#ifndef EENO_LIMB_BODY_BOTTOM
#define EENO_LIMB_BODY_BOTTOM 11
#endif
#ifndef EENO_LIMB_BODY_TOP
#define EENO_LIMB_BODY_TOP 2
#endif
#ifndef EENO_LIMB_LEFT_EYE
#define EENO_LIMB_LEFT_EYE 3
#endif
#ifndef EENO_LIMB_LEFT_FOREARM
#define EENO_LIMB_LEFT_FOREARM 6
#endif
#ifndef EENO_LIMB_LEFT_HAND
#define EENO_LIMB_LEFT_HAND 7
#endif
#ifndef EENO_LIMB_LEFT_UPPER_ARM
#define EENO_LIMB_LEFT_UPPER_ARM 5
#endif
#ifndef EENO_LIMB_MAX
#define EENO_LIMB_MAX 12
#endif
#ifndef EENO_LIMB_NONE
#define EENO_LIMB_NONE 0
#endif
#ifndef EENO_LIMB_RIGHT_EYE
#define EENO_LIMB_RIGHT_EYE 4
#endif
#ifndef EENO_LIMB_RIGHT_FOREARM
#define EENO_LIMB_RIGHT_FOREARM 9
#endif
#ifndef EENO_LIMB_RIGHT_HAND
#define EENO_LIMB_RIGHT_HAND 10
#endif
#ifndef EENO_LIMB_RIGHT_UPPER_ARM
#define EENO_LIMB_RIGHT_UPPER_ARM 8
#endif
#ifndef EENO_LIMB_ROOT
#define EENO_LIMB_ROOT 1
#endif
#ifndef EENO_SNOW_PILE_LIMB_BOTTOM
#define EENO_SNOW_PILE_LIMB_BOTTOM 1
#endif
#ifndef EENO_SNOW_PILE_LIMB_MAX
#define EENO_SNOW_PILE_LIMB_MAX 3
#endif
#ifndef EENO_SNOW_PILE_LIMB_NONE
#define EENO_SNOW_PILE_LIMB_NONE 0
#endif
#ifndef EENO_SNOW_PILE_LIMB_TOP
#define EENO_SNOW_PILE_LIMB_TOP 2
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_EenoLimb
#define COMBO_ENEMY_ASSET_ENUM_EenoLimb
typedef s32 EenoLimb;
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_EenoSnowPileLimb
#define COMBO_ENEMY_ASSET_ENUM_EenoSnowPileLimb
typedef s32 EenoSnowPileLimb;
#endif
#define EN_SNOWMAN_GET_TYPE(thisx) ((thisx)->params)
#define EN_SNOWMAN_GET_ATTACK_RANGE(thisx) (((thisx)->params >> 8) & 0xFF)

typedef enum {
    EN_SNOWMAN_TYPE_SMALL,
    EN_SNOWMAN_TYPE_LARGE,
    EN_SNOWMAN_TYPE_SPLIT,
    EN_SNOWMAN_TYPE_SMALL_SNOWBALL,
    EN_SNOWMAN_TYPE_LARGE_SNOWBALL
} EnSnowmanType;

struct EnSnowman;

typedef void (*EnSnowmanActionFunc)(struct EnSnowman*, PlayState*);

typedef enum EnSnowmanBodyPart {
    EN_SNOWMAN_BODYPART_0,
    EN_SNOWMAN_BODYPART_1,
    EN_SNOWMAN_BODYPART_2,
    EN_SNOWMAN_BODYPART_3,
    EN_SNOWMAN_BODYPART_4,
    EN_SNOWMAN_BODYPART_5,
    EN_SNOWMAN_BODYPART_6,
    EN_SNOWMAN_BODYPART_7,
    EN_SNOWMAN_BODYPART_8,
    EN_SNOWMAN_BODYPART_MAX
} EnSnowmanBodyPart;

typedef struct EnSnowman {
    Actor actor;
    SkelAnime skelAnime;
    SkelAnime snowPileSkelAnime;
    Vec3s jointTable[EENO_LIMB_MAX];
    Vec3s morphTable[EENO_LIMB_MAX];
    Vec3s snowPileJointTable[EENO_SNOW_PILE_LIMB_MAX];
    Vec3s snowPileMorphTable[EENO_SNOW_PILE_LIMB_MAX];
    EnSnowmanActionFunc actionFunc;
    EnSnowmanActionFunc prevActionFunc;
    u8 isHoldingSnowball;
    u8 combineState;
    u8 turningOnSteepSlope;
    u8 drawDmgEffType;
    union {
                    s16 timer;
                    s16 snowballsToThrowBeforeIdling;
                } work;
    s16 snowPileTargetRotY;
    s16 combineTimer;
    f32 eenoScale;
    union {
                    f32 frameToStartHoldingSnowball;
                    f32 frameToThrowSnowball;
                    f32 targetScaleDuringCombine;
                } fwork;
    f32 attackRange;
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    Vec3f combinePos;
    Vec3f snowballPos;
    Vec3f bodyPartsPos[EN_SNOWMAN_BODYPART_MAX];
    ColliderCylinder collider;
} EnSnowman;

#endif
