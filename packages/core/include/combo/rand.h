#ifndef COMBO_RAND_H
#define COMBO_RAND_H

#include <combo/types.h>

u32     Rand_Next(void);
void    Rand_Seed(u32 seed);
float   Rand_ZeroOne(void);
f32     Rand_ZeroFloat(f32 scale);
float   Rand_Centered(void);
void    Rand_Seed_Variable(u32* rndNum, u32 seed);
u32     Rand_Next_Variable(u32* rndNum);
float   Rand_ZeroOne_Variable(u32* rndNum);
float   Rand_Centered_Variable(u32* rndNum);
s16     Rand_S16Offset(s16 base, s16 range);
s16     Rand_S16OffsetStride(s16 base, s16 stride, s16 range);

#endif
