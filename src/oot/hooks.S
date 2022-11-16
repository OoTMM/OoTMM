#include <combo.h>

/* Hook into Main, to load the payload */
PATCH_START(0x800005ac)
  jal 0x80006600
  nop
PATCH_END

/* Hook into ReadWriteSRAM */
PATCH_START(0x80091474)
  j comboReadWriteFlash
  nop
PATCH_END

/* Expand the Heap */
PATCH_START 0x800a4330
  lui t7,0x8060
PATCH_END

PATCH_START 0x800a4350
  lui t8,0x8060
PATCH_END
