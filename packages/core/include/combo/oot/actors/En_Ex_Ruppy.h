#ifndef COMBO_OOT_EN_EX_RUPPY_H
#define COMBO_OOT_EN_EX_RUPPY_H

#include <combo/actor.h>

typedef struct Actor_EnExRuppy Actor_EnExRuppy;

typedef void (*EnExRuppyActionFunc)(Actor_EnExRuppy*, GameState_Play*);

struct Actor_EnExRuppy
{
    /* 0x0000 */ Actor actor;
    /* 0x013C */ EnExRuppyActionFunc actionFunc;
    /* 0x0140 */ s16 colorIdx;
    /* 0x0142 */ s16 type;
    /* 0x0144 */ s16 invisible;
    /* 0x0146 */ s16 timer;
    /* 0x0148 */ s16 rupeeValue;
    /* 0x014A */ s16 unk_15A;
    /* 0x014C */ s16 galleryFlag;
    /* 0x0150 */ f32 unk_160;
} ;

_Static_assert(sizeof(Actor_EnExRuppy) == 0x0154, "OoT Actor_EnExRuppy size is wrong");

#endif
