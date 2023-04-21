#include <combo.h>

/* Fix kakariko gate unlocking by time travel */
PATCH_START 0x80a50af8
  li t9,0
PATCH_END
