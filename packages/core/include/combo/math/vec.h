#ifndef COMBO_MATH_VEC_H
#define COMBO_MATH_VEC_H

#include <combo/types.h>

typedef struct
{
    s16 x;
    s16 y;
    s16 z;
}
Vec3s;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
}
Vec3i;

typedef struct
{
    float x;
    float y;
    float z;
}
Vec3f;

typedef struct
{
    u16 x;
    u16 y;
    u16 z;
}
Vec3us;

typedef struct
{
    float distance;
    s16 angle;
}
VecPolar;

typedef struct
{
    s16 distance;
    s16 angle;
}
VecPolarS;

/* TODO: Move this */
typedef struct
{
    Vec3s   center;
    s16     radius;
}
Sphere16;

#endif
