#ifndef COMBO_MATH_H
#define COMBO_MATH_H

#include <combo/types.h>
#include <combo/math/vec.h>

#define fabsf(f) __builtin_fabsf(f)

#define BINANG_SUB(a, b) ((s16)(a - b))
#define BINANG_ADD(a, b) ((s16)(a + b))
#define BINANG_ROT180(angle) ((s16)(angle + 0x8000))

#define SQXZ(vec) ((vec.x) * (vec.x) + (vec.z) * (vec.z))
#define DOTXYZ(vec1, vec2) ((vec1.x) * (vec2.x) + (vec1.y) * (vec2.y) + (vec1.z) * (vec2.z))

#define LERPIMP(v0, v1, t) ((v0) + (((v1) - (v0)) * (t)))
#define LERPIMP_ALT(v0, v1, t) (((v1) - (v0)) * (t) + (v0))
#define S16_LERP(v0, v1, t) ((s16)(((v1) - (v0)) * (t)) + (v0))
#define F32_LERP(v0, v1, t) ((1.0f - (t)) * (f32)(v0) + (t) * (f32)(v1))
#define F32_LERP_ALT(v0, v1, t) ((f32)(v0) * (1.0f - (t)) + (t) * (f32)(v1))
#define F32_LERPIMP(v0, v1, t) ((f32)(v0) + (((f32)(v1) - (f32)(v0)) * (t)))
#define F32_LERPIMPINV(v0, v1, t) ((f32)(v0) + (((f32)(v1) - (f32)(v0)) / (t)))
#define BINANG_LERPIMP(v0, v1, t) ((v0) + (s16)(BINANG_SUB((v1), (v0)) * (t)))
#define BINANG_LERPIMPINV(v0, v1, t) ((v0) + BINANG_SUB((v1), (v0)) / (t))

#define LERPWEIGHT(val, prev, next) (((val) - (prev)) / ((next) - (prev)))
#define F32_LERPWEIGHT(val, prev, next) (((f32)(val) - (f32)(prev)) / ((f32)(next) - (f32)(prev)))

#define IS_ZERO(f) (fabsf(f) < 0.008f)

#define M_PIf 3.14159265358979323846f
#define M_SQRT2 1.41421356237309504880f
#define MAXFLOAT 3.40282347e+38f
#define SHT_MAX 32767.0f
#define FLT_MAX 3.40282347e+38f
#define SHT_MINV (1.0f / SHT_MAX)

#define VEC3F_LERPIMPDST(dst, v0, v1, t)                \
    {                                                   \
        (dst)->x = (v0)->x + (((v1)->x - (v0)->x) * t); \
        (dst)->y = (v0)->y + (((v1)->y - (v0)->y) * t); \
        (dst)->z = (v0)->z + (((v1)->z - (v0)->z) * t); \
    }                                                   \
    (void)0

float sqrtf(float value);

f32 cosf(f32 __x);
f32 sinf(f32 __x);

void    Math_ApproachF(f32* pValue, f32 target, f32 scale, f32 maxStep);
void    Math_ApproachZeroF(f32* pValue, f32 scale, f32 maxStep);
void    Math_ApproachS(s16 *pValue, s16 target, s16 scale, s16 maxStep);
float   Math_CosS(s16 angle);
float   Math_SinS(s16 angle);
s16     Math_Atan2S(f32 y, f32 x);
f32     Math_Atan2F(f32 y, f32 x);
#define Math_FAtan2F Math_Atan2F
s16     Math_Atan2S_XY(f32 x, f32 y);
f32     Math_Atan2F_XY(f32 x, f32 y);
s32     Math_StepToF(float* pValue, float target, float step);
s32     Math_StepToS(s16* pValue, s16 target, s16 step);
float   Math_SmoothStepToF(float* pValue, float target, float fraction, float step, float minStep);
s16     Math_SmoothStepToS(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep);
void    Math_Vec3f_Copy(Vec3f* dest, Vec3f* src);
void    Math_Vec3s_Copy(Vec3s* dest, Vec3s* src);
void    Math_Vec3f_Sum(Vec3f* a, Vec3f* b, Vec3f* dest);
void    Math_Vec3f_Diff(Vec3f* a, Vec3f* b, Vec3f* dest);
s32     Math_ScaledStepToS(s16* pValue, s16 target, s16 step);
float   Math3D_Vec3fDistSq(const Vec3f* a, const Vec3f* b);
s32     Math3D_CosOut(Vec3f* a, Vec3f* b, f32* dst);
float   Math3D_Dist1DSq(float a, float b);
float   Math3D_Dist2DSq(float x1, float y1, float x2, float y2);
s16     Math_Vec3f_Yaw(Vec3f* origin, Vec3f* point);
s16     Math_Vec3f_Pitch(Vec3f* origin, Vec3f* point);
f32     Math_FAcosF(f32 angle);
f32     Math_SinF(f32 rad);
f32     Math_Vec3f_DistXZ(Vec3f* a, Vec3f* b);
s32     Math3D_LineSegVsPlane(f32 nx, f32 ny, f32 nz, f32 originDist, Vec3f* linePointA, Vec3f* linePointB, Vec3f* intersect, s32 fromFront);


#endif
