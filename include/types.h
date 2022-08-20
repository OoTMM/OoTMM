#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

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
