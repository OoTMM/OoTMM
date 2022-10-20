#include <combo.h>

/* Skip text */
PATCH_START 0x80aabec4
  nop
  nop
PATCH_END

PATCH_START 0x80aabed4
  nop
PATCH_END

PATCH_START 0x80aabee0
  li v0,0
PATCH_END

PATCH_START 0x80aabca4
  nop
  nop
PATCH_END

PATCH_START 0x80aabcb4
  nop
  nop
PATCH_END
