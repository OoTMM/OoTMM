#include <combo.h>

.globl comboGameSwitch2
comboGameSwitch2:
  la sp,(kComboDmaData + 0x20002000)
  la t0,(comboGameSwitch3 + 0x20000000)
  jr t0
   nop

.globl comboGameSwitch4
comboGameSwitch4:
  /* Reset the tag registers */
  mtc0 zero,$28
  mtc0 zero,$29

  /* Reset the icache */
  la t0,0x80000000
  la t1,0x80004000
.LicacheLoop:
  cache 8,(t0)
  addiu t0,0x20
  bne t0,t1,.LicacheLoop
   nop

  /* Reset the dcache */
  la t0,0x80000000
  la t1,0x80002000
.LdcacheLoop:
  cache 9,(t0)
  addiu t0,0x10
  bne t0,t1,.LdcacheLoop
   nop

  /* Execute the game */
  jr a0
   nop
