#include <combo.h>

.globl comboGameSwitch2
comboGameSwitch2:
  la sp,(kComboDmaData + 0x2000)
  j comboGameSwitch3
   nop
