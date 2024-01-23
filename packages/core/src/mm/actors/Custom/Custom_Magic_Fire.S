#include <combo.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_SPELL_FIRE + 0x14
.int Magic_Fire_InitVars
PATCH_END
