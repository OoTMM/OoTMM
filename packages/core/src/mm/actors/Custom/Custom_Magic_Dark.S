#include <combo.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_SPELL_LOVE + 0x14
.int Magic_Dark_InitVars
PATCH_END
