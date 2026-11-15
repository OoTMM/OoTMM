#ifndef COMBO_ENEMY_PORT_LIZALFOS_H
#define COMBO_ENEMY_PORT_LIZALFOS_H

#include <combo.h>

enum {
    ZF_LIMB_NONE,
    ZF_LIMB_ROOT_ROOT,
    ZF_LIMB_ROOT,
    ZF_LIMB_UPPER_BODY,
    ZF_LIMB_NECK_ROOT,
    ZF_LIMB_HEAD_ROOT,
    ZF_LIMB_JAW_ROOT_ROOT,
    ZF_LIMB_JAW_ROOT,
    ZF_LIMB_JAW,
    ZF_LIMB_HEAD,
    ZF_LIMB_NECK,
    ZF_LIMB_ARM_ROOT_ROOT,
    ZF_LIMB_ARM_ROOT,
    ZF_LIMB_FOREARM_ROOT,
    ZF_LIMB_RIGHT_HAND_AND_SWORD_ROOT,
    ZF_LIMB_RIGHT_HAND_AND_SWORD,
    ZF_LIMB_FOREARM,
    ZF_LIMB_ARM,
    ZF_LIMB_LEFT_ARM_ROOT_ROOT,
    ZF_LIMB_LEFT_ARM_ROOT,
    ZF_LIMB_LEFT_FOREARM_ROOT,
    ZF_LIMB_LEFT_HAND_ROOT,
    ZF_LIMB_LEFT_HAND,
    ZF_LIMB_LEFT_FOREARM,
    ZF_LIMB_LEFT_ARM,
    ZF_LIMB_ARMOR,
    ZF_LIMB_TAIL_ROOT_ROOT,
    ZF_LIMB_TAIL_ROOT,
    ZF_LIMB_TAIL_END_ROOT,
    ZF_LIMB_TAIL_END,
    ZF_LIMB_TAIL,
    ZF_LIMB_SHEATH_ROOT_ROOT,
    ZF_LIMB_SHEATH_ROOT,
    ZF_LIMB_SHEATH,
    ZF_LIMB_RIGHT_THIGH_ROOT_ROOT,
    ZF_LIMB_RIGHT_THIGH_ROOT,
    ZF_LIMB_RIGHT_SHIN_ROOT,
    ZF_LIMB_RIGHT_FOOT_CLAWS_ROOT,
    ZF_LIMB_RIGHT_FOOT_CLAWS,
    ZF_LIMB_RIGHT_SHIN,
    ZF_LIMB_RIGHT_THIGH,
    ZF_LIMB_LEFT_THIGH_ROOT_ROOT,
    ZF_LIMB_LEFT_THIGH_ROOT,
    ZF_LIMB_LEFT_SHIN_ROOT,
    ZF_LIMB_LEFT_FOOT_CLAWS_ROOT,
    ZF_LIMB_LEFT_FOOT_CLAWS,
    ZF_LIMB_LEFT_SHIN,
    ZF_LIMB_LEFT_THIGH,
    ZF_LIMB_BODY,
    ZF_LIMB_MAX
};
typedef enum EnZfType {
    ENZF_TYPE_DINOLFOS = -2,
    ENZF_TYPE_LIZALFOS_SINGLE,
    ENZF_TYPE_LIZALFOS_PAIRED_MINIBOSS_A,
    ENZF_TYPE_LIZALFOS_PAIRED_MINIBOSS_B
} EnZfType;

struct EnZf;

typedef void (*EnZfActionFunc)(struct EnZf*, struct PlayState*);

typedef struct EnZf {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[ZF_LIMB_MAX];
    Vec3s morphTable[ZF_LIMB_MAX];
    s32 action;
    s32 unk_3E0;
    s32 actionState;
    EnZfActionFunc actionFunc;
    s16 headRot;
    s16 headRotStep;
    s32 timer;
    s16 timer2;
    s16 frozenTimer;
    s16 noFloorAhead;
    s16 isSwordSheathed;
    s16 switchFlag;
    s16 curPlatform;
    s16 homePlatform;
    s16 targetPlatform;
    u8 alpha;
    char pad_405[3];
    f32 dodgeSpeed;
    f32 dodgeVelocityY;
    u8 lastDamageReaction;
    char pad_411[3];
    s32 swordBlureEffectIndex;
    ColliderCylinder bodyCollider;
    ColliderQuad swordCollider;
    Vec3f rightFootPos;
    Vec3f leftFootPos;
    Vec3f bodyPartsPos[9];
} EnZf;

#endif
