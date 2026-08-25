#ifndef COMBO_MM_EN_BAL_H
#define COMBO_MM_EN_BAL_H

#include <combo/actor.h>

#include <assets/mm/objects/object_bal.h>

struct EnBal;

typedef enum TingleLimb {
    /* 0x00 */ TINGLE_LIMB_NONE,
    /* 0x01 */ TINGLE_LIMB_ROOT,
    /* 0x02 */ TINGLE_LIMB_PELVIS,
    /* 0x03 */ TINGLE_LIMB_UPPER_BODY_ROOT,
    /* 0x04 */ TINGLE_LIMB_UPPER_BODY_WRAPPER,
    /* 0x05 */ TINGLE_LIMB_BALLOON_ROOT,
    /* 0x06 */ TINGLE_LIMB_BALLOON,
    /* 0x07 */ TINGLE_LIMB_RIGHT_ARM_ROOT,
    /* 0x08 */ TINGLE_LIMB_RIGHT_ARM_WRAPPER,
    /* 0x09 */ TINGLE_LIMB_RIGHT_UPPER_ARM,
    /* 0x0A */ TINGLE_LIMB_RIGHT_FOREARM,
    /* 0x0B */ TINGLE_LIMB_RIGHT_HAND_ROOT,
    /* 0x0C */ TINGLE_LIMB_RIGHT_HAND,
    /* 0x0D */ TINGLE_LIMB_HELD_PEN,
    /* 0x0E */ TINGLE_LIMB_LEFT_ARM_ROOT,
    /* 0x0F */ TINGLE_LIMB_LEFT_ARM_WRAPPER,
    /* 0x10 */ TINGLE_LIMB_LEFT_UPPER_ARM,
    /* 0x11 */ TINGLE_LIMB_LEFT_FOREARM,
    /* 0x12 */ TINGLE_LIMB_LEFT_HAND_ROOT,
    /* 0x13 */ TINGLE_LIMB_LEFT_HAND,
    /* 0x14 */ TINGLE_LIMB_HELD_MAP,
    /* 0x15 */ TINGLE_LIMB_TORSO,
    /* 0x16 */ TINGLE_LIMB_HEAD,
    /* 0x17 */ TINGLE_LIMB_RIGHT_LEG_ROOT,
    /* 0x18 */ TINGLE_LIMB_RIGHT_THIGH,
    /* 0x19 */ TINGLE_LIMB_RIGHT_LOWER_LEG_ROOT,
    /* 0x1A */ TINGLE_LIMB_RIGHT_SHIN,
    /* 0x1B */ TINGLE_LIMB_RIGHT_FOOT,
    /* 0x1C */ TINGLE_LIMB_LEFT_LEG_ROOT,
    /* 0x1D */ TINGLE_LIMB_LEFT_THIGH,
    /* 0x1E */ TINGLE_LIMB_LEFT_LOWER_LEG_ROOT,
    /* 0x1F */ TINGLE_LIMB_LEFT_SHIN,
    /* 0x20 */ TINGLE_LIMB_LEFT_FOOT,
    /* 0x21 */ TINGLE_LIMB_MAX
} TingleLimb;

typedef void (*EnBalActionFunc)(struct EnBal*, PlayState*);

typedef struct EnBal {
    /* 0x000 */ PictoActor picto;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ EnBalActionFunc actionFunc;
    /* 0x190 */ ColliderJntSph collider;
    /* 0x1B0 */ ColliderJntSphElement colliderElements[1];
    /* 0x1F0 */ Vec3f balloonScale;
    /* 0x1FC */ s32 eyeTexIndex;
    /* 0x200 */ Vec3s jointTable[TINGLE_LIMB_MAX];
    /* 0x2C6 */ Vec3s morphTable[TINGLE_LIMB_MAX];
    /* 0x38C */ Vec3s headRot;
    /* 0x392 */ Vec3s torsoRot;
    /* 0x398 */ s16 eyeTimer;
    /* 0x39A */ s16 forceEyesShut;
    /* 0x39C */ s16 watchTarget;
    /* 0x39E */ s16 skyFloatPhase;
    /* 0x3A0 */ s16 textId;
    /* 0x3A2 */ s8 unk3A2[0x2];
    /* 0x3A4 */ s16 balloonAction;
    /* 0x3A6 */ s16 idleAnimStage;
    /* 0x3A8 */ s16 timer;
    /* 0x3AA */ s8 unk3AA[0x2];
    /* 0x3AC */ s16 purchaseMapId;
    /* 0x3AE */ s16 locationMapId;
    /* 0x3B0 */ s16 inflateEarly;
    /* 0x3B2 */ u8 isTalking;
} EnBal; /* size = 0x3B4 */

#endif
