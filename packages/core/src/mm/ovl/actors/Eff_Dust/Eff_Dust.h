#ifndef OVL_EFF_DUST
#define OVL_EFF_DUST

#include <combo.h>

struct EffDust;

typedef void (*EffDustActionFunc)(struct EffDust*, PlayState*);

typedef struct EffDust {
    Actor actor;
    f32 distanceTraveled[64];
    Vec3f initialPositions[64];
    u8 index;
    u8 life;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 scalingFactor;
    EffDustActionFunc actionFunc;
    EffDustActionFunc drawFunc;
} EffDust; // size = 0x560

typedef enum {
    EFF_DUST_TYPE_0,
    EFF_DUST_TYPE_1,
    EFF_DUST_TYPE_SPIN_ATTACK_CHARGE,
    EFF_DUST_TYPE_3,
    EFF_DUST_TYPE_4,
    EFF_DUST_TYPE_5
} EffDustType;

#endif
