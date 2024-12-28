#ifndef MATH_STRUCTS_H
#define MATH_STRUCTS_H

#include <ultra64.h>

#if defined(__MM__)
typedef struct Vec2f {
    f32 x, z;
} Vec2f; // size = 0x08
#else
typedef struct Vec2f {
    f32 x, y;
} Vec2f; // size = 0x08
#endif

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f; // size = 0x0C

typedef struct Vec3us {
    u16 x, y, z;
} Vec3us; // size = 0x06

typedef struct Vec3s {
    s16 x, y, z;
} Vec3s; // size = 0x06

typedef struct Vec3i {
    s32 x, y, z;
} Vec3i; // size = 0x0C

#endif
