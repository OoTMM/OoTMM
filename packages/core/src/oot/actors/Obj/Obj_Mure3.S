#include <combo.h>

/* Resize */
PATCH_START 0x80b5584c
.int 0x164
PATCH_END

/* Reloc */
PATCH_START 0x80b558e4
.int 0
PATCH_END
