#ifndef OVL_BG_ICEFLOE_H
#define OVL_BG_ICEFLOE_H

#include <combo.h>

struct BgIcefloe;

typedef void (*BgIcefloeActionFunc)(struct BgIcefloe*, PlayState*);

typedef struct BgIcefloe {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgIcefloeActionFunc actionFunc;
    /* 0x160 */ s32 timer;
} BgIcefloe; // size = 0x164

#endif
