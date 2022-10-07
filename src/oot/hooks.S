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
