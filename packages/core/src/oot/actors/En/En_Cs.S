#include <combo.h>

/* Disable "SOLD OUT" */
PATCH_START 0x80ae1a0c
  nop
PATCH_END
