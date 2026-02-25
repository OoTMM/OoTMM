#include <combo.h>

/* FD link fixes */
PATCH_START 0x80c04be0
  li t0,2
PATCH_END

PATCH_START 0x80c04bec
  slti at,v0,3
  sub  t0,at
  sh   t0,0x2c2(a3)
  sh   t9,0x2ce(a3)
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
