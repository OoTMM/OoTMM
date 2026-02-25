#include <combo.h>

/* Use our own DMA handler */
PATCH_START 0x80080a08
  j DmaManagerRunRequest
   nop
PATCH_END
