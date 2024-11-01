#ifndef OVL_SNOWBALL2_H
#define OVL_SNOWBALL2_H

#include <combo/actor.h>
#include <combo/xflags.h>

struct Actor_ObjSnowball2;

typedef void (*Actor_ObjSnowball2ActionFunc)(struct Actor_ObjSnowball2*, PlayState*);

typedef struct Actor_ObjSnowball2
{
    Actor actor;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    Actor_ObjSnowball2ActionFunc actionFunc;
    s16 unk_1A8;
    s16 unk_1AA;
    s8 unk_1AC;
    u8 unk_1AD;
    s8 unk_1AE;
    s8 unk_1AF;
    char unk1B0[4];
    Xflag  xflag;
    u8     isExtended;
}
Actor_ObjSnowball2;

#endif
