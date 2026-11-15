#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_BG_HIDAN_CURTAIN_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_BG_HIDAN_CURTAIN_H

#include <combo.h>

struct BgHidanCurtain;

typedef void (*BgHidanCurtainActionFunc)(struct BgHidanCurtain*, struct PlayState*);

typedef struct BgHidanCurtain {
    Actor actor;
    BgHidanCurtainActionFunc actionFunc;
    u8 type;
    u8 treasureFlag;
    u8 size;
    u8 alpha;
    s16 timer;
    s16 texScroll;
    ColliderCylinder collider;
} BgHidanCurtain;

#endif
