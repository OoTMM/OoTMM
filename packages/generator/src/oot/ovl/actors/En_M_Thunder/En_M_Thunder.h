#ifndef OVL_EN_M_THUNDER
#define OVL_EN_M_THUNDER

#include <combo.h>

struct EnMThunder;

typedef void (*EnMThunderActionFunc)(struct EnMThunder*, PlayState*);

typedef struct EnMThunder {
    Actor actor;
    ColliderCylinder collider;
    LightNode* lightNode;
    LightInfo lightInfo;
    f32 unk_1AC;
    f32 unk_1B0;
    f32 unk_1B4;
    f32 unk_1B8;
    f32 unk_1BC;
    EnMThunderActionFunc actionFunc;
    u16 unk_1C4;
    u8 unk_1C6;
    u8 unk_1C7;
    u8 unk_1C8;
    u8 unk_1C9;
    u8 unk_1CA;
} EnMThunder;

#endif
