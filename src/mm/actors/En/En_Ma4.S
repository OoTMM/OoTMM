#include <combo.h>

/* Never play the epona song cutscene */
PATCH_START 0x80abee58
  nop
PATCH_END

/* Skip the whole alien cutscene */
PATCH_START 0x80abf8b4
  li t3,1
PATCH_END
