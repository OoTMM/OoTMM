#ifndef COMBO_OVL_EN_KUSA_H
#define COMBO_OVL_EN_KUSA_H

#include <combo.h>

struct EnKusa;

typedef void (*EnKusaActionFunc)(struct EnKusa*, PlayState*);

typedef enum EnKusaType {
    /* 0 */ ENKUSA_TYPE_0,
    /* 1 */ ENKUSA_TYPE_1,
    /* 2 */ ENKUSA_TYPE_2
} EnKusaType;

typedef struct EnKusa {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnKusaActionFunc actionFunc;
    /* 0x0150 */ ColliderCylinder collider;
    /* 0x019C */ s16 timer;
    /* 0x019E */ s8 requiredObjectSlot;

    Xflag xflag;
} EnKusa; // size = 0x01A0

#endif
