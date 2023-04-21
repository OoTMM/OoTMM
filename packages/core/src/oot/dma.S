#include <combo.h>

/* Hook into DmaManagerRunRequest, to use our extended DMA table */
PATCH_START 0x80000b10
  la a2, kComboDmaData
PATCH_END
