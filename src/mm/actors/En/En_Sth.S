#include <combo.h>

/* Ocean - skip check for wallet */
PATCH_START 0x80b676d0
  li t8,0
PATCH_END

/* Ocean Spider House: Always give the good reward. */
PATCH_START 0x80b6755c
  li t0,0
PATCH_END

/* Remove precond for mask of truth */
PATCH_START 0x80b67a40
  li at,0xfff
PATCH_END
