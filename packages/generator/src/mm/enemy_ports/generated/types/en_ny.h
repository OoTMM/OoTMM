#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_NY_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_NY_H

#include <combo.h>

struct EnNy;

typedef void (*EnNyActionFunc)(struct EnNy*, struct PlayState*);

typedef struct EnNy {
    Actor actor;
    EnNyActionFunc actionFunc;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    char unk_1B0[0x18];
    s16 timer;
    s16 unk_1CA;
    s16 hitPlayer;
    u16 unk_1CE;
    u8 unk_1D0;
    s8 unk_1D1;
    s32 unk_1D4;
    s32 unk_1D8;
    s32 stoneTimer;
    f32 unk_1E0;
    f32 unk_1E4;
    f32 unk_1E8;
    f32 unk_1EC;
    f32 unk_1F0;
    f32 unk_1F4;
    Vec3f unk_1F8[16];
} EnNy;

#endif
