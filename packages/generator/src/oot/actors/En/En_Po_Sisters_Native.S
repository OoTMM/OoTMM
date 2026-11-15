#include <combo.h>

PATCH_START 0x8093d6e8
  jal comboEnemyNativePoeSisterSwitch
PATCH_END

PATCH_START 0x8093e570
  jal comboEnemyNativePoeSisterSwitch
PATCH_END

PATCH_START 0x8093e59c
  jal comboEnemyNativePoeSisterUnset
PATCH_END

PATCH_START 0x8093ea08
  jal comboEnemyNativePoeSisterBgm
PATCH_END

PATCH_START 0x8093c5cc
  jal comboEnemyNativePoeSisterRestore
PATCH_END

PATCH_START 0x8093cdf8
  jal comboEnemyNativePoeSisterCutscene
PATCH_END

PATCH_START 0x8093d204
  jal comboEnemyNativePoeSisterCutscene
PATCH_END

PATCH_START 0x8093f0b4
  jal comboEnemyNativePoeSisterCutscene
PATCH_END

PATCH_START 0x80940ec4
  .word 0
PATCH_END
