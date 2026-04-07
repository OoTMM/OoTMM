#include <combo.h>

.balign 8
kOKUJOURoom00Objects:
.short 0x0192
.short 0x0188
.short 0x019C
.short 0x01BE
.short 0x01C8
.short 0x01B8
.short OBJECT_KANBAN

/* Expand the object list */
PATCH_VROM 0x21b8000 + 0x30
.int 0x0b070000
.int kOKUJOURoom00Objects - 0x80000000
PATCH_END
