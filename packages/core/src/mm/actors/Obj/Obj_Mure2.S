#include <combo.h>

/* Resize */
PATCH_START 0x8096157c
.int 0x188
PATCH_END

/* Use our own init handler */
PATCH_START 0x80961580
.int ObjMure2_InitWrapper
PATCH_END

/* Reloc */
PATCH_START 0x809616c4
.int 0
PATCH_END

/* Use our own children spawning handler */
PATCH_START 0x8096145c
  jal ObjMure2_SpawnChildrenWrapper
PATCH_END

/* Reloc */
PATCH_START 0x809616a0
.int 0
PATCH_END
