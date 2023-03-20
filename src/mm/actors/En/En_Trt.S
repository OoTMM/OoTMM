#include <combo.h>

/* Disable normal textboxes */
PATCH_START 0x80a8bd08
  nop
PATCH_END

PATCH_START 0x80a8c960
  nop
PATCH_END

PATCH_START 0x80a8cbd8
  nop
PATCH_END

PATCH_START 0x80a8cfc8
  nop
PATCH_END

PATCH_START 0x80a8df08
  nop
PATCH_END

PATCH_START 0x80a8dfa0
  nop
PATCH_END

/* Disable the original confirm textbox */
PATCH_START 0x80a8cab4
  nop
PATCH_END
