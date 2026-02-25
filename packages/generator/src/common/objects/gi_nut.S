#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT_ID 0x00bb
#else
#define OBJECT_ID 0x0094
#endif

PATCH_OBJECT OBJECT_ID, 0x0ea8
.quad 0
.quad 0
PATCH_END

PATCH_OBJECT OBJECT_ID, 0x1150
.quad 0xde00000009000000 /* gSPDisplayList 0x09000000 */
PATCH_END
