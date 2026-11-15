#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_WDHAND_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_WDHAND_H

#include <combo.h>

#ifndef DEXIHAND_LIMB_01
#define DEXIHAND_LIMB_01 1
#endif
#ifndef DEXIHAND_LIMB_02
#define DEXIHAND_LIMB_02 2
#endif
#ifndef DEXIHAND_LIMB_03
#define DEXIHAND_LIMB_03 3
#endif
#ifndef DEXIHAND_LIMB_04
#define DEXIHAND_LIMB_04 4
#endif
#ifndef DEXIHAND_LIMB_05
#define DEXIHAND_LIMB_05 5
#endif
#ifndef DEXIHAND_LIMB_06
#define DEXIHAND_LIMB_06 6
#endif
#ifndef DEXIHAND_LIMB_07
#define DEXIHAND_LIMB_07 7
#endif
#ifndef DEXIHAND_LIMB_MAX
#define DEXIHAND_LIMB_MAX 8
#endif
#ifndef DEXIHAND_LIMB_NONE
#define DEXIHAND_LIMB_NONE 0
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_DexihandLimb
#define COMBO_ENEMY_ASSET_ENUM_DexihandLimb
typedef s32 DexihandLimb;
#endif
struct EnWdhand;

typedef void (*EnWdhandActionFunc)(struct EnWdhand*, PlayState*);

#define EN_WDHAND_NUM_SEGMENTS 3
#define EN_WDHAND_NUM_COLLIDER_ELEMENTS (2 * EN_WDHAND_NUM_SEGMENTS + 1)

#define EN_WDHAND_CUT_POINT  0
#define EN_WDHAND_HAND_POINT 1

#define EN_WDHAND_INIT_VELOCITY_MAX 0x7F
#define EN_WDHAND_GET_Y_INIT_VELOCITY(thisx) (((thisx)->params >> 0) & 0x7F)
#define EN_WDHAND_GET_Z_INIT_VELOCITY(thisx) (((thisx)->params >> 7) & 0x7F)

typedef struct EnWdhand {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[DEXIHAND_LIMB_MAX];
    Vec3s morphTable[DEXIHAND_LIMB_MAX];
    EnWdhandActionFunc actionFunc;
    s16 limbIndexBeforeCut;
    s16 limbIndexAfterCut;
    s16 timer;
    Vec3s limbRotations[EN_WDHAND_NUM_SEGMENTS + 1];
    f32 globalLimbScaleFactor;
    f32 limbScaleFactors[EN_WDHAND_NUM_SEGMENTS];
    MtxF relativeToWorldTransform;
    Vec3f endPoints[2];
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[EN_WDHAND_NUM_COLLIDER_ELEMENTS];
} EnWdhand;

#endif
