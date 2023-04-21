#include <combo.h>

/* Change the actor spawn check */
PATCH_START 0x80aa0d80
  nop
  nop
  jal EnHs_InitHook
PATCH_END

/* Skip dialogue */
PATCH_START 0x80aa11b4
  nop
  nop
PATCH_END

PATCH_START 0x80aa11c4
  nop
PATCH_END

PATCH_START 0x80aa10a0
  nop
  nop
PATCH_END

PATCH_START 0x80aa10b0
  nop
PATCH_END

PATCH_START 0x80aa10bc
  li v0,0
PATCH_END

/* Disable the timer */
PATCH_START 0x80aa0f1c
  nop
PATCH_END

PATCH_START 0x80aa0f44
  nop
PATCH_END
