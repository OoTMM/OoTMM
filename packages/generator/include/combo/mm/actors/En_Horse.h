#ifndef COMBO_MM_EN_HORSE_H
#define COMBO_MM_EN_HORSE_H

#include <combo/actor.h>

#define HORSE_TYPE_EPONA 0
#define HORSE_TYPE_HNI 1
#define HORSE_TYPE_2 2
#define HORSE_TYPE_BANDIT 3
#define HORSE_TYPE_DONKEY 4
#define HORSE_TYPE_MAX 5

typedef struct EnHorse 
{
    /* 0x000 */ Actor actor;
    /* 0x144 */ u32 action;
    /* 0x148 */ s32 noInputTimer;
    /* 0x14C */ s32 noInputTimerMax;
    /* 0x150 */ s32 type;
    char unk_154[0x440];
} EnHorse; // size = 0x594

_Static_assert(sizeof(EnHorse) == 0x594, "MM Actor_EnHorse size is wrong");

#endif