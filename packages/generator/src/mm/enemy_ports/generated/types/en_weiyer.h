#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_WEIYER_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_WEIYER_H

#include <combo.h>

struct EnWeiyer;

typedef void (*EnWeiyerActionFunc)(struct EnWeiyer*, struct PlayState*);

typedef struct EnWeiyer {
    Actor actor;
    SkelAnime skelAnime;
    EnWeiyerActionFunc actionFunc;
    s16 timer;
    s16 targetYaw;
    Vec3s jointTable[19];
    Vec3s morphTable[19];
    f32 swimHeight;
    f32 targetSwimHeight;
    ColliderCylinder collider;
} EnWeiyer;

#endif
