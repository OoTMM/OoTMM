#ifndef COMBO_UTIL_H
#define COMBO_UTIL_H

#include <stddef.h>
#include <combo/types.h>

#define ASSERT_OFFSET(s,f,o) _Static_assert(offsetof(s,f) == o, "Offset of " #f " in " #s " is not " #o)

#define BITMAP8_GET(m,b)   ((m)[(b) >> 3] & (1 << ((b) & 0x7)))
#define BITMAP8_SET(m,b)   ((m)[(b) >> 3] |= (1 << ((b) & 0x7)))
#define BITMAP8_CLEAR(m,b) ((m)[(b) >> 3] &= ~(1 << ((b) & 0x7)))

#define BITMAP16_GET(m,b)   ((m)[(b) >> 4] & (1 << ((b) & 0xf)))
#define BITMAP16_SET(m,b)   ((m)[(b) >> 4] |= (1 << ((b) & 0xf)))
#define BITMAP16_CLEAR(m,b) ((m)[(b) >> 4] &= ~(1 << ((b) & 0xf)))

#define BITMAP32_GET(m,b)   ((m)[(b) >> 5] & (1 << ((b) & 0x1f)))
#define BITMAP32_SET(m,b)   ((m)[(b) >> 5] |= (1 << ((b) & 0x1f)))
#define BITMAP32_CLEAR(m,b) ((m)[(b) >> 5] &= ~(1 << ((b) & 0x1f)))

u8   comboReadPhysU8(u32 paddr);
s8   comboReadPhysI8(u32 paddr);
u16  comboReadPhysU16(u32 paddr);
s16  comboReadPhysI16(u32 paddr);
u32  comboReadPhysU32(u32 paddr);
s32  comboReadPhysI32(u32 paddr);

#endif
