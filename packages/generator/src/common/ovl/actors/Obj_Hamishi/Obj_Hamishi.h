#ifndef Z_OBJ_HAMISHI_H
#define Z_OBJ_HAMISHI_H

#include <combo.h>

struct Actor_ObjHamishi;

typedef struct Actor_ObjHamishi {
    Actor actor;
    ColliderCylinder collider;
    f32 shakePosSize;
    f32 shakeRotSize;
    s16 shakeFrames;
    s16 shakePosPhase;
    s16 shakeRotPhase;
    s16 hitCount;
#if defined(GAME_MM)
    s8 unk_1A0;
    s8 unk_1A1;
    u8 unk_1A2;
#endif

    Xflag  xflag;
    u8     isExtended;
} Actor_ObjHamishi;

#endif
