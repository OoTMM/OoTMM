#include <combo.h>

.balign 8
kKakusiabaRoom10Objects:
.short 0x0146
.short 0x01B9
.short 0x01EB
.short 0x0031
.short OBJECT_KANBAN

/* Expand the object list */
PATCH_VROM 0x2ec7000 + 0x28
.int 0x0b050000
.int kKakusiabaRoom10Objects - 0x80000000
PATCH_END
