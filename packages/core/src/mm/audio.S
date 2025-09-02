#include <combo.h>

/* Fix a vanilla bug that can cause a crash with some custom songs */
PATCH_START 0x80198ffc
  move t6,t5
PATCH_END

/* Make Kamaro use SEQ_PLAYER_BGM_SUB instead of SEQ_PLAYER_BGM_MAIN */
PATCH_START 0x800b9e14
  li a0, 3
PATCH_END
