#include <combo.h>

/* Igos */

PATCH_GROUP PG_MM_CS_IGOS
/* Skip intro cutscene */
PATCH_START 0x809f22c4
  li t1,1
PATCH_END

PATCH_GROUP_END
