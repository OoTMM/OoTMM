#include <combo.h>

/* Fast push */
PATCH_START 0x80b7f2a4
  lui a2,0x3fcc
PATCH_END

PATCH_START 0x80b7f2b4
  lui a1,0x4100
PATCH_END
