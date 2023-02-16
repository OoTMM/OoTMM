#include <combo.h>

/* Gomess */

/* Skip cutscene */
PATCH_START 0x808c50d8
  li t3,1
PATCH_END

PATCH_START 0x808c5210
  li t9,1
PATCH_END
