#include <combo.h>

/* Fix a vanilla bug that can cause a crash with some custom songs */
PATCH_START 0x80198ffc
  move t6,t5
PATCH_END
