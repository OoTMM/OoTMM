#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x00eb
#else
#define OBJECT_ID 0x00c1
#endif

/* Remove stick by default */
PATCH_OBJECT OBJECT_ID, 0x1698
.quad 0xdf00000000000000
PATCH_END
