#include <combo.h>

/* Skip DC cutscene */
PATCH_START 0x809015d4
  li v0,1
PATCH_END
