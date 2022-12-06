#include <combo.h>

/* Remove precond */
PATCH_START 0x80bc728c
  li t5,0xfff
PATCH_END
