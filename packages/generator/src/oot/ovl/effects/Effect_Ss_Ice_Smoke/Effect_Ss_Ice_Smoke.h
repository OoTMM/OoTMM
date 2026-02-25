#ifndef OVL_EFF_SS_ICE_SMOKE_H
#define OVL_EFF_SS_ICE_SMOKE_H

#include <combo.h>

typedef struct EffectSsIceSmokeInitParams {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ s16 scale;
} EffectSsIceSmokeInitParams;

#endif
