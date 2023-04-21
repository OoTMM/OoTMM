#include <combo.h>

/* Fast push */
PATCH_START 0x80a33450
  slti at,t8,1
PATCH_END
