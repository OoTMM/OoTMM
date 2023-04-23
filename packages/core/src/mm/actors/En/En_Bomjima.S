#include <combo.h>

/* Fix for FD link */
PATCH_START 0x80bfe340
  slti at,v1,2
  beqz at,. + 0x10
  nop
  sh   a3,0x2c8(a0)
PATCH_END
