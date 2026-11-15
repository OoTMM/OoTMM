#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_H

#include <combo.h>

#ifndef ANUBICE_LIMB_ABDOMEN_ROOT
#define ANUBICE_LIMB_ABDOMEN_ROOT 4
#endif
#ifndef ANUBICE_LIMB_BODY_ROOT
#define ANUBICE_LIMB_BODY_ROOT 2
#endif
#ifndef ANUBICE_LIMB_CHEST
#define ANUBICE_LIMB_CHEST 3
#endif
#ifndef ANUBICE_LIMB_HEAD
#define ANUBICE_LIMB_HEAD 13
#endif
#ifndef ANUBICE_LIMB_HEAD_ROOT
#define ANUBICE_LIMB_HEAD_ROOT 12
#endif
#ifndef ANUBICE_LIMB_JAW
#define ANUBICE_LIMB_JAW 15
#endif
#ifndef ANUBICE_LIMB_JAW_ROOT
#define ANUBICE_LIMB_JAW_ROOT 14
#endif
#ifndef ANUBICE_LIMB_JEWEL
#define ANUBICE_LIMB_JEWEL 11
#endif
#ifndef ANUBICE_LIMB_JEWEL_ROOT
#define ANUBICE_LIMB_JEWEL_ROOT 10
#endif
#ifndef ANUBICE_LIMB_LOWER_ABDOMEN
#define ANUBICE_LIMB_LOWER_ABDOMEN 6
#endif
#ifndef ANUBICE_LIMB_MAX
#define ANUBICE_LIMB_MAX 16
#endif
#ifndef ANUBICE_LIMB_NONE
#define ANUBICE_LIMB_NONE 0
#endif
#ifndef ANUBICE_LIMB_ROOT
#define ANUBICE_LIMB_ROOT 1
#endif
#ifndef ANUBICE_LIMB_TAIL_BASE
#define ANUBICE_LIMB_TAIL_BASE 8
#endif
#ifndef ANUBICE_LIMB_TAIL_ROOT
#define ANUBICE_LIMB_TAIL_ROOT 7
#endif
#ifndef ANUBICE_LIMB_TAIL_TIP
#define ANUBICE_LIMB_TAIL_TIP 9
#endif
#ifndef ANUBICE_LIMB_UPPER_ABDOMEN
#define ANUBICE_LIMB_UPPER_ABDOMEN 5
#endif
#ifndef COMBO_ENEMY_ASSET_ENUM_AnubiceLimb
#define COMBO_ENEMY_ASSET_ENUM_AnubiceLimb
typedef s32 AnubiceLimb;
#endif
#include "bg_hidan_curtain.h"

struct EnAnubice;

typedef void (*EnAnubiceActionFunc)(struct EnAnubice*, struct PlayState*);

typedef struct EnAnubice {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[ANUBICE_LIMB_MAX];
    Vec3s morphTable[ANUBICE_LIMB_MAX];
    EnAnubiceActionFunc actionFunc;
    s16 timeAlive;
    s16 isNearWall;
    s16 fallTargetYaw;
    s16 deathTimer;
    s16 knockbackTimer;
    s16 isMirroringPlayer;
    s16 isPlayerOutOfRange;
    s16 isKnockedback;
    s16 hasSearchedForFlameCircles;
    f32 hoverVelocityTimer;
    f32 animLastFrame;
    f32 targetHeight;
    f32 playerHeightOffset;
    f32 headPitch;
    f32 focusHeightOffset;
    Vec3f headPos;
    Vec3f fireballRot;
    Vec3f home;
    Vec3f knockbackRecoveryVelocity;
    struct BgHidanCurtain* flameCircles[5];
    char unk_2C4[0x4];
    ColliderCylinder collider;
} EnAnubice;

#endif
