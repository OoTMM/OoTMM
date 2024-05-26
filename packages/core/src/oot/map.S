#include <combo.h>

/* Always use the extended map for graveyard */
PATCH_START 0x8006bf94
  li t2,1
PATCH_END

/* Always use the extended map for gerudo fortress */
PATCH_START 0x8006c054
  move t2,at
PATCH_END
