#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_TAG_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_ANUBICE_TAG_H

#include <combo.h>

struct EnAnubiceTag;

typedef void (*EnAnubiceTagActionFunc)(struct EnAnubiceTag*, struct PlayState*);

typedef struct EnAnubiceTag {
    Actor actor;
    EnAnubiceTagActionFunc actionFunc;
    struct EnAnubice* anubis;
    f32 extraTriggerRange;
} EnAnubiceTag;

#endif
