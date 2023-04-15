#include <combo.h>

/* Colossal wallet has 3 digits */
PATCH_START 0x800f761e
.short 3
PATCH_END
