#include <combo.h>

#if defined(GAME_OOT)
#define OBJECT OBJECT_GAMEPLAY_FIELD_KEEP
#define OFFSET 0x95b0
#endif

#if defined(GAME_MM)
#define OBJECT OBJECT_COMB
#define OFFSET 0xcb0
#endif

/* Load the texture */
PATCH_OBJECT OBJECT, OFFSET + 0x18
.quad 0xde00000008000000
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END

/* Disable the default color */
PATCH_OBJECT OBJECT, OFFSET + 0x70
.quad 0x0
PATCH_END
