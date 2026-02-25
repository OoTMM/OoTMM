#include <combo.h>

/* Debug screen on crash */
PATCH_START 0x80083a1c
  jal osWritebackDCacheAll
PATCH_END

/* Disable auto-scroll */
PATCH_START 0x80083a30
  sb zero,0x7cf(t8)
PATCH_END

PATCH_START 0x80082fa4
  nop
PATCH_END
