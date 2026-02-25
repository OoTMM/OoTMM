#include <combo.h>

/* Butterfly CSMC */
PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x1d30
.quad 0x0
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x1d68
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x1e30
.quad 0x0
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x1e68
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END

/* Textures for rocks */
PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x66c8
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END

PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x6778
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END

/* Combiner for rocks */
PATCH_OBJECT OBJECT_GAMEPLAY_FIELD_KEEP, 0x6700
.int 0xFC127E03
.int 0xFF0FFDFF
PATCH_END
