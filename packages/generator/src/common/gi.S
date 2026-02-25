#include <combo.h>

/* Hook into LoadObjectGi */
PATCH_START(LoadObjectGi)
  j comboLoadObjectGi
   nop
PATCH_END
