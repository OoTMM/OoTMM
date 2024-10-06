#ifndef Z_EN_BUTTE_H
#define Z_EN_BUTTE_H

#include <combo.h>

typedef struct Actor_EnButte Actor_EnButte;

typedef void (*Actor_EnButteFunc)(Actor_EnButte*, GameState_Play*);

struct Actor_EnButte
{
    Actor                   actor;
    ColliderJntSph          collider;
    ColliderJntSphElement   colliderItems[1];
    SkelAnime               skelAnime;
    Vec3s                   jointTable[8];
    Vec3s                   morphTable[8];
    Actor_EnButteFunc       actionFunc;
    s16                     timer;
    u8                      flightParamsIdx;
    u8                      unk_257;
    u8                      drawSkelAnime;
    s16                     swordDownTimer;
    s16                     unk_25C;
    s16                     unk_25E;
    s16                     unk_260;
    f32                     posYTarget;
};

#endif
