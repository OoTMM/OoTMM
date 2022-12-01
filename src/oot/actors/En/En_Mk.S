#include <combo.h>

/* Skip Eye Drops dialogue */
PATCH_START 0x80aacbf4
  nop
  nop
PATCH_END

PATCH_START 0x80aacbfc
  li v0,1
PATCH_END

PATCH_START 0x80aacb38
  nop
  nop
PATCH_END

PATCH_START 0x80aacb40
  li v0,1
PATCH_END

PATCH_START 0x80aaca5c
  nop
  nop
PATCH_END

PATCH_START 0x80aaca64
  li v0,1
PATCH_END

PATCH_START 0x80aac99c
  li v0,0
PATCH_END

PATCH_START 0x80aac900
  li v0,0
PATCH_END

PATCH_START 0x80aac8a4
  li v0,0
PATCH_END

PATCH_START 0x80aac80c
  li v0,1
PATCH_END

/* Disable timer */
PATCH_START 0x80aac7a8
  nop
PATCH_END

PATCH_START 0x80aac7c4
  nop
PATCH_END

/* Disable check for Eye drops */
PATCH_START 0x80aacddc
  li t9,0xfff
PATCH_END
