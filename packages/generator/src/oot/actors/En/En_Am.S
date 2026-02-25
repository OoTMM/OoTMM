#include <combo.h>
/* Starts at 0x808F9080 */

/* Armos statue fast push */
en_am_calculation_1:
    lui     at, 0x3F81
    ori     at, at, 0x3F19
    mtc1    at, $f8
    jr      ra
    nop

en_am_calculation_2:
    lui     at, 0x3F81
    ori     at, at, 0x3F19
    mtc1    at, $f10
    jr      ra
    nop

PATCH_START 0x808F9080 + 0x1428
  addiu t0, a2, 0xF000
PATCH_END

PATCH_START 0x808F9080 + 0x1528
  jal en_am_calculation_1
   nop
PATCH_END

PATCH_START 0x808F9080 + 0x15E0
  jal en_am_calculation_2
   nop
PATCH_END

