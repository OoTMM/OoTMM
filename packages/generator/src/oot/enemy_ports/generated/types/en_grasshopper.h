#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_GRASSHOPPER_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_GRASSHOPPER_H

#include <combo.h>

#ifndef DRAGONFLY_LIMB_BACK_LEFT_FOOT
#define DRAGONFLY_LIMB_BACK_LEFT_FOOT 12
#endif
#ifndef DRAGONFLY_LIMB_BACK_LEFT_LOWER_LEG
#define DRAGONFLY_LIMB_BACK_LEFT_LOWER_LEG 11
#endif
#ifndef DRAGONFLY_LIMB_BACK_LEFT_UPPER_LEG
#define DRAGONFLY_LIMB_BACK_LEFT_UPPER_LEG 10
#endif
#ifndef DRAGONFLY_LIMB_BACK_RIGHT_FOOT
#define DRAGONFLY_LIMB_BACK_RIGHT_FOOT 15
#endif
#ifndef DRAGONFLY_LIMB_BACK_RIGHT_LOWER_LEG
#define DRAGONFLY_LIMB_BACK_RIGHT_LOWER_LEG 14
#endif
#ifndef DRAGONFLY_LIMB_BACK_RIGHT_UPPER_LEG
#define DRAGONFLY_LIMB_BACK_RIGHT_UPPER_LEG 13
#endif
#ifndef DRAGONFLY_LIMB_FRONT_LEFT_FOOT
#define DRAGONFLY_LIMB_FRONT_LEFT_FOOT 18
#endif
#ifndef DRAGONFLY_LIMB_FRONT_LEFT_LOWER_LEG
#define DRAGONFLY_LIMB_FRONT_LEFT_LOWER_LEG 17
#endif
#ifndef DRAGONFLY_LIMB_FRONT_LEFT_UPPER_LEG
#define DRAGONFLY_LIMB_FRONT_LEFT_UPPER_LEG 16
#endif
#ifndef DRAGONFLY_LIMB_FRONT_RIGHT_FOOT
#define DRAGONFLY_LIMB_FRONT_RIGHT_FOOT 21
#endif
#ifndef DRAGONFLY_LIMB_FRONT_RIGHT_LOWER_LEG
#define DRAGONFLY_LIMB_FRONT_RIGHT_LOWER_LEG 20
#endif
#ifndef DRAGONFLY_LIMB_FRONT_RIGHT_UPPER_LEG
#define DRAGONFLY_LIMB_FRONT_RIGHT_UPPER_LEG 19
#endif
#ifndef DRAGONFLY_LIMB_HEAD
#define DRAGONFLY_LIMB_HEAD 22
#endif
#ifndef DRAGONFLY_LIMB_JAW
#define DRAGONFLY_LIMB_JAW 23
#endif
#ifndef DRAGONFLY_LIMB_LEFT_WING
#define DRAGONFLY_LIMB_LEFT_WING 8
#endif
#ifndef DRAGONFLY_LIMB_MAX
#define DRAGONFLY_LIMB_MAX 24
#endif
#ifndef DRAGONFLY_LIMB_NONE
#define DRAGONFLY_LIMB_NONE 0
#endif
#ifndef DRAGONFLY_LIMB_RIGHT_WING
#define DRAGONFLY_LIMB_RIGHT_WING 9
#endif
#ifndef DRAGONFLY_LIMB_ROOT
#define DRAGONFLY_LIMB_ROOT 1
#endif
#ifndef DRAGONFLY_LIMB_TAIL_SEGMENT_1
#define DRAGONFLY_LIMB_TAIL_SEGMENT_1 2
#endif
#ifndef DRAGONFLY_LIMB_TAIL_SEGMENT_2
#define DRAGONFLY_LIMB_TAIL_SEGMENT_2 3
#endif
#ifndef DRAGONFLY_LIMB_TAIL_SEGMENT_3
#define DRAGONFLY_LIMB_TAIL_SEGMENT_3 4
#endif
#ifndef DRAGONFLY_LIMB_TAIL_SEGMENT_4
#define DRAGONFLY_LIMB_TAIL_SEGMENT_4 5
#endif
#ifndef DRAGONFLY_LIMB_TAIL_TIP
#define DRAGONFLY_LIMB_TAIL_TIP 6
#endif
#ifndef DRAGONFLY_LIMB_THORAX
#define DRAGONFLY_LIMB_THORAX 7
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_DragonflyLimb
#define COMBO_ENEMY_ASSET_ENUM_DragonflyLimb
typedef s32 DragonflyLimb;
#endif
#define DRAGONFLY_GET_TYPE(thisx) ((thisx)->params)

struct EnGrasshopper;

typedef void (*EnGrasshopperActionFunc)(struct EnGrasshopper*, PlayState*);

typedef enum EnGrasshopperType {
    DRAGONFLY_TYPE_UNUSED_NORMAL = -1,
    DRAGONFLY_TYPE_NORMAL,
    DRAGONFLY_TYPE_GROWS_WHEN_SPAWNED,
    DRAGONFLY_TYPE_WOODFALL_TEMPLE_FINAL_ROOM,
    DRAGONFLY_TYPE_WOODFALL
} EnGrasshopperType;

typedef struct EnGrasshopperEffect {
    u8 isEnabled;
    Vec3f pos;
    Vec3f velocity;
    f32 yaw;
    Vec3f scale;
    s16 timer;
    s16 lightningIndex;
} EnGrasshopperEffect;

#define DRAGONFLY_EFFECT_COUNT 100

typedef enum EnGrasshopperBodyPart {
    DRAGONFLY_BODYPART_0,
    DRAGONFLY_BODYPART_1,
    DRAGONFLY_BODYPART_2,
    DRAGONFLY_BODYPART_3,
    DRAGONFLY_BODYPART_4,
    DRAGONFLY_BODYPART_5,
    DRAGONFLY_BODYPART_6,
    DRAGONFLY_BODYPART_7,
    DRAGONFLY_BODYPART_8,
    DRAGONFLY_BODYPART_9,
    DRAGONFLY_BODYPART_10,
    DRAGONFLY_BODYPART_11,
    DRAGONFLY_BODYPART_MAX
} EnGrasshopperBodyPart;

typedef enum EnGrasshopperShadowBodyPart {
    DRAGONFLY_SHADOW_BODYPART_0,
    DRAGONFLY_SHADOW_BODYPART_1,
    DRAGONFLY_SHADOW_BODYPART_2,
    DRAGONFLY_SHADOW_BODYPART_3,
    DRAGONFLY_SHADOW_BODYPART_4,
    DRAGONFLY_SHADOW_BODYPART_5,
    DRAGONFLY_SHADOW_BODYPART_6,
    DRAGONFLY_SHADOW_BODYPART_7,
    DRAGONFLY_SHADOW_BODYPART_8,
    DRAGONFLY_SHADOW_BODYPART_9,
    DRAGONFLY_SHADOW_BODYPART_10,
    DRAGONFLY_SHADOW_BODYPART_11,
    DRAGONFLY_SHADOW_BODYPART_12,
    DRAGONFLY_SHADOW_BODYPART_13,
    DRAGONFLY_SHADOW_BODYPART_MAX
} EnGrasshopperShadowBodyPart;

typedef struct EnGrasshopper {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[DRAGONFLY_LIMB_MAX];
    Vec3s morphTable[DRAGONFLY_LIMB_MAX];
    EnGrasshopperActionFunc actionFunc;
    u8 decision;
    u8 shouldTurn;
    s8 unk_2AE[2];
    s16 timer;
    s16 waitTimer;
    s16 postBankTimer;
    s16 drawDmgEffTimer;
    s16 drawDmgEffType;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f bodyPartsPos[DRAGONFLY_BODYPART_MAX];
    s16 bodyPartIndex;
    s16 splashCount;
    s16 action;
    s16 index;
    s16 bankState;
    s16 type;
    f32 dragonflyScale;
    f32 angularVelocity;
    f32 animEndFrame;
    f32 approachSpeed;
    f32 baseFlyHeight;
    f32 targetPosY;
    Vec3s targetRot;
    Vec3f flyingHomePos;
    s16 bobPhase;
    s8 unk_38E[0x26];
    Vec3f targetApproachPos;
    Vec3f tailTipPos;
    Vec3f effectBasePos;
    Vec3f shadowBodyPartsPos[DRAGONFLY_LIMB_MAX];
    f32 waterSurface;
    Vec3f damagedVelocity;
    Vec3s targetBankRot;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[2];
    EnGrasshopperEffect effects[DRAGONFLY_EFFECT_COUNT];
} EnGrasshopper;

#endif
