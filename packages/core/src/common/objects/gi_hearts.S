#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x00bd
#else
#define OBJECT_ID 0x0096
#endif

/* For double defense support */
PATCH_OBJECT OBJECT_ID, 0x1290
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_ID, 0x12b0
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_ID, 0x1470
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_ID, 0x1490
.quad 0x0
PATCH_END
