#include <combo.h>

/* Always give the quiver upgrades */
PATCH_START 0x809c7b24
  li t2,0
PATCH_END

PATCH_START 0x809c7dd0
  li t3,0
PATCH_END

/* Speed up swamp shooting gallery score count */
PATCH_START 0x809c86a0
  nop
PATCH_END

/* Fix for FD link */
PATCH_START 0x809c6fb8
  slti at,v0,2
  bnez at,. + 0x25c
PATCH_END
