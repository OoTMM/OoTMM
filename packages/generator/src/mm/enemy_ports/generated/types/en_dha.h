#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DHA_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_DHA_H

#include <combo.h>

struct EnDha;

typedef void (*EnDhaActionFunc)(struct EnDha*, struct PlayState*);

typedef struct EnDha {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[4];
    Vec3s morphTable[4];
    u8 unk_1C0;
    EnDhaActionFunc actionFunc;
    s16 actionTimer;
    s16 timer;
    u8 unk_1CC;
    s16 limbAngleX[2];
    s16 limbAngleY;
    Vec3s handAngle;
    Vec3f handPos[2];
    Vec3f armPos;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[5];
} EnDha;

#endif
