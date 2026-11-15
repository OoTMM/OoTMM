#ifndef COMBO_ENEMY_BODYBREAK_H
#define COMBO_ENEMY_BODYBREAK_H

#include <combo.h>

typedef struct {
    MtxF* matrices;
    Gfx** dLists;
    s16* objectSlots;
    PlayState* play;
    s16 count, capacity;
    s32 val, prevLimbIndex;
} BodyBreak;

typedef struct {
    Actor actor;
    u8 action;
    s16 timer;
    Gfx* displayList;
    f32 rotZ, rotZSpeed;
} EnemyBodyPart;

#endif
