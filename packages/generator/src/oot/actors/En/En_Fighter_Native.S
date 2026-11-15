#include <combo.h>

PATCH_START 0x80b43890
  jal comboEnemyNativeFighterPlayerCs
PATCH_END

PATCH_START 0x80b41350
  jal comboEnemyNativeFighterRestore
PATCH_END

PATCH_START 0x80b44ff8
  jal comboEnemyNativeFighterRestore
PATCH_END

PATCH_START 0x80b438a0
  jal comboEnemyNativeFighterText
PATCH_END
