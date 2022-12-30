#include <combo.h>

/* Make Navi shut up */
PATCH_START 0x80a7647c
  li v0,1
PATCH_END
