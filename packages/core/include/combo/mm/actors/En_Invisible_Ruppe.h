#ifndef COMBO_MM_ACTORS_EN_INVISIBLE_RUPPE_H
#define COMBO_MM_ACTORS_EN_INVISIBLE_RUPPE_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_EnInvisibleRuppe Actor_EnInvisibleRuppe;

typedef void (*EnInvisibleRuppeFunc)(Actor_EnInvisibleRuppe*, GameState_Play*);

struct ALIGNED(4) Actor_EnInvisibleRuppe
{
    Actor                   base;
    ColliderCylinder        collider;
    s16                     switchFlag;
    s16                     unk_192;
    EnInvisibleRuppeFunc    handler;
    Xflag                   xflag;
    u8                      isExtended:1;
};

ASSERT_SIZE(Actor_EnInvisibleRuppe, 0x1a0);
ASSERT_OFFSET(Actor_EnInvisibleRuppe, base,       0x000);
ASSERT_OFFSET(Actor_EnInvisibleRuppe, collider,   0x144);
ASSERT_OFFSET(Actor_EnInvisibleRuppe, switchFlag, 0x190);
ASSERT_OFFSET(Actor_EnInvisibleRuppe, unk_192,    0x192);
ASSERT_OFFSET(Actor_EnInvisibleRuppe, handler,    0x194);

#endif
