#include <combo/asm.h>

/* Hook into LoadObjectGi */
PATCH_START(LoadObjectGi)
  j comboLoadObjectGi
   nop
PATCH_END
