#ifndef COMBO_ENEMY_NATIVE_TYPES_MM_EN_MKK_H
#define COMBO_ENEMY_NATIVE_TYPES_MM_EN_MKK_H

#include <combo.h>
#include "native_sphere.h"

#define ENMKK_GET_2(thisx) ((thisx)->params & 2)
#define ENMKK_GET_4(thisx) ((thisx)->params & 4)
#define ENMKK_GET_FF00(thisx) (((thisx)->params >> 8) & 0xFF)

struct EnMkk;

typedef void (*EnMkkActionFunc)(struct EnMkk*, PlayState*);

typedef struct EnMkk {
    Actor actor;
    EnMkkActionFunc actionFunc;
    u8 primColorSelect;
    u8 unk_149;
    u8 alpha;
    u8 unk_14B;
    u8 unk_14C;
    s16 unk_14E;
    s16 unk_150;
    s16 unk_152;
    Vec3f unk_154;
    Vec3f unk_160;
    Vec3f unk_16C;
    f32 unk_178;
    ColliderSphere collider;
} EnMkk;

#endif
