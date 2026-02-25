#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x00c7
#else
#define OBJECT_ID 0x009f
#endif

PATCH_OBJECT OBJECT_ID, 0x4e8
.quad 0
.quad 0
PATCH_END
