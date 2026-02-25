#include <combo.h>

/* Use our own DMA handler */
PATCH_START 0x80000b0c
  j DmaManagerRunRequest
   nop
PATCH_END
