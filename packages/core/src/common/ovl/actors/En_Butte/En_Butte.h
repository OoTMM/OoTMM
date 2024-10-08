#ifndef OVL_EN_BUTTE_H
#define OVL_EN_BUTTE_H

#include <combo.h>
#include <combo/xflags.h>

typedef struct Actor_EnButte Actor_EnButte;

typedef void (*Actor_EnButteFunc)(Actor_EnButte*, GameState_Play*);

#define BUTTERFLY_GET(thisx) ((thisx)->params)
#define BUTTERFLY_GET_1(thisx) ((thisx)->params & 1)

#define BUTTERFLY_MINUS1    0xffff
#define BUTTERFLY_0         0
#define BUTTERFLY_1         1

#define BUTTERFLY_PARAMS(param) (param)

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
    Xflag                   xflag;
};

#endif // Z_EN_BUTTE_H
