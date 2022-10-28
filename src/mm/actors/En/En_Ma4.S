#include <combo.h>

/* Never play the epona song cutscene */
PATCH_START 0x80abee58
  nop
PATCH_END
