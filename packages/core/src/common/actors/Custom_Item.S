#include <combo.h>

#if defined(GAME_OOT)
#define INIT_ADDR 0x800e8824
#else
#define INIT_ADDR 0x801af504
#endif

PATCH_START INIT_ADDR
.int CustomItem_gActorInit
PATCH_END
