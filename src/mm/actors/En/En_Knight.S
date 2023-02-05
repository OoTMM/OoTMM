#include <combo.h>

/* Remove the death cutscene */
PATCH_START 0x809b5854
  li t8,0
PATCH_END

/* Remove the intro cutscene */
PATCH_START 0x809b2c80
  li t9,1
PATCH_END
