#include <combo.h>

/* Skip Zelda Letter dialogue */
PATCH_START 0x8097dcd4
  nop
  nop
PATCH_END

PATCH_START 0x8097dce4
  nop
PATCH_END

PATCH_START 0x8097e0f8
  nop
  nop
PATCH_END

PATCH_START 0x8097e108
  nop
PATCH_END

/* Disable "SOLD OUT" when selling the mask */
PATCH_START 0x8097e1fc
  nop
PATCH_END
