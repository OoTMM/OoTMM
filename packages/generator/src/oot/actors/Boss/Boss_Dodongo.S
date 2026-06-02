#include <combo.h>

/* Fast boss intro */
PATCH_START 0x8089e7c8
  li t8,1
PATCH_END

PATCH_START 0x8089f0c4
  j     BossDodongo_AfterSetupExplode
   or   a0, s0, zero
PATCH_END

PATCH_START 0x8089f594
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
