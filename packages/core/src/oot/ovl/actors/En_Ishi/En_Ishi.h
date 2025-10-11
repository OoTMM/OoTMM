#ifndef OVL_EN_ISHI_H
#define OVL_EN_ISHI_H

#include <combo.h>

#define ISHI_GET_SWITCH_FLAG_UPPER(thisx)  ((((thisx)->params) >> (12 - 2)) & (NBITS_TO_MASK(4) << 2))
#define ISHI_GET_SWITCH_FLAG_LOWER(thisx)  PARAMS_GET_U((thisx)->params, 6, 2)
#define ISHI_GET_SWITCH_FLAG(thisx) (ISHI_GET_SWITCH_FLAG_UPPER(thisx) | ISHI_GET_SWITCH_FLAG_LOWER(thisx))

typedef enum EnIshiType {
    /* 0x00 */ ROCK_SMALL,
    /* 0x01 */ ROCK_LARGE
} EnIshiType;

struct EnIshi;

typedef void (*EnIshiActionFunc)(struct EnIshi*, struct PlayState*);
typedef void (*EnIshiEffectSpawnFunc)(struct EnIshi*, struct PlayState*);
typedef void (*EnIshiDrawFunc)(struct EnIshi*, struct PlayState*);

typedef struct EnIshi {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnIshiActionFunc actionFunc;
    /* 0x0150 */ ColliderCylinder collider;
} EnIshi; // size = 0x019C

#endif
