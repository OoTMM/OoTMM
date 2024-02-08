#include <combo.h>

ItemPreventEmptyEquip:
  sge at,t1,0xff
  beqz at,.Lret
  lbu v0,0x00(s1)
  li  v0,0xff

.Lret:
  jr    ra
   ori   at,zero,9

/* Item Menu - Left Check */
PATCH_START 0x80819d94
  nop
PATCH_END

/* Item Menu - Right Check */
PATCH_START 0x80819e74
  nop
PATCH_END

/* Item Menu - Bottom Check */
PATCH_START 0x8081a1dc
  nop
PATCH_END

/* Item Menu - Top Check */
PATCH_START 0x8081a180
  nop
PATCH_END

/* Item Menu - Disable "To Equip" on empty slots */
PATCH_START 0x8081a268
  jal     ItemPreventEmptyEquip
   addu   s1,t6,t7
PATCH_END

/* Item Menu - Disable equipping items on empty slots */
PATCH_START 0x8081a2f0
  jal     ItemPreventEmptyEquip
   nop
PATCH_END

EquipsPreventEmpty:
  /* Replaced code */
  addu  t8,t4,v0

  /* Load the equip mask at the cursor in t3 */
  addu  t7,v0
  addiu t7,-1
  li    t3,1
  sll   t3,t7

  /* Load the save equips in t7 */
  la  t7,(gSaveContext + 0x74 + 0x28)
  lhu t7,(t7)

  /* Compare */
  and   t7,t3
  beqzl t7,.LEquipsRet
   li   v1,0xff

  /* Replaced code */
  lbu  v1,(t8)

.LEquipsRet:
  jr    ra
   nop

/* Equip menu - left check */
PATCH_START 0x80818494
  nop
PATCH_END

/* Equip menu - right check */
PATCH_START 0x808185bc
  nop
PATCH_END

/* Equip menu - down check */
PATCH_START 0x808187c8
  nop
PATCH_END

/* Equip menu - up check */
PATCH_START 0x8081871c
  nop
PATCH_END

/* Equip menu - prevent equiping non-owned items */
PATCH_START 0x80818da4
  jal EquipsPreventEmpty
   nop
PATCH_END

deepHook_KaleidoSetCursorColor:
  HOOK_CALL KaleidoSetCursorColor
  jr ra
   nop

/* Use our color hook */
PATCH_START(0x8081f85c)
  jal deepHook_KaleidoSetCursorColor
  nop
PATCH_END

PATCH_START(0x8081f86c)
  li ra, 0x3c03c
PATCH_END

PATCH_START(0x8081f8d0)
  nop
PATCH_END

PATCH_START(0x8081f8e4)
  nop
  nop
PATCH_END

PATCH_START(0x8081f928)
  nop
PATCH_END

/* Disable AA and Divot in some cases */
PATCH_START 0x8009c5ac
  nop
PATCH_END

/* Relocs */
PATCH_START 0x8082f624
.int 0
.int 0
.int 0
PATCH_END

PATCH_START 0x8082f68c
.int 0
PATCH_END

PATCH_START 0x8082f694
.int 0
.int 0
PATCH_END

PATCH_START 0x80099F80
  ADDIU   a2, zero, 0x0008
  BEQ     a2, a0, .LstartPause
  ADDIU   at, zero, 0x0001
  BEQ     at, a0, .LcheckLetterbox
  ADDIU   at, zero, PAUSE_STATE_OWLWARP_0
  BNE     at, a0, .LcheckPreReady
  OR      v0, a0, zero
.LcheckLetterbox:
  JAL     0x80099650
  SW      v1, 0x001C (sp)
  LW      v1, 0x001C (sp)
  BNEZ    v0, . + 0x130 // return
.LstartPause:
  ADDIU   a2, zero, 0x0001
  LW      t2, 0x8011BA00
  SH      a2, 0x0190 (t2)
  LHU     t4, 0x01D4 (v1)
  SH      zero, 0x01E4 (v1)
  SH      zero, 0x01EC (v1)
  ADDIU   t5, t4, 0x0001
  B       . + 0x10C // return
  SH      t5, 0x01D4 (v1)
  NOP
  NOP
  NOP
  NOP
  NOP
.LcheckPreReady:
  ADDIU   at, zero, PAUSE_STATE_OWLWARP_1
  BEQ     v0, at, . + 0x1C
  LUI     t6, 0x8012
  ADDIU   at, zero, 0x0002
  BEQ     v0, at, . + 0x10
PATCH_END

KaleidoScope_BeforeUpdate_Hook:
  ADDIU   sp, sp, -0x18
  SW      ra, 0x0014 (sp)
  JAL     KaleidoScope_BeforeUpdate
  NOP
  LA      t0, 0x8011BA00
  LW      ra, 0x0014 (sp)
  JR      ra
  ADDIU   sp, sp, 0x18

PATCH_START 0x808263C8
  OR      s1, a0, zero
  SW      ra, 0x0024 (sp)
  JAL     KaleidoScope_BeforeUpdate_Hook
  NOP
PATCH_END

KaleidoScope_BeforeDraw_Hook:
  ADDIU   sp, sp, -0x18
  SW      ra, 0x0014 (sp)
  SW      t0, 0x0010 (sp)

  JAL     KaleidoScope_BeforeDraw
  OR      a0, s1, zero

  OR      t5, v0, zero
  OR      a0, s0, zero
  LHU     v0, 0x01D4 (s0)

  LW      t0, 0x0010 (sp)
  LW      ra, 0x0014 (sp)
  JR      ra
  ADDIU   sp, sp, 0x18

PATCH_START 0x8039AEC0 + 0x48AC60 // RAM + VRAM Offset
  JAL     KaleidoScope_BeforeDraw_Hook
  NOP
  BNEZ    t5, . + 0xA0
PATCH_END

PATCH_START 0x8009A110
  LBU     v0, 0x0935 (v0)
PATCH_END

PATCH_START 0x80398BE4 + 0x48AC60 // RAM + VRAM Offset
  LBU     t6, 0x0935 (v0)
PATCH_END

PATCH_START 0x80398C00 + 0x48AC60 // RAM + VRAM Offset
  LBU     v0, 0x01d5 (t0)
PATCH_END

PATCH_START 0x80394B6C + 0x48AC60 // RAM + VRAM Offset
  LBU     t8, 0x01d5 (v1)
PATCH_END

/* Fast magical arrows */
PATCH_START 0x8081aaac
  slti at,t8,0
PATCH_END

PATCH_START 0x8081aa94
  li t9,0xff
PATCH_END

PATCH_START 0x8081aaa8
  li t4,0x1
PATCH_END

PATCH_START 0x8081a444
  li t8,0x4
PATCH_END

PATCH_START 0x8081af34
  li v1,0x1
PATCH_END

PATCH_START 0x8081ab28
  li t8,0x5
PATCH_END

/* Fix the issue where adult link has hover boots in the quiver slot when slingshot wasn't obtained */
PreventHoverBootsQuiverSlot:
  and     t6,v1,t5
  bnez    t6,PreventHoverBootsQuiverSlot_Return
   lbu     t4,(t7)
  addiu   ra,0x3c

PreventHoverBootsQuiverSlot_Return:
  jr ra
   nop

PATCH_START 0x808192d0
  jal PreventHoverBootsQuiverSlot
   nop
PATCH_END

/* Use our kaleido icon drawing */
PATCH_START 0x8081a948
  nop
PATCH_END

PATCH_START 0x8081a964
  move a1,t1
  nop
PATCH_END

PATCH_START 0x8081a970
  jal KaleidoScope_DrawItemHook
  sw  t2, 0x0010 (sp)
PATCH_END

/* Relocs */
PATCH_START 0x8082f160
.int 0
PATCH_END

/* Use our item name hook */
PATCH_START 0x80822e6c
  move a1,v1
  nop
  jal KaleidoScope_LoadItemName
PATCH_END
