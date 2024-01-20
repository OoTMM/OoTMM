#include <combo.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_SPELL_WIND + 0x14
.int Magic_Wind_InitVars
PATCH_END
