#ifndef COMBO_UTIL_H
#define COMBO_UTIL_H

#include <stddef.h>

#define ASSERT_OFFSET(s,f,o) _Static_assert(offsetof(s,f) == o, "Offset of " #f " in " #s " is not " #o)

#endif
