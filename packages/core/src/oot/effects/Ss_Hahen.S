#include <combo.h>

/* Use the custom table */
PATCH_START 0x80b315f8
  la    t3,gExObjectsAddr
  sll   t0,2
  addu  t3,t0
  lw    t3,0(t3)
  nop
  nop
PATCH_END
