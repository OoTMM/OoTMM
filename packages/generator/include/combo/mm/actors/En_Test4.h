#ifndef INCLUDED_COMBO_MM_ACTORS_EN_TEST4_H
#define INCLUDED_COMBO_MM_ACTORS_EN_TEST4_H

#include <combo/actor.h>

typedef struct Actor_EnTest4 Actor_EnTest4;

struct Actor_EnTest4
{
    Actor   base;
    u8      daytimeIndex;
    u16     prevTime;
    char    unk_148[8];
    void*   handler;
};

ASSERT_SIZE(Actor_EnTest4, 0x154);
ASSERT_OFFSET(Actor_EnTest4, base, 0x000);
ASSERT_OFFSET(Actor_EnTest4, daytimeIndex, 0x144);
ASSERT_OFFSET(Actor_EnTest4, prevTime, 0x146);
ASSERT_OFFSET(Actor_EnTest4, unk_148, 0x148);
ASSERT_OFFSET(Actor_EnTest4, handler, 0x150);

#endif
