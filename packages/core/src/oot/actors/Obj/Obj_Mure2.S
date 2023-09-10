#include <combo.h>

/* Resize */
PATCH_START 0x80abdfec
.int 0x180
PATCH_END

/* Use our own init handler */
PATCH_START 0x80abdff0
.int ObjMure2_InitWrapper
PATCH_END

/* Reloc */
PATCH_START 0x80abe14c
.int 0
PATCH_END

/* Use our own children spawning handler */
PATCH_START 0x80abdec8
  jal ObjMure2_SpawnChildrenWrapper
PATCH_END

/* Reloc */
PATCH_START 0x80abe128
.int 0
PATCH_END
