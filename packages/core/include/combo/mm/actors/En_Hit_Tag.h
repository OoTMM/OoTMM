#ifndef COMBO_MM_ACTORS_EN_HIT_TAG_H
#define COMBO_MM_ACTORS_EN_HIT_TAG_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_EnHitTag Actor_EnHitTag;

typedef void (*EnHitTagFunc)(Actor_EnHitTag*, GameState_Play*);

struct ALIGNED(4) Actor_EnHitTag
{
    Actor                   base;
    ColliderCylinder        collider;
    EnHitTagFunc            handler;
    Xflag                   xflag;
    u8                      isExtended:1;
};

ASSERT_SIZE(Actor_EnHitTag, 0x19c);

#endif
