#include <combo.h>

/* Use malloc to allocate the (now oversized) bank descriptors */
PATCH_START 0x80190eb0
  sll a0,a1,0x02
  jal malloc
PATCH_END

/* Use our custom seq table */
PATCH_START 0x80190e14
  lui t6,%HI(gCustomAudioTables)
  addiu t6,%LO(gCustomAudioTables)
  lw t1,0x00(t6)
  lw t2,0x04(t6)
  lw t3,0x08(t6)
  lw t6,0x0c(t6)
  nop
  nop
PATCH_END

/* Don't re-process tables */
PATCH_START 0x80190e68
  nop
PATCH_END

PATCH_START 0x80190e7c
  nop
PATCH_END

PATCH_START 0x80190e90
  nop
PATCH_END
