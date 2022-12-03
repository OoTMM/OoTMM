#include <combo.h>

.globl comboGameSwitch2
comboGameSwitch2:
  la sp,(kComboDmaData + 0x2000)
  li a0,0xfffffff0
  and sp,a0
  j comboGameSwitch3
   nop

.globl comboGameSwitch4
comboGameSwitch4:
  /* Reset the tag registers */
  mtc0 zero,$28
  mtc0 zero,$29

  /* Reset the timer and cause */
  mtc0 zero, $9
  mtc0 zero, $11
  mtc0 zero, $13

#if 0
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
#endif

  /* Execute the game */
  jr a0
   nop
