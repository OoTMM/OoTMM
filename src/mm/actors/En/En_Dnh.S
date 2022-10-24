#include <combo.h>

/* Bytecode - Disable pictobox check */
PATCH_START 0x80a51276
.short 0
PATCH_END

/* Bytecode - Disable some event check */
PATCH_START 0x80a51394
.short 0
PATCH_END
