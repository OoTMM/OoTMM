#include <combo.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_WARP + 0x14
.int CustomWarp_gActorInit
PATCH_END
