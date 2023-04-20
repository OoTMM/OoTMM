#include <combo.h>

/* Use our custom handler */
PATCH_START 0x80bcfcf0
  la at,EnHg_HandlerGiveMask
  sw at,0x1d4(a2)
PATCH_END

PATCH_START 0x80bcfd08
  nop
PATCH_END
