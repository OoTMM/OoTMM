#include <combo.h>

/* Skip epona cutscene */
PATCH_START 0x8091a1ac
  li t4,1
PATCH_END
