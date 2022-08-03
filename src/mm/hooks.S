#include <combo/asm.h>

/* Rebase MM */
PATCH_START(0x80092960)
  lui t7, 0xb400
PATCH_END

/* Downsize the heap by 0x20000 */
PATCH_START(0x80174c4c)
  lui t8, 0x8076
PATCH_END

/* Hook before InitDmaManager to load the payload */
PATCH_START(0x8008035c)
  jal 0x800982b0
  nop
PATCH_END

/* Skip the N64 logo */
PATCH_START(0x808006cc)
  li t9, 1
PATCH_END

PATCH_START(0x80800134)
  jr ra
  nop
PATCH_END
