#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_KAME_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_KAME_H

#include <combo.h>

#ifndef SNAPPER_LIMB_BACK_LEFT_FOOT
#define SNAPPER_LIMB_BACK_LEFT_FOOT 10
#endif
#ifndef SNAPPER_LIMB_BACK_LEFT_LEG
#define SNAPPER_LIMB_BACK_LEFT_LEG 9
#endif
#ifndef SNAPPER_LIMB_BACK_RIGHT_FOOT
#define SNAPPER_LIMB_BACK_RIGHT_FOOT 12
#endif
#ifndef SNAPPER_LIMB_BACK_RIGHT_LEG
#define SNAPPER_LIMB_BACK_RIGHT_LEG 11
#endif
#ifndef SNAPPER_LIMB_BODY
#define SNAPPER_LIMB_BODY 1
#endif
#ifndef SNAPPER_LIMB_EYES
#define SNAPPER_LIMB_EYES 4
#endif
#ifndef SNAPPER_LIMB_FRONT_LEFT_FOOT
#define SNAPPER_LIMB_FRONT_LEFT_FOOT 6
#endif
#ifndef SNAPPER_LIMB_FRONT_LEFT_LEG
#define SNAPPER_LIMB_FRONT_LEFT_LEG 5
#endif
#ifndef SNAPPER_LIMB_FRONT_RIGHT_FOOT
#define SNAPPER_LIMB_FRONT_RIGHT_FOOT 8
#endif
#ifndef SNAPPER_LIMB_FRONT_RIGHT_LEG
#define SNAPPER_LIMB_FRONT_RIGHT_LEG 7
#endif
#ifndef SNAPPER_LIMB_HEAD
#define SNAPPER_LIMB_HEAD 2
#endif
#ifndef SNAPPER_LIMB_JAW
#define SNAPPER_LIMB_JAW 3
#endif
#ifndef SNAPPER_LIMB_MAX
#define SNAPPER_LIMB_MAX 13
#endif
#ifndef SNAPPER_LIMB_NONE
#define SNAPPER_LIMB_NONE 0
#endif
#ifndef SPIKED_SNAPPER_LIMB_BODY
#define SPIKED_SNAPPER_LIMB_BODY 1
#endif
#ifndef SPIKED_SNAPPER_LIMB_MAX
#define SPIKED_SNAPPER_LIMB_MAX 4
#endif
#ifndef SPIKED_SNAPPER_LIMB_NONE
#define SPIKED_SNAPPER_LIMB_NONE 0
#endif
#ifndef SPIKED_SNAPPER_LIMB_SHELL
#define SPIKED_SNAPPER_LIMB_SHELL 2
#endif
#ifndef SPIKED_SNAPPER_LIMB_SPIKES
#define SPIKED_SNAPPER_LIMB_SPIKES 3
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_SnapperLimb
#define COMBO_ENEMY_ASSET_ENUM_SnapperLimb
typedef s32 SnapperLimb;
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_SpikedSnapperLimb
#define COMBO_ENEMY_ASSET_ENUM_SpikedSnapperLimb
typedef s32 SpikedSnapperLimb;
#endif
struct EnKame;

typedef void (*EnKameActionFunc)(struct EnKame*, PlayState*);

typedef enum SnapperBodyPart {
    SNAPPER_BODYPART_JAW,
    SNAPPER_BODYPART_FRONT_LEFT_FOOT,
    SNAPPER_BODYPART_FRONT_RIGHT_FOOT,
    SNAPPER_BODYPART_BACK_LEFT_FOOT,
    SNAPPER_BODYPART_BACK_RIGHT_FOOT,
    SNAPPER_BODYPART_BODY_1,
    SNAPPER_BODYPART_BODY_2,
    SNAPPER_BODYPART_BODY_3,
    SNAPPER_BODYPART_BODY_4,
    SNAPPER_BODYPART_BODY_5,
    SNAPPER_BODYPART_MAX
} SnapperBodyPart;

typedef struct EnKame {
    Actor actor;
    SkelAnime snapperSkelAnime;
    Vec3s snapperJointTable[SNAPPER_LIMB_MAX];
    Vec3s snapperMorphTable[SNAPPER_LIMB_MAX];
    SkelAnime spikedSnapperSkelAnime;
    Vec3s spikedSnapperJointTable[SPIKED_SNAPPER_LIMB_MAX];
    Vec3s spikedSnapperMorphTable[SPIKED_SNAPPER_LIMB_MAX];
    EnKameActionFunc actionFunc;
    u8 eyeIndex;
    u8 drawDmgEffType;
    union {
                    s16 timer;
                    s16 targetAngularVelocityY;
                    s16 flipType;
                };
    s16 voiceTimer;
    s16 stunTimer;
    s16 targetRotY;
    s16 angularVelocityY;
    f32 limbScaleXZ;
    union {
                    f32 limbScaleY;
                    f32 spikesScale;
                };
    f32 drawDmgEffAlpha;
    f32 drawDmgEffScale;
    f32 drawDmgEffFrozenSteamScale;
    Vec3f targetPos;
    Vec3f bodyPartsPos[SNAPPER_BODYPART_MAX];
    ColliderCylinder collider;
} EnKame;

#endif
