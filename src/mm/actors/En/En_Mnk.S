#include <combo.h>

/* Disable deku pipe instrument sound */
PATCH_START 0x80ab7f44
  nop
PATCH_END

PATCH_START 0x80ab82dc
  nop
PATCH_END
