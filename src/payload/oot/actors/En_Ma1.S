#include <combo/asm.h>

/* Hook into out item giving func */
PATCH_START(0x809f1288)
  jal comboGetItemInRange
PATCH_END
