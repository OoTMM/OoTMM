#include <combo.h>

/* Force quiver2 as the 2nd reward */
PATCH_START 0x80a90198
  move v0,at
PATCH_END

PATCH_START 0x80a90260
  move v0,at
PATCH_END

/* Use our hook */
PATCH_START 0x80a8f668
  j EnGe1_AfterCaught
PATCH_END

/* Relocs for caught hook */
PATCH_START 0x80a90f24
.int 0
.int 0
PATCH_END

PATCH_START 0x80a90f58
.int 0
.int 0
PATCH_END
