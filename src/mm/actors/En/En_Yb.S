#include <combo.h>

/* Disable precond */
PATCH_START 0x80bfaacc
  li at,0xfff
PATCH_END
