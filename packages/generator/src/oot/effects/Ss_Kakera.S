#include <combo.h>

/* Use the custom table */
PATCH_START 0x80b34d04
  la    t8,gExObjectsAddr
  sll   t5,2
  addu  t8,t5
  lw    t8,0(t8)
  nop
  nop
PATCH_END

PATCH_START 0x80b34d48
  la    t4,gExObjectsAddr
  sll   t1,2
  addu  t4,t1
  lw    t4,0(t4)
  nop
  nop
PATCH_END
