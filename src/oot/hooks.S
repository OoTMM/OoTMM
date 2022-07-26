#include <combo/asm.h>

/* Hook into InitHeap, to load the payload */
PATCH_START(0x800cd3c0)
  j 0x80006600
  nop
PATCH_END

/* Hook into ReadWriteSRAM */
PATCH_START(0x80091474)
  j comboReadWriteFlashHook
  nop
PATCH_END
