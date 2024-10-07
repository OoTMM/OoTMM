#ifndef OVL_EN_BUTTE_H
#define OVL_EN_BUTTE_H

#include <combo.h>

typedef struct Actor_EnButte Actor_EnButte;

typedef void (*Actor_EnButteFunc)(Actor_EnButte*, GameState_Play*);

#define BUTTERFLY_GET(thisx) ((thisx)->params)
#define BUTTERFLY_GET_1(thisx) ((thisx)->params & 1)

#define BUTTERFLY_MINUS1    -1
#define BUTTERFLY_0         0
#define BUTTERFLY_1         1

#define BUTTERFLY_PARAMS(param) (param)

struct Actor_EnButte
{
    Actor                   actor;
    ColliderJntSph          collider;
    ColliderJntSphElement   colldierElements[1];
    SkelAnime               skelAnime;
    Vec3s                   jointTable[8];
    Vec3s                   morphTable[8];
    Actor_EnButteFunc       actionFunc;
    s16                     unk_24C;
    u8                      unk_24E;
    u8                      unk_24F;
    u8                      unk_250;
    s16                     unk_252;
    s16                     unk_254;
    s16                     unk_256;
    s16                     unk_258;
    f32                     unk_25C;
};

#endif // Z_EN_BUTTE_H
