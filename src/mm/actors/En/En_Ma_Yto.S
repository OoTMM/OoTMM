#include <combo.h>

/* Remove the random chance of getting nothing */
PATCH_START 0x80b8e9b8
  li at,0xfff
PATCH_END
