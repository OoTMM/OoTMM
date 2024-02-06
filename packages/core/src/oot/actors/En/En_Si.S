#include <combo.h>

/* Nop other drawing code */
PATCH_START 0x80b4b408
  nop
PATCH_END

PATCH_START 0x80b4b414
  nop
PATCH_END

/* Normal handler - Pass the actor as the second arg */
PATCH_START 0x80b4b194
  move a1,s0
PATCH_END

/* Nop the textbox */
PATCH_START 0x80b4b1ac
  nop
PATCH_END

/* Same for boomerang/hookshot handler */
PATCH_START 0x80b4b284
  move a1,s0
PATCH_END

PATCH_START 0x80b4b29c
  nop
PATCH_END

/* Disable the default fanfare */
PATCH_START 0x80b4b1b4
  nop
PATCH_END

PATCH_START 0x80b4b2a4
  nop
PATCH_END
