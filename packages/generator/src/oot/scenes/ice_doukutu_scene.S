#include <combo.h>

/* Alcove camera fix */
PATCH_VROM 0x2BEB000 + 0x1a25
.byte 0x01
PATCH_END

PATCH_VROM 0x2BEB000 + 0x1a2d
.byte 0x01
PATCH_END
