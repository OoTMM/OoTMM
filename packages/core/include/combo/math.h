#ifndef COMBO_MATH_H
#define COMBO_MATH_H

#include <combo/types.h>
#include <combo/math/vec.h>

#define fabsf(f) __builtin_fabsf(f)

#define SQXZ(vec) ((vec.x) * (vec.x) + (vec.z) * (vec.z))

float sqrtf(float value);

float   Math_CosS(s16 angle);
float   Math_SinS(s16 angle);
s32     Math_StepToF(float* pValue, float target, float step);
s32     Math_StepToS(s16* pValue, s16 target, s16 step);
float   Math_SmoothStepToF(float* pValue, float target, float fraction, float step, float minStep);
void    Math_Vec3f_Copy(Vec3f* dest, Vec3f* src);
void    Math_Vec3f_Sum(Vec3f* a, Vec3f* b, Vec3f* dest);
void    Math_Vec3f_Diff(Vec3f* a, Vec3f* b, Vec3f* dest);

#endif
