#include <combo.h>

/* Dampe QoL */
PATCH_START 0x80b6354c
  nop
  nop
  nop
  nop
  li t0,8
PATCH_END

/* GI renumbering */
PATCH_START 0x80b63364
  li a2,GI_MM_SCOOP
PATCH_END
