#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DODOJR_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DODOJR_H

#include <combo.h>

struct EnDodojr;

typedef void (*EnDodojrActionFunc)(struct EnDodojr*, struct PlayState*);

typedef struct EnDodojr {
    Actor actor;
    SkelAnime skelAnime;
    EnDodojrActionFunc actionFunc;
    ColliderCylinder collider;
    Actor* bomb;
    Vec3f headPos;
    Vec3f dustPos;
    s16 counter;
    s16 stunTimer;
    s16 freezeFrameTimer;
    s16 timer;
    s16 crawlSfxTimer;
    f32 rootScale;
    Vec3s jointTable[15];
    Vec3s morphTable[15];
} EnDodojr;

#endif
