#include <combo.h>

/* Disable hookshot check */
PATCH_START 0x80a7cfc4
  li t5,1
PATCH_END
