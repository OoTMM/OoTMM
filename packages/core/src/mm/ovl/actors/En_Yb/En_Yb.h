#ifndef COMBO_OVL_EN_YB_H
#define COMBO_OVL_EN_YB_H

#include <combo.h>
#include <combo/actor.h>

struct Actor_EnYb;

typedef void (*Actor_EnYbActionFunc)(struct Actor_EnYb*, GameState_Play*);

typedef enum ObjectYbLimb
{
    YB_LIMB_NONE,
    YB_LIMB_MAIN_ROOT,
    YB_LIMB_PANTS,
    YB_LIMB_LEGS_ROOT,
    YB_LIMB_RIGHT_THIGH,
    YB_LIMB_RIGHT_SHIN,
    YB_LIMB_RIGHT_FOOT,
    YB_LIMB_LEFT_THIGH,
    YB_LIMB_LEFT_SHIN,
    YB_LIMB_LEFT_FOOT,
    YB_LIMB_CHEST_ROOT,
    YB_LIMB_HEAD,
    YB_LIMB_PONYTAIL,
    YB_LIMB_EMPTY_5E88,
    YB_LIMB_LEFT_UPPER_ARM,
    YB_LIMB_LEFT_FOREARM,
    YB_LIMB_LEFT_HAND,
    YB_LIMB_RIGHT_UPPER_ARM,
    YB_LIMB_RIGHT_FOREARM,
    YB_LIMB_RIGHT_HAND,
    YB_LIMB_EMPTY_5EDC,
    YB_LIMB_TORSO,
    YB_LIMB_MAX
} ObjectYbLimb;

typedef struct Actor_EnYb
{
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[YB_LIMB_MAX];
    char unkPadding20C[0x94];
    Vec3s morphTable[YB_LIMB_MAX];
    char unkPadding324[0x94];
    ColliderCylinder collider;
    Vec3f shadowPos;
    u16 playerOcarinaOut;
    s16 animIndex;
    s16 alpha;
    s16 csIdList[2];
    s16 csIdIndex;
    s16 teachingCutsceneTimer;
    Actor_EnYbActionFunc actionFunc;
}
Actor_EnYb;

ASSERT_SIZE(Actor_EnYb, 0x424);

#endif
