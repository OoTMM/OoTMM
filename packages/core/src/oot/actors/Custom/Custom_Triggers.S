#include <combo.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_TRIGGERS + 0x14
.int CustomTriggers_gActorInit
PATCH_END
