#ifndef Z_EN_YUKABYUN_H
#define Z_EN_YUKABYUN_H

#include <combo.h>

struct EnYukabyun;

typedef void (*EnYukabyunActionFunc)(struct EnYukabyun*, struct PlayState*);

typedef struct EnYukabyun {
    Actor actor;
    EnYukabyunActionFunc actionfunc;
    s16 unk_150;
    u8 unk_152;
    ColliderCylinder collider;
} EnYukabyun;

#endif
