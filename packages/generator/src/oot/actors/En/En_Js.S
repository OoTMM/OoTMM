#include <combo.h>

/* Use our own handler for the buy */
PATCH_START 0x80adb790
  li at,0
PATCH_END

PATCH_START 0x80adb7b4
  nop
PATCH_END
