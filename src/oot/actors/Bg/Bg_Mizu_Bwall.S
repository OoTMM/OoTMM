#include <combo.h>

/* Skip DC cutscene */
PATCH_START 0x80901618
  nop
PATCH_END
