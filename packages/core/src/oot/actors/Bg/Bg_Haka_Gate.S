#include <combo.h>

/* Fast push */
PATCH_START 0x80a30700
  slti at,v0,0x19
PATCH_END

PATCH_START 0x80a30874
  li t8,0
PATCH_END
