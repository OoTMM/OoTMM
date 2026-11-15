#include <combo.h>

.text
.global comboEnemyNativeEffectSpawn
comboEnemyNativeEffectSpawn:
  addiu sp,sp,-0x40
  sw ra,0x1c(sp)
#if defined(GAME_OOT)
  j 0x8001b470
#else
  j 0x800b04dc
#endif
   nop
