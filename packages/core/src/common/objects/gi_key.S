#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x00aa
#else
#define OBJECT_ID 0x0086
#endif

/* Disable the default coloring */
PATCH_OBJECT OBJECT_ID, 0x818
.quad 0
.quad 0
PATCH_END
