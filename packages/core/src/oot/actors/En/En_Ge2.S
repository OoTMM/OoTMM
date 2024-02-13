#include <combo.h>

/* Use our hook */
PATCH_START 0x80b23f60
  j EnGe1_AfterCaught
PATCH_END

PATCH_START 0x80b24094
  j EnGe1_AfterCaught
PATCH_END

/* Relocs for the stone mask hook */
PATCH_START 0x80b252b8
.int 0
.int 0
PATCH_END
