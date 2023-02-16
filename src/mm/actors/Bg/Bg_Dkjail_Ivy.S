#include <combo.h>

/* Skip princess cutscene */
PATCH_START 0x80ade76c
  nop
PATCH_END
