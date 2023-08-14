#include <combo.h>

/* Use our draw wrapper */
PATCH_START 0x80abc22c
.int ObjHana_DrawWrapper
PATCH_END

/* Reloc */
PATCH_START 0x80abc310
.int 0
PATCH_END
