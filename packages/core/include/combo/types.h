#ifndef COMBO_TYPES_H
#define COMBO_TYPES_H

#include <ultra64.h>

#define NORETURN    __attribute__((noreturn))
#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define UNREACHABLE __builtin_unreachable
#define UNUSED(x)   ((void)x)

#define PALIGN(val, align)  ((void*)ALIGN(((u32)(val)), align))

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

typedef struct PACKED
{
    s16 x;
    s16 y;
    s16 z;
}
Vec3s;

typedef struct PACKED
{
    s32 x;
    s32 y;
    s32 z;
}
Vec3i;

typedef struct PACKED
{
    float x;
    float y;
    float z;
}
Vec3f;

#endif /* TYPES_H */
