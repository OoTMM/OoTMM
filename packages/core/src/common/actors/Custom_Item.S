#include <combo.h>

#if defined(GAME_OOT)
PATCH_START 0x800e8824
.int CustomItem_gActorInit
PATCH_END
#endif
