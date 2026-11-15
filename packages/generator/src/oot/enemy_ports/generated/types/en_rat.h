#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_RAT_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_RAT_H

#include <combo.h>
#include "native_sphere.h"

#ifndef REAL_BOMBCHU_LIMB_BACK_LEFT_LEG
#define REAL_BOMBCHU_LIMB_BACK_LEFT_LEG 7
#endif
#ifndef REAL_BOMBCHU_LIMB_BACK_RIGHT_LEG
#define REAL_BOMBCHU_LIMB_BACK_RIGHT_LEG 9
#endif
#ifndef REAL_BOMBCHU_LIMB_BODY
#define REAL_BOMBCHU_LIMB_BODY 1
#endif
#ifndef REAL_BOMBCHU_LIMB_FRONT_LEFT_LEG
#define REAL_BOMBCHU_LIMB_FRONT_LEFT_LEG 6
#endif
#ifndef REAL_BOMBCHU_LIMB_FRONT_RIGHT_LEG
#define REAL_BOMBCHU_LIMB_FRONT_RIGHT_LEG 8
#endif
#ifndef REAL_BOMBCHU_LIMB_HEAD
#define REAL_BOMBCHU_LIMB_HEAD 2
#endif
#ifndef REAL_BOMBCHU_LIMB_MAX
#define REAL_BOMBCHU_LIMB_MAX 10
#endif
#ifndef REAL_BOMBCHU_LIMB_NONE
#define REAL_BOMBCHU_LIMB_NONE 0
#endif
#ifndef REAL_BOMBCHU_LIMB_TAIL_BASE
#define REAL_BOMBCHU_LIMB_TAIL_BASE 3
#endif
#ifndef REAL_BOMBCHU_LIMB_TAIL_END
#define REAL_BOMBCHU_LIMB_TAIL_END 5
#endif
#ifndef REAL_BOMBCHU_LIMB_TAIL_MIDDLE
#define REAL_BOMBCHU_LIMB_TAIL_MIDDLE 4
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_RealBombchuLimb
#define COMBO_ENEMY_ASSET_ENUM_RealBombchuLimb
typedef s32 RealBombchuLimb;
#endif
#define EN_RAT_IS_OVERWORLD_TYPE(thisx) ((thisx)->params & 0x8000)
#define EN_RAT_GET_TYPE(thisx) ((thisx)->params)
#define EN_RAT_GET_ATTACK_RANGE(thisx) ((thisx)->params & 0xFF)

typedef enum {
    EN_RAT_TYPE_DUNGEON,
    EN_RAT_TYPE_OVERWORLD
} EnRatType;

struct EnRat;

typedef void (*EnRatActionFunc)(struct EnRat*, PlayState*);

typedef struct EnRat {
    Actor actor;
    SkelAnime skelAnime;
    EnRatActionFunc actionFunc;
    u8 hasLostTrackOfPlayer;
    u8 shouldRotateOntoSurfaces;
    s16 animLoopCounter;
    s16 timer;
    union {
                    s16 stunTimer;
                    s16 hookedState;
                } damageReaction;
    Vec3s jointTable[REAL_BOMBCHU_LIMB_MAX];
    Vec3s morphTable[REAL_BOMBCHU_LIMB_MAX];
    Vec3f axisForwards;
    Vec3f axisUp;
    Vec3f axisLeft;
    Vec3f smokePos;
    Vec3f sparkOffsets[2];
    f32 visualJitter;
    f32 attackRange;
    f32 revivePosY;
    s32 blure1Index;
    s32 blure2Index;
    ColliderSphere collider;
} EnRat;

#endif
