#include <combo.h>

/* Use our wrappers */
PATCH_START 0x8086a090
.int EnBox_InitWrapper
PATCH_END

PATCH_START 0x8086a09c
.int EnBox_DrawWrapper
PATCH_END

/* Disable some relocs */
PATCH_START 0x8086a26c
.int 0
PATCH_END

PATCH_START 0x8086a278
.int 0
PATCH_END
