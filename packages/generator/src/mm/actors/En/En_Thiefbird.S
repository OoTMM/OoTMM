#include <combo.h>

/* Disable major item thief */
PATCH_START 0x80c10b0c
  jr ra
   li v0,0
PATCH_END
