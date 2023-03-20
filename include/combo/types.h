#ifndef COMBO_TYPES_H
#define COMBO_TYPES_H

#include <ultra64.h>

#define NORETURN    __attribute__((noreturn))
#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define UNREACHABLE __builtin_unreachable

#define ALIGN8(val) (((val) + 7) & ~7)
#define ALIGN16(val) (((val) + 0xF) & ~0xF)
#define ALIGN32(val) (((val) + 0x1F) & ~0x1F)
#define ALIGN64(val) (((val) + 0x3F) & ~0x3F)
#define ALIGN256(val) (((val) + 0xFF) & ~0xFF)

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

typedef struct PACKED
{
    s16 x;
    s16 y;
    s16 z;
}
Vector3s;

typedef struct PACKED
{
    float x;
    float y;
    float z;
}
Vector3f;

#endif /* TYPES_H */
