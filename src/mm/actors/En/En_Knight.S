#include <combo.h>

/* Remove the cutscene */
PATCH_START 0x809b5854
  li t8,0
PATCH_END
