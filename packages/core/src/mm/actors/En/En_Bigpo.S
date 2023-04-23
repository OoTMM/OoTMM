#include <combo.h>

/* Dampe QoL */
PATCH_START 0x80b6354c
  nop
  nop
  nop
  nop
  li t0,8
PATCH_END
