#include <combo.h>

/* Use our custom flags for owl flags */
PATCH_START 0x800f4ab4
  lui a0,%HI(gMmOwlFlags)
PATCH_END

PATCH_START 0x800f4b00
  lw a0,%LO(gMmOwlFlags)(a0)
PATCH_END

PATCH_START 0x800f4b1c
  lw a0,(gMmOwlFlags)
PATCH_END

PATCH_START 0x80157a5c
  lw t6,(gMmOwlFlags)
  nop
PATCH_END
