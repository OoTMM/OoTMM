#include <combo.h>

/* Fast push */
PATCH_START 0x8092864c
  slti at,t7,1
PATCH_END
