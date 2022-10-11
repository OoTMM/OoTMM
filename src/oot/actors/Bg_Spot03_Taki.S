#include <combo.h>

PATCH_START 0x80a795cc
  jal comboZoraWaterfallDown
PATCH_END

PATCH_START 0x80a795f8
  nop
PATCH_END

PATCH_START 0x80a796d8
  nop
PATCH_END
