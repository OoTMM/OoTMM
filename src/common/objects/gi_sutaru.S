#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x015c
#else
#define OBJECT_ID 0x0125
#endif

PATCH_OBJECT OBJECT_ID, 0x450
.quad 0
.quad 0
PATCH_END
