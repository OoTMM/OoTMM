#include <combo.h>

/* Disable Nabooru in Spirit */
PATCH_START 0x809a41a8
  li t7,1
PATCH_END
