#ifndef OOTMM_PORT_STALFOS_H
#define OOTMM_PORT_STALFOS_H

#include <combo.h>

typedef enum StalfosLimb {
    STALFOS_LIMB_NONE,
    STALFOS_LIMB_ROOT,
    STALFOS_LIMB_UPPERBODY_ROOT,
    STALFOS_LIMB_CORE_LOWER_ROOT,
    STALFOS_LIMB_CORE_UPPER_ROOT,
    STALFOS_LIMB_NECK_ROOT,
    STALFOS_LIMB_HEAD_ROOT,
    STALFOS_LIMB_7,
    STALFOS_LIMB_8,
    STALFOS_LIMB_JAW_ROOT,
    STALFOS_LIMB_JAW,
    STALFOS_LIMB_HEAD,
    STALFOS_LIMB_NECK_UPPER,
    STALFOS_LIMB_NECK_LOWER,
    STALFOS_LIMB_CORE_UPPER,
    STALFOS_LIMB_CHEST,
    STALFOS_LIMB_SHOULDER_R_ROOT,
    STALFOS_LIMB_SHOULDER_ARMOR_R_ROOT,
    STALFOS_LIMB_SHOULDER_ARMOR_R,
    STALFOS_LIMB_SHOULDER_L_ROOT,
    STALFOS_LIMB_SHOULDER_ARMOR_L_ROOT,
    STALFOS_LIMB_SHOULDER_ARMOR_L,
    STALFOS_LIMB_ARM_L_ROOT,
    STALFOS_LIMB_UPPERARM_L_ROOT,
    STALFOS_LIMB_FOREARM_L_ROOT,
    STALFOS_LIMB_HAND_L_ROOT,
    STALFOS_LIMB_HAND_L,
    STALFOS_LIMB_SHIELD,
    STALFOS_LIMB_FOREARM_L,
    STALFOS_LIMB_UPPERARM_L,
    STALFOS_LIMB_ARM_R_ROOT,
    STALFOS_LIMB_UPPERARM_R_ROOT,
    STALFOS_LIMB_FOREARM_R_ROOT,
    STALFOS_LIMB_HAND_R_ROOT,
    STALFOS_LIMB_SWORD,
    STALFOS_LIMB_HAND_R,
    STALFOS_LIMB_FOREARM_R,
    STALFOS_LIMB_UPPERARM_R,
    STALFOS_LIMB_CORE_LOWER,
    STALFOS_LIMB_LOWERBODY_ROOT,
    STALFOS_LIMB_WAIST_ROOT,
    STALFOS_LIMB_LEGS_ROOT,
    STALFOS_LIMB_LEG_L_ROOT,
    STALFOS_LIMB_THIGH_L_ROOT,
    STALFOS_LIMB_LOWERLEG_L_ROOT,
    STALFOS_LIMB_ANKLE_L_ROOT,
    STALFOS_LIMB_ANKLE_L,
    STALFOS_LIMB_FOOT_L_ROOT,
    STALFOS_LIMB_FOOT_L,
    STALFOS_LIMB_LOWERLEG_L,
    STALFOS_LIMB_THIGH_L,
    STALFOS_LIMB_LEG_R_ROOT,
    STALFOS_LIMB_THIGH_R_ROOT,
    STALFOS_LIMB_LOWERLEG_R_ROOT,
    STALFOS_LIMB_ANKLE_R_ROOT,
    STALFOS_LIMB_ANKLE_R,
    STALFOS_LIMB_FOOT_R_ROOT,
    STALFOS_LIMB_FOOT_R,
    STALFOS_LIMB_LOWERLEG_R,
    STALFOS_LIMB_THIGH_R,
    STALFOS_LIMB_WAIST,
    STALFOS_LIMB_MAX
} StalfosLimb;

typedef struct {
    MtxF* matrices;
    Gfx** dLists;
    s16* objectSlots;
    PlayState* play;
    s16 count;
    s16 capacity;
    s32 val;
    s32 prevLimbIndex;
} StalfosBodyBreak;

#define BODYBREAK_OBJECT_SLOT_DEFAULT -1

struct EnTest;

typedef void (*EnTestActionFunc)(struct EnTest*, struct PlayState*);

typedef struct EnTest {
    Actor actor;
    Vec3s bodyPartsPos[10];
    SkelAnime skelAnime;
    Vec3s jointTable[STALFOS_LIMB_MAX];
    Vec3s morphTable[STALFOS_LIMB_MAX];
    SkelAnime upperSkelanime;
    Vec3s upperJointTable[STALFOS_LIMB_MAX];
    Vec3s upperMorphTable[STALFOS_LIMB_MAX];
    u8 unk_7C8;
    EnTestActionFunc actionFunc;
    Vec3s headRot;
    Vec3s headRotOffset;
    u8 unk_7DC;
    char unk_7DD[0x1];
    u8 unk_7DE;
    s16 iceTimer;
    u8 lastDamageReaction;
    s32 unk_7E4;
    s32 timer;
    f32 unk_7EC;
    StalfosBodyBreak bodyBreak;
    s8 swordState;
    s32 effectIndex;
    ColliderCylinder bodyCollider;
    ColliderQuad swordCollider;
    ColliderCylinder shieldCollider;
    u8 initialized;
} EnTest;

typedef enum StalfosType {
    STALFOS_TYPE_INVISIBLE,
    STALFOS_TYPE_1,
    STALFOS_TYPE_2,
    STALFOS_TYPE_CEILING,
    STALFOS_TYPE_4,
    STALFOS_TYPE_5
} StalfosType;

#endif
