#include <combo.h>

/* Always give the quiver upgrades */
PATCH_START 0x809c7b24
  li t2,0
PATCH_END

PATCH_START 0x809c7dd0
  li t3,0
PATCH_END
