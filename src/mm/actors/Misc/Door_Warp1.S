#include <combo.h>

/* Patch DoorWarp1_RemainsIndex */
PATCH_START 0x808b849c
  jr ra
   li v0,0
PATCH_END

