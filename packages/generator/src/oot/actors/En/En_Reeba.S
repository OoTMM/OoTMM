#include <combo.h>

PATCH_START 0x80890720
  jal comboEnemyNativeLeeverFloor
PATCH_END

PATCH_START 0x80890b70
  jal comboEnemyNativeLeeverFloor
PATCH_END

PATCH_START 0x80890cb0
  jal comboEnemyNativeLeeverFloor
PATCH_END

/* Assign aimType only to native Leevers */
PATCH_START 0x8096f850
  nop
PATCH_END
