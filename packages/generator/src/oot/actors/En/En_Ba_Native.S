#include <combo.h>

PATCH_START 0x809e92b8
  jal comboEnemyNativeTentacleGet
PATCH_END

PATCH_START 0x809ea950
  jal comboEnemyNativeTentacleSet
PATCH_END
