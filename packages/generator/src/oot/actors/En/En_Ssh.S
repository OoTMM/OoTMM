#include <combo.h>

/* Make cursed men fall immediately. */
PATCH_START 0x80b26128
  lui at,0x44c8
PATCH_END
