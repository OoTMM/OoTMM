#include <combo.h>

/*
Replaces:
  LW      T7, 0x001C (SP)
  LW      A0, 0x0018 (SP)
  ADDIU   T9, R0, 0x001A
  LW      T8, 0x1CCC (T7)
  SH      T9, 0x020A (A0)
  SW      T8, 0x0120 (A0)
*/
PATCH_START 0x808C112C
  lw      a0, 0x0018 (sp)
  jal     ArmsHook_SetTimerAndParent
  lw      a1, 0x001C (sp)
  nop
  nop
  nop
PATCH_END
