#include <combo.h>

/* Disable some stuff related to the target */
PATCH_START 0x80b4f740
  nop
PATCH_END

PATCH_START 0x80b4f750
  nop
PATCH_END

PATCH_START 0x80b4f764
  nop
PATCH_END
