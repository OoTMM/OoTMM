#include <combo.h>

/* Skip the empty bottle check */
PATCH_START 0x80b7801c
  li v0,1
PATCH_END

/* Disable talk */
PATCH_START 0x80b780bc
  nop
PATCH_END

PATCH_START 0x80b781bc
  nop
PATCH_END

/* Skip the text box */
PATCH_START 0x80b78030
  nop
PATCH_END

PATCH_START 0x80b77f68
  nop
  nop
PATCH_END

PATCH_START 0x80b77f78
  nop
  nop
PATCH_END

PATCH_START 0x80b78004
  nop
  nop
PATCH_END

PATCH_START 0x80b78014
  nop
  nop
PATCH_END
