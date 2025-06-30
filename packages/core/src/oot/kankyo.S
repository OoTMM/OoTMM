#include <combo.h>

/* Force Sandstorm state to dissipating instead of acting in Haunted Wastelands */
PATCH_START 0x800628b0
  li t9, 4
PATCH_END
