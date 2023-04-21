#include <combo.h>

/* Skip cutscenes */
PATCH_START 0x80983550
  li v0,1
PATCH_END
