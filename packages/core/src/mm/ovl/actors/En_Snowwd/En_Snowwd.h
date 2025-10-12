#ifndef OVL_EN_SNOWWD_H
#define OVL_EN_SNOWWD_H

#include <combo.h>
#include <combo/xflags.h>

#define SNOWWD_GET_DROP_TABLE(thisx) (((thisx)->params & 0xF80) >> 7)
#define SNOWWD_DROPPED_COLLECTIBLE(thisx) ((thisx)->home.rot.z)

struct EnSnowwd;

typedef void (*EnSnowwdActionFunc)(struct EnSnowwd*, PlayState*);

typedef struct EnSnowwd {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ s16 timer;
    /* 0x194 */ EnSnowwdActionFunc actionFunc;
    Xflag xflag;
} EnSnowwd; // size = 0x198

#endif
