#include <combo.h>

#if defined(GAME_OOT)
# define OBJECT OBJECT_ICE_OBJECTS
# define OFFSET 0x23d0
#endif

#if defined(GAME_MM)
# define OBJECT OBJECT_ICICLE
# define OFFSET 0xd0
#endif

/* Disable default color */
PATCH_OBJECT OBJECT, OFFSET + 0x70
.quad 0
PATCH_END
