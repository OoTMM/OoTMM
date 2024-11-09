#include <combo.h>

/* Expand the object list */
PATCH_VROM 0x2eb6000 + 0x28
.int 0x0b030000
.int 0x03001998
PATCH_END

PATCH_VROM 0x2eb6000 + 0x1998
.short 0x000c
.short 0x0031
.short OBJECT_KANBAN
PATCH_END
