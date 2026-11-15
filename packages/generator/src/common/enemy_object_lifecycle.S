#include <combo.h>

.text
.global comboObjectNativeFinishRoomChange
comboObjectNativeFinishRoomChange:
  addiu sp,sp,-0x28
  sw s0,0x18(sp)
#if defined(GAME_OOT)
  j 0x80080ca0
#else
  j 0x8012ec00
#endif
   nop
