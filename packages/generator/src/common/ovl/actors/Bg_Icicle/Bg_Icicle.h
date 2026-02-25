#ifndef COMBO_BG_ICICLE_H
#define COMBO_BG_ICICLE_H

#include <combo.h>

struct Actor_BgIcicle;

typedef void (*BgIceTuraraActionFunc)(struct Actor_BgIcicle*, struct PlayState*);

typedef enum BgIceTuraraType {
    TURARA_STALAGMITE,
    ICICLE_STALAGMITE_RANDOM_DROP = TURARA_STALAGMITE,
    TURARA_STALACTITE,
    ICICLE_STALACTITE = TURARA_STALACTITE,
    TURARA_STALACTITE_REGROW,
    ICICLE_STALACTITE_REGROW = TURARA_STALACTITE_REGROW,
    ICICLE_STALAGMITE_FIXED_DROP,
} BgIceTuraraType;

typedef struct Actor_BgIcicle {
    DynaPolyActor dyna;
    BgIceTuraraActionFunc actionFunc;
#if defined(GAME_MM)
    u8 unk_160;
    u8 unk_161;
#endif
    s16 shiverTimer;
    ColliderCylinder collider;

    Xflag xflag;
    u8 isExtended;
} Actor_BgIcicle;

#endif
