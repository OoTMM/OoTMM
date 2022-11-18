#include <combo.h>

/* Disable every switch-based elf message */
PATCH_START 0x80ba19b4
  li v0,1
PATCH_END
