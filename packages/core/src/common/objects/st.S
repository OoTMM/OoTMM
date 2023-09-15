#include <combo.h>

#if defined(GAME_OOT)
#define ID 0x024
#else
#define ID 0x020
#endif

PATCH_OBJECT ID, 0x42e8
.int 0xde000000
.int 0x0a000000
.int 0
.int 0
PATCH_END

PATCH_OBJECT ID, 0x4580
.int 0xde000000
.int 0x0a000000
.int 0
.int 0
PATCH_END
