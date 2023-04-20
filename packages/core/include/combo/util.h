#ifndef COMBO_UTIL_H
#define COMBO_UTIL_H

#include <stddef.h>

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

#endif
