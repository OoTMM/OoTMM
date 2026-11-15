#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_BAT_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_BAT_H

#include <combo.h>
#include "native_sphere.h"

#define BAD_BAT_GET_TYPE(thisx) (((thisx)->params >> 0) & 0x1F)
#define BAD_BAT_GET_PARAMFLAGS(thisx) (((thisx)->params >> 5) & 7)
#define BAD_BAT_GET_SWITCH_FLAG(thisx) (((thisx)->params >> 8) & 0xFF)
#define BAD_BAT_PARAMS(switchFlag, paramflags, type) ((type) | ((paramflags) << 5) | ((switchFlag) << 8))

#define BAD_BAT_PARAMFLAG_CHECK_HEIGHTREL (1 << 1)
#define BAD_BAT_PARAMFLAG_PERCH (1 << 2)

#define BAD_BAT_GET_NUMBER_TO_SPAWN(thisx) ((thisx)->params)

struct EnBat;

typedef void (*EnBatActionFunc)(struct EnBat*, PlayState*);

typedef enum BadBatBodyPart {
    BAD_BAT_BODYPART_0,
    BAD_BAT_BODYPART_1,
    BAD_BAT_BODYPART_2,
    BAD_BAT_BODYPART_MAX
} BadBatBodyPart;

typedef struct EnBat {
    Actor actor;
    EnBatActionFunc actionFunc;
    u8 drawDmgEffType;
    u8 paramFlags;
    s16 switchFlag;
    s16 timer;
    s16 pitchTarget;
    s16 yawTarget;
    s16 animationFrame;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    f32 drawDmgEffAlpha;
    Vec3f bodyPartsPos[BAD_BAT_BODYPART_MAX];
    ColliderSphere collider;
} EnBat;

#endif
