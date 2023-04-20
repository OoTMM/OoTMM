#include <combo.h>

/* Disable precond */
PATCH_START 0x80a7298c
  li t3,0xfff
PATCH_END
