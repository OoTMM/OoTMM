#include <combo.h>

/* Allow raising the temple even after clearing it */
PATCH_START 0x80aa84ac
  li t5,0
PATCH_END
