#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_EIYER_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_EIYER_H

#include <combo.h>

struct EnEiyer;

typedef void (*EnEiyerActionFunc)(struct EnEiyer*, struct PlayState*);

typedef struct EnEiyer {
    Actor actor;
    SkelAnime skelanime;
    EnEiyerActionFunc actionFunc;
    s16 timer;
    s16 targetYaw;
    Vec3s jointTable[19];
    Vec3s morphTable[19];
    Vec3f basePos;
    ColliderCylinder collider;
} EnEiyer;

#endif
