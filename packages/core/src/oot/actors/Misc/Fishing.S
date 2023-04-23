#include <combo.h>

/* Accept smaller fish */
PATCH_START 0x80a42388
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42404
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42410
  lui at,0x4230
PATCH_END

PATCH_START 0x80a4247c
  lui at,0x4230
PATCH_END

/* Disable fishing anti-piracy */
PATCH_START 0x80a35160
  li v0,0
PATCH_END

/* Make fishing easier */
EasierFishing:
  lw      t2,(gSaveContext+0x04)
  bnez    t2,.Lafter
   andi   t8,t3,1
  bnez    t8,.Lret
   lui    t8, 0x4230
  lui     t8, 0x4250
  jr      ra
   nop

.Lafter:
  bnez t8,.Lret
  lui  t8,0x4210
  lui  t8,0x4238

.Lret:
  jr   ra
   nop

FishBite:
  la        t0,0x801F5DE0
  lw        at,0x009C(t0)
  bnez      at,.LFishBite_ret
   mul.s    $f4,$f10,$f2
  lui       t0,0x3f80
  mtc1      t0,$f4
.LFishBite_ret:
  jr ra
   nop

PATCH_START 0x80a35908
  jal     EasierFishing
   lui    at,0x4282
  mtc1    at,$f8
  mtc1    t8,$f18
  swc1    $f18,0x019c(s2)
PATCH_END

PATCH_START 0x80a35964
  nop
PATCH_END

PATCH_START 0x80a35988
  nop
PATCH_END

PATCH_START 0x80a3d570
  jal     FishBite
   lwc1   $f10,0x0198(s0)
PATCH_END

PATCH_START 0x80A3C7E0
  nop
  nop
  nop
PATCH_END

PATCH_START 0x80a3ec80
  nop
PATCH_END

PATCH_START 0x80a3ec9c
  nop
PATCH_END

PATCH_START 0x80a3ecac
  nop
PATCH_END

PATCH_START 0x80A3ED08
  move at,t5
PATCH_END

/* Make sinking lure always available */
PATCH_START 0x80a393e0
  nop
PATCH_END

PATCH_START 0x80a393f0
  nop
PATCH_END

/* Don't reset lure position */
PATCH_START 0x80a42544
  nop
PATCH_END

PATCH_START 0x80a4254c
  nop
PATCH_END

PATCH_START 0x80a4260c
  nop
PATCH_END

PATCH_START 0x80a42614
  nop
PATCH_END

/* Ignore sinking lure for prizes */
PATCH_START 0x80a4239c
  nop
  nop
PATCH_END

PATCH_START 0x80a423fc
  nop
  nop
PATCH_END

/* Display the normal text for the sinking lure */
PATCH_START 0x80a420b8
  li t8,0x4086
PATCH_END

PATCH_START 0x80A3CA0C
  li at,0x00c8
PATCH_END

PATCH_START 0x80A3CA20
  sh at,0x0192(s0)
PATCH_END

PATCH_START 0x80A3CA30
  swc1 $f6,0x01ac(s0)
PATCH_END
