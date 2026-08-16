#include <combo.h>

/* Hook into out menu handler */
KaleidoScope_AfterSetCutsorColor_Trampoline:
  lw      a0,0x40(sp)
  lw      s0,0x14(sp)
  lw      s1,0x18(sp)
  j       KaleidoScope_AfterSetCutsorColor
   addiu  sp,0x40

PATCH_START 0x8081c674
  j KaleidoScope_AfterSetCutsorColor_Trampoline
   nop
PATCH_END

KaleidoScope_AfterSetCutsorColor_Mask_Trampoline:
  lw      a0, 0x50(sp)
  lw      s0, 0x18(sp)
  j       KaleidoScope_AfterSetCutsorColor
   addiu  sp, sp, 0x50

PATCH_START 0x80820F94
  j KaleidoScope_AfterSetCutsorColor_Mask_Trampoline
   nop
PATCH_END

/* Relocs */
PATCH_START 0x8082cea4
.int 0
.int 0
PATCH_END

PATCH_START 0x8082cf50
.int 0
.int 0
PATCH_END

PATCH_START 0x8082d8f8
.int 0
PATCH_END

PATCH_START 0x8082d8fc
.int 0
PATCH_END

PATCH_START 0x8082c948
.int 0
PATCH_END

/* Hide dummy sun song from quest screen */
KaleidoScope_HideDummySunSong:
  lui at,0x0002
  not at
  and s3,at
  li at,0x1
  jr ra
    and t8,v1,s3

PATCH_START 0x80816878
  jal  KaleidoScope_HideDummySunSong
   nop
PATCH_END

/*
==================================================================================================
 Handle OoT Hookshot name
==================================================================================================
*/

PATCH_START 0x80824818
  jal KaleidoScope_LoadNamedItemCustom
PATCH_END

PATCH_START 0x808160A0 + 0x1703c
.int 0
PATCH_END

PATCH_START 0x8081c604
  jal   KaleidoScope_ShowItemMessage
PATCH_END

PATCH_START 0x8081c5e4
  jal   KaleidoScope_ShowItemMessage
PATCH_END

PATCH_START 0x80820F00
  jal KaleidoScope_ShowItemMessage
PATCH_END

PATCH_START 0x80820F20
  jal KaleidoScope_ShowItemMessage
PATCH_END

KaleidoScope_LoadIcons_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x0014 (sp)

  jal     KaleidoScope_LoadIcons
   addiu  a2, sp, 0x4c

  lw      ra, 0x0014 (sp)
  jr      ra
  addiu   sp, sp, 0x20

/*
Replaces:
  jal   CmpDma_LoadAllFiles
*/
PATCH_START 0x80828ac4
  jal   KaleidoScope_LoadIcons_Hook
PATCH_END

/*
==================================================================================================
 Pause Menu (Select Item subscreen) - Icon Draw
==================================================================================================
*/

KaleidoScope_DrawIconCustom_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x001C (sp)

  // Parameter 5 (inventory slot index)
  sw      s1, 0x0010 (sp)

  // Parameter 6 (vertices index for Quadrangle)
  lw      at, 0x0030 (sp)
  sw      at, 0x0014 (sp)

  // Parameter 7 (Vertex buffer index)
  sw      s2, 0x0018 (sp)

  jal     KaleidoScope_DrawIconCustom
  // Overwrite A1 with item Id in V0 (we can calculate A1 with it later)
   or     a1, v0, zero

  lw      ra, 0x001C (sp)

  jr      ra
   addiu  sp, sp, 0x20

/*
Hook draw for item textures on "Select Item" screen.
Replaces:
  jal     KaleidoScope_DrawTexQuadRGBA32
*/
PATCH_START 0x8081BB70
  jal     KaleidoScope_DrawIconCustom_Hook
PATCH_END

/* Remove relocations. */
PATCH_START 0x8082C944
.int 0x00000000 // Removes: 0x44005AD0
PATCH_END

/*
==================================================================================================
 Pause Menu (Select Mask subscreen) - Icon Draw
==================================================================================================
*/

KaleidoScope_DrawMaskCustom_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x001C (sp)

  // Parameter 5: unused by KaleidoScope_DrawMaskIconCustom,
  // since it derives the mask slot from vertIdx.
  sw      zero, 0x0010 (sp)

  // Parameter 6: vertices index for Quadrangle
  lw      at, 0x0030 (sp)
  sw      at, 0x0014 (sp)

  // Parameter 7: vertex buffer index
  sw      s4, 0x0018 (sp)

  jal     KaleidoScope_DrawMaskIconCustom
   or     a1, v0, zero

  lw      ra, 0x001C (sp)

  jr      ra
   addiu  sp, sp, 0x20

/*
==================================================================================================
 Pause Menu (Select Mask subscreen) - Moon-given mask cursor fix
==================================================================================================
*/

KaleidoScope_MaskCursorMoonItem_Trampoline:
  addiu   sp, sp, -0x20
  sw      ra, 0x001C(sp)
  sw      t2, 0x0018(sp)
  sw      t3, 0x0014(sp)
  sw      t4, 0x0010(sp)

  lhu     a0, 0x005E(sp)
  or      a1, v0, zero

  jal     KaleidoScope_ResolveMoonMaskCursorItem
   nop

  sh      v0, 0x0060(sp)

  lw      t4, 0x0010(sp)
  lw      t3, 0x0014(sp)
  lw      t2, 0x0018(sp)
  lw      ra, 0x001C(sp)

  li      t5, 999

  jr      ra
   addiu  sp, sp, 0x20

/*
Hook draw for item textures on "Select Mask" screen.
Replaces:
  jal     KaleidoScope_DrawTexQuadRGBA32
*/
PATCH_START 0x80820458
  jal     KaleidoScope_DrawMaskCustom_Hook
PATCH_END

/* Remove relocations. */
PATCH_START 0x8082cc34
.int 0x00000000 /* Removes: 0x4400a3b8 */
PATCH_END

/*
 * Draw path: don't skip whole slot.
 */
PATCH_START 0x808201ec
  nop
  nop
PATCH_END

/*
 * Horizontal cursor move: don't overwrite candidate item with ITEM_NONE.
 */
PATCH_START 0x80820770
  nop
  nop
PATCH_END

/*
 * Left-page special-position scan: don't skip whole slot.
 */
PATCH_START 0x808207fc
  nop
  nop
PATCH_END

/*
 * Right-page special-position scan: don't skip whole slot.
 */
PATCH_START 0x808208ec
  nop
  nop
PATCH_END

/*
 * Up/down / refresh path 1: keep branch, remove ITEM_NONE store.
 */
PATCH_START 0x80820a98
  nop
PATCH_END

/*
 * Up/down / refresh path 2: remove ITEM_NONE store.
 */
PATCH_START 0x80820adc
  nop
PATCH_END

/*
 * Resolve final cursor item.
 */
PATCH_START 0x80820ae0
  jal     KaleidoScope_MaskCursorMoonItem_Trampoline
   lhu    v0, 0x0040(sp)
PATCH_END

/* Fast magical arrow */

PATCH_START 0x8081c6d0
  slti at, t8, 0
PATCH_END

PATCH_START 0x8081c6b8
  li t9, 0xff
PATCH_END

PATCH_START 0x8081c6cc
  li t4, 0x1
PATCH_END

PATCH_START 0x8081cb10
  li v1, 0x1
PATCH_END

PATCH_START 0x8081c744
  li t8, 0x5
PATCH_END

PATCH_START 0x80163bc4
  jal   KaleidoScope_UpdateWrapper
   move a1, t9
PATCH_END

PATCH_START 0x808240d0
  li    t9, -0x2b
PATCH_END

PATCH_START 0x80824114
  addiu   v1, v1, 0x16
PATCH_END

PATCH_START 0x80824134
  addiu   v1, v1, 0x40
PATCH_END

PATCH_START 0x80824150
  li    t7, 0x800
PATCH_END

PATCH_START 0x808241e4
  li    a3, 0x40
PATCH_END

PATCH_START 0x808219d4
  nop
PATCH_END

PATCH_START 0x80821a50
  nop
PATCH_END

PATCH_START 0x80821d54
  nop
PATCH_END

PATCH_START 0x808288c4
  nop
PATCH_END

/*
  Long series of patches for shifting magic arrow unique select item ID to allow more custom items
*/

PATCH_START 0x8081B9AA
  .short (0x017A - ((ITEM_MM_CUSTOM_MAX - 0xB5)) * 2) 
PATCH_END

PATCH_START 0x8081B9AE
  .short (0x017A - ((ITEM_MM_CUSTOM_MAX - 0xB5)) * 2) 
PATCH_END

PATCH_START 0x8081B9B6
  .short (0x017A - ((ITEM_MM_CUSTOM_MAX - 0xB5)) * 2) 
PATCH_END

PATCH_START 0x8081C536
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081C92E
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CC0A
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CC82
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CDBA
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CE5A
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CED2
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CFEE
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081D08E
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081D106
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081CB6A
  .short ITEM_MM_CUSTOM_MAX
PATCH_END

PATCH_START 0x8081C73A
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CC32
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CC9E
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CDE2
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CE82
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CEEE
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081D016
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081D0B6
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081D122
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CB92
  .short (0 - (ITEM_MM_CUSTOM_MAX - ITEM_MM_BOW_ARROW_FIRE))
PATCH_END

PATCH_START 0x8081CC12
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CC8E
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CDC2
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CE62
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CEDE
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CFF6
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081D096
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081D112
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

PATCH_START 0x8081CB72
  .short ITEM_MM_CUSTOM_MAX + 3
PATCH_END

/*
==================================================================================================
 MM item equip age gate
==================================================================================================
*/

KaleidoScope_MmEquipAgeGate_ButtonMask:
  addiu   sp, sp, -0x20
  sw      ra, 0x001c(sp)
  sw      a0, 0x0018(sp)
  sw      t1, 0x0014(sp)
  sw      s1, 0x0010(sp)

  andi    t5, a0, 0x0007
  beqz    t5, .L_mm_agegate_restore
   nop

  jal     KaleidoScope_CheckMmSlotAgeReq
   lhu    a0, 0x0268(s0)

  bnez    v0, .L_mm_agegate_allow
   nop

  jal     Audio_PlaySfx
   li     a0, 0x4806

  b       .L_mm_agegate_block
   nop

.L_mm_agegate_allow:
  li      t5, 0x1
  b       .L_mm_agegate_restore
   nop

.L_mm_agegate_block:
  move    t5, zero

.L_mm_agegate_restore:
  lw      s1, 0x0010(sp)
  lw      t1, 0x0014(sp)
  lw      a0, 0x0018(sp)
  lw      ra, 0x001c(sp)
  jr      ra
   addiu  sp, sp, 0x20

PATCH_START 0x8081c2a0
  jal     KaleidoScope_MmEquipAgeGate_ButtonMask
   nop
  .int    0x11a000b0
PATCH_END

/*
==================================================================================================
 MM mask equip age gate
==================================================================================================
*/

KaleidoScope_MmMaskEquipAgeGate_Accept:
  addiu   sp, sp, -0x30

  sw      ra, 0x002c(sp)
  sw      a0, 0x0028(sp)
  sw      v0, 0x0024(sp)
  sw      v1, 0x0020(sp)
  sw      s0, 0x001c(sp)
  sw      t4, 0x0018(sp)

  jal     KaleidoScope_CheckMmSlotAgeReq
   addiu  a0, v1, 0x0018

  sw      v0, 0x0014(sp)

  lw      t4, 0x0018(sp)
  lw      s0, 0x001c(sp)
  lw      v1, 0x0020(sp)
  lw      v0, 0x0024(sp)
  lw      a0, 0x0028(sp)
  lw      ra, 0x002c(sp)

  lhu     t5, 0x0070(sp)
  lw      t6, 0x019c(s0)
  addiu   t8, v1, 0x0018
  sll     t7, v1, 0x2

  lw      t9, 0x0014(sp)

  jr      ra
   addiu  sp, sp, 0x30

PATCH_START 0x80820e0c
  jal     KaleidoScope_MmMaskEquipAgeGate_Accept
   nop
  .int    0x1320ff8e
  nop
PATCH_END

/*
==================================================================================================
 MM native icon age greying
==================================================================================================
*/

KaleidoScope_NativeItemAgeAndFormCheck_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x001c(sp)
  sw      a0, 0x0018(sp)
  sw      a2, 0x0014(sp)
  sw      v0, 0x0010(sp)

  lbu     t9, 0x2410(t9)

  beqz    t9, .L_native_age_form_restore
   nop

  jal     KaleidoScope_CheckMmItemAgeReq
   andi   a0, a2, 0xffff

  move    t9, v0

.L_native_age_form_restore:
  lw      v0, 0x0010(sp)
  lw      a2, 0x0014(sp)
  lw      a0, 0x0018(sp)
  lw      ra, 0x001c(sp)
  jr      ra
   addiu  sp, sp, 0x20

PATCH_START 0x80828b14
  jal     KaleidoScope_NativeItemAgeAndFormCheck_Hook
PATCH_END