#include <combo.h>

/* Disable ruto in water temple */
PATCH_START 0x809c8a1c
  li v0,1
PATCH_END
