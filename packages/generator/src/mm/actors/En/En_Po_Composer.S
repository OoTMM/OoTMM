#include <combo.h>

/* Use short dialogue */
PATCH_START 0x80bc51b4
  li t4,1
PATCH_END
