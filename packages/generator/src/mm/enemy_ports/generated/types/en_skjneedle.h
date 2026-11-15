#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_SKJNEEDLE_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_SKJNEEDLE_H

#include <combo.h>

struct EnSkjneedle;

typedef struct EnSkjneedle {
    Actor actor;
    char unk_14C[0x48];
    ColliderCylinder collider;
    s16 unusedTimer1;
    s16 killTimer;
    char unk_1E4[4];
} EnSkjneedle;

#endif
