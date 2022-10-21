#include <combo.h>

/* Disable "SOLD OUT" */
PATCH_START 0x80ad0a34
  nop
PATCH_END
