#ifndef INCLUDED_COMBO_TIME_H
#define INCLUDED_COMBO_TIME_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;

u32     Time_Game2Linear(u8 day, u16 time);
void    Time_Linear2Game(u8* day, u16* time, u32 linear);
u32     Time_LinearMoonCrash(void);
u32     Time_LinearNewCycle(void);
int     Time_IsMoonCrashLinear(u32 time);
int     Time_IsMoonCrash(u8 day, u16 time);
u32     Time_FromHalfDay(u8 halfDays);

#endif
