#include <combo.h>

/* Disable the env check for the clock town scrub */
PATCH_START 0x80add204
  nop
PATCH_END
