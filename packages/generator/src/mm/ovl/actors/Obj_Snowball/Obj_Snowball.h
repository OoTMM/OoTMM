#ifndef OVL_SNOWBALL_H
#define OVL_SNOWBALL_H

#include <combo/actor.h>
#include <combo/xflags.h>

struct Actor_ObjSnowball;

typedef void (*Actor_ObjSnowballActionFunc)(struct Actor_ObjSnowball*, PlayState*);

typedef struct {
    Vec3f unk_00;
    f32 unk_0C;
    f32 unk_10;
    f32 unk_14;
    f32 unk_18;
    Vec3s unk_1C;
    s16 unk_22;
    s16 unk_24;
    s16 unk_26;
    CollisionPoly* unk_28;
    u8 unk_2C;
    u8 unk_2D;
} Actor_ObjSnowballStruct; // size = 0x30

typedef struct Actor_ObjSnowball {
    Actor actor;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    Actor_ObjSnowballActionFunc actionFunc;
    Actor_ObjSnowballStruct unk_1A8[2];
    s8 unk_208;
    s8 unk_209;
    s8 unk_20A;
    s8 unk_20B;
    f32 unk_20C;
    s8 unk_210;
    s8 unk_211;

    Xflag  xflag;
    u8     isExtended;
} Actor_ObjSnowball; // size = 0x214

#endif
