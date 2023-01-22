#ifndef COMBO_ASM_H
#define COMBO_ASM_H

.set noreorder
.set nobopt
.set noat

#include <PR/rcp.h>
#include <combo/defs.h>

/* N64 regdefs don't march ours */
#define zero  $0
#define at    $1
#define v0    $2
#define v1    $3
#define a0    $4
#define a1    $5
#define a2    $6
#define a3    $7
#define t0    $8
#define t1    $9
#define t2    $10
#define t3    $11
#define t4    $12
#define t5    $13
#define t6    $14
#define t7    $15
#define s0    $16
#define s1    $17
#define s2    $18
#define s3    $19
#define s4    $20
#define s5    $21
#define s6    $22
#define s7    $23
#define t8    $24
#define t9    $25
#define k0    $26
#define k1    $27
#define gp    $28
#define sp    $29
#define fp    $30
#define ra    $31

.macro PATCH_START addr
.section .patch, "awx"
.int 0x1
.int \addr
.int (1f - 0f)
0:
.endm

.macro PATCH_LOAD_STORE bits, unsigned
.section .patch, "awx"
.int 0x2
.short \bits
.short \unsigned
.int (1f - 0f)
0:
.endm

.macro PATCH_REL_HILO addr
.section .patch, "awx"
.int 0x3
.int \addr
.int (1f - 0f)
0:
.endm

.macro PATCH_REL_JUMP addr
.section .patch, "awx"
.int 0x4
.int \addr
.int (1f - 0f)
0:
.endm

.macro PATCH_WRITE32 value
.section .patch, "awx"
.int 0x5
.int \value
.int (1f - 0f)
0:
.endm

.macro PATCH_OBJECT objectId, offset
.section .patch, "awx"
.int 0x7
.int \objectId
.int \offset
.int (1f - 0f)
0:
.endm

.macro PATCH_VROM addr
.section .patch, "awx"
.int 0x9
.int \addr
.int (1f - 0f)
0:
.endm

.macro PATCH_END
1:
.balign 8
.previous
.endm

.macro HOOK_SAVE
  addiu sp,-0x44
  sw    ra,0x00(sp)
  sw    a0,0x04(sp)
  sw    a1,0x08(sp)
  sw    a2,0x0c(sp)
  sw    a3,0x10(sp)
  sw    t0,0x14(sp)
  sw    t1,0x18(sp)
  sw    t2,0x1c(sp)
  sw    t3,0x20(sp)
  sw    t4,0x24(sp)
  sw    t5,0x28(sp)
  sw    t6,0x2c(sp)
  sw    t7,0x30(sp)
  sw    t8,0x34(sp)
  sw    t9,0x38(sp)
  sw    v0,0x3c(sp)
  sw    v1,0x40(sp)
.endm

.macro HOOK_RESTORE
  lw    ra,0x00(sp)
  lw    a0,0x04(sp)
  lw    a1,0x08(sp)
  lw    a2,0x0c(sp)
  lw    a3,0x10(sp)
  lw    t0,0x14(sp)
  lw    t1,0x18(sp)
  lw    t2,0x1c(sp)
  lw    t3,0x20(sp)
  lw    t4,0x24(sp)
  lw    t5,0x28(sp)
  lw    t6,0x2c(sp)
  lw    t7,0x30(sp)
  lw    t8,0x34(sp)
  lw    t9,0x38(sp)
  lw    v0,0x3c(sp)
  lw    v1,0x40(sp)
  addiu sp,0x44
.endm

.macro HOOK_CALL addr
HOOK_SAVE
  jal \addr
  nop
HOOK_RESTORE
.endm

#endif /* COMBO_ASM_H */
