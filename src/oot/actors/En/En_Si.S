#include <combo.h>

/* Nop other drawing code */
PATCH_START 0x80b4b408
  nop
PATCH_END

PATCH_START 0x80b4b414
  nop
PATCH_END

/* Pass the actor as the second arg */
PATCH_START 0x80b4b194
  move a1,s0
PATCH_END

/* Nop the textbox */
PATCH_START 0x80b4b1ac
  nop
PATCH_END
