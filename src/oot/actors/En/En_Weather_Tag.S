#include <combo.h>

/* Disable snow in ZD before beating water, as it interferes with storms */
PATCH_START 0x80ad732c
  li t1,1
PATCH_END
