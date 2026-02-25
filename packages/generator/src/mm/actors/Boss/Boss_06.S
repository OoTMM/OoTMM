#include <combo.h>

/* Igos */

/* Skip intro cutscene */
PATCH_START 0x809f22c4
  li t1,1
PATCH_END
