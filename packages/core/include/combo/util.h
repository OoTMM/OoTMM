#ifndef COMBO_UTIL_H
#define COMBO_UTIL_H

#include <stddef.h>
#include <combo/types.h>

#define ASSERT_SIZE(s, sz)      _Static_assert(sizeof(s) == sz, #s " size is wrong")
#define ASSERT_OFFSET(s,f,o)    _Static_assert(offsetof(s,f) == o, "Offset of " #f " in " #s " is not " #o)

#define BITMAP8_GET(m,b)   (!!((m)[(b) >> 3] & (1 << ((b) & 0x7))))
#define BITMAP8_SET(m,b)   ((m)[(b) >> 3] |= (1 << ((b) & 0x7)))
#define BITMAP8_CLEAR(m,b) ((m)[(b) >> 3] &= ~(1 << ((b) & 0x7)))

#define BITMAP16_GET(m,b)   (!!((m)[(b) >> 4] & (1 << ((b) & 0xf))))
#define BITMAP16_SET(m,b)   ((m)[(b) >> 4] |= (1 << ((b) & 0xf)))
#define BITMAP16_CLEAR(m,b) ((m)[(b) >> 4] &= ~(1 << ((b) & 0xf)))

#define BITMAP32_GET(m,b)   (!!((m)[(b) >> 5] & (1 << ((b) & 0x1f))))
#define BITMAP32_SET(m,b)   ((m)[(b) >> 5] |= (1 << ((b) & 0x1f)))
#define BITMAP32_CLEAR(m,b) ((m)[(b) >> 5] &= ~(1 << ((b) & 0x1f)))

#define ALIGN8(val) (((val) + 7) & ~7)
#define ALIGN16(val) (((val) + 0xF) & ~0xF)
#define ALIGN32(val) (((val) + 0x1F) & ~0x1F)
#define ALIGN64(val) (((val) + 0x3F) & ~0x3F)
#define ALIGN256(val) (((val) + 0xFF) & ~0xFF)

u8   comboReadPhysU8(u32 paddr);
s8   comboReadPhysI8(u32 paddr);
u16  comboReadPhysU16(u32 paddr);
s16  comboReadPhysI16(u32 paddr);
u32  comboReadPhysU32(u32 paddr);
s32  comboReadPhysI32(u32 paddr);

#endif
