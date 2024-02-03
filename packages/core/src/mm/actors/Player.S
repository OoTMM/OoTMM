#include <combo.h>

/* Expand the GI buffer */
PATCH_START 0x808420c8
  li a0,0x6000
PATCH_END

/* Force GetItemCollectBehavior return value */
PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d400
  li v0,0xff
PATCH_END

PATCH_START 0x8083d428
  li v0,0xff
PATCH_END

PATCH_START 0x8083d2a0
  li v0,0xff
PATCH_END

/* Never decrement the razor blade health */
PATCH_START 0x80840024
  move t8,v0
PATCH_END

/* Disable first song of time cutscene */
PATCH_START 0x808528c4
  li t8,0x1c00
PATCH_END

/* Ignore some special GI values */
PATCH_START 0x80852da4
  li at,0x7fff
PATCH_END

PATCH_START 0x80852dac
  li at,0x7fff
PATCH_END

PATCH_START 0x80852db4
  li at,0x7fff
PATCH_END

PATCH_START 0x80852dd8
  li at,0x7fff
PATCH_END

/* Sun Mask DrawGiId */
PATCH_START 0x80859760
  li t8,DRAWGI_MM_MASK_SUN
PATCH_END

/* Use our Player_UpdateWrapper */
PATCH_START 0x801d0b68
.int Player_UpdateWrapper
PATCH_END

PATCH_START 0x80123100
  lui t5,%HI(Player_UpdateWrapper)
PATCH_END

PATCH_START 0x8012310c
  addiu t5,%LO(Player_UpdateWrapper)
PATCH_END

/* Ignore the owl save flag when soaring */
PATCH_START 0x80841640
  li t6,0
PATCH_END

/* Don't spawn the soaring cutscene */
PATCH_START 0x80841728
  nop
PATCH_END

PATCH_START 0x808416f4
  nop
  nop
PATCH_END

/* Always reset form on void */
PATCH_START 0x808345e8
  nop
PATCH_END

/* Write to the transition flag */
Player_WriteTransitionFlag:
  /* Replaced code */
  sh t9,-0x7786(at)

  li  t0,1
  lui t9,%HI(gIsEntranceOverride)
  sb  t0,%LO(gIsEntranceOverride)(t9)

  /* Replaced code */
  jr ra
   li t0,0x4

PATCH_START 0x808354d8
  jal Player_WriteTransitionFlag
   nop
PATCH_END

/* Reset deku form on void */
PATCH_START 0x8083bda0
  nop
  nop
  nop
  nop
  nop
PATCH_END

/* Bremen speed */
PATCH_VROM 0xCD8A10
.float 1.0
PATCH_END

/* Don't spawn the cutscene of first time transforming */
PATCH_START 0x808555E4
  nop
PATCH_END

PATCH_GROUP PG_MM_LENIENT_SPIKES

PATCH_START 0x8082da90 + 0x2a95e
.half 0x1000
PATCH_END

PATCH_START 0x8082da90 + 0x2a41e
.half 0x0000
PATCH_END

PATCH_START 0x8082da90 + 0x2a139
.byte 0x19
PATCH_END

PATCH_GROUP_END

// FD spins
PATCH_GROUP PG_MM_FD_ANYWHERE
PATCH_START 0x8082da90 + 0x5880
  nop
PATCH_END
PATCH_GROUP_END

/* Use our handler to draw GI */
PATCH_START 0x80126964
  jal comboPlayerDrawGI
PATCH_END

/* Use our handler to set the draw GI ID */
PATCH_START 0x8082ece0
  j comboPlayerSetDrawGi
   nop
PATCH_END

/* GI renumbering */
PATCH_START 0x8085a878
  li  a2,GI_MM_PENDANT_OF_MEMORIES
PATCH_END

kItemToGi:
.short 0x00
.short 0x00
.short 0x00
.short GI_MM_SWORD_KOKIRI
.short GI_MM_SWORD_RAZOR
.short GI_MM_SWORD_GILDED
.short GI_MM_GREAT_FAIRY_SWORD
.short GI_MM_STICK
.short GI_MM_SWORD_KOKIRI
.short GI_MM_BOW
.short GI_MM_ARROW_FIRE
.short GI_MM_ARROW_ICE
.short GI_MM_ARROW_LIGHT
.short GI_MM_HOOKSHOT
.short GI_MM_BOMB
.short GI_MM_POWDER_KEG
.short GI_MM_BOMBCHU
.short 0x40 /* ??? */
.short GI_MM_NUT
.short GI_MM_PICTOGRAPH_BOX
.short GI_MM_OCARINA_OF_TIME
.short GI_MM_BOTTLE_EMPTY
.short GI_MM_FISH
.short 0x75 /* ??? */
.short 0x76 /* Ice Trap ..? */
.short 0x69 /* Zora egg ..? */
.short 0x6A /* Gold Dust ..? */
.short 0x6C /* ??? */
.short 0x6E /* Seahorse ..? */
.short 0x6B /* Mushroom ..? */
.short 0x70 /* Fake empty bottle ..? */
.short 0x5F /* ??? */
.short GI_MM_BUGS
.short 0x65 /* ??? */
.short 0x66 /* ??? */
.short GI_MM_POTION_RED
.short GI_MM_POTION_BLUE
.short GI_MM_POTION_GREEN
.short 0x61 /* Half milk ..? */
.short 0x61 /* Half milk ..? */
.short GI_MM_CHATEAU
.short GI_MM_FAIRY
.short GI_MM_MOON_TEAR
.short GI_MM_DEED_LAND
.short GI_MM_ROOM_KEY
.short GI_MM_LETTER_TO_KAFEI
.short GI_MM_MAGIC_BEAN
.short GI_MM_DEED_SWAMP
.short GI_MM_DEED_MOUNTAIN
.short GI_MM_DEED_OCEAN
.short GI_MM_MOON_TEAR
.short GI_MM_LETTER_TO_MAMA
.short GI_MM_BOOTS_IRON
.short GI_MM_BOOTS_HOVER
.short GI_MM_PENDANT_OF_MEMORIES
.short GI_MM_SPELL_WIND
.short GI_MM_SPELL_LOVE
.short GI_MM_SPELL_FIRE
.short GI_MM_MASK_TRUTH
.short GI_MM_MASK_KAFEI
.short GI_MM_MASK_ALL_NIGHT
.short GI_MM_MASK_BUNNY
.short GI_MM_MASK_KEATON
.short GI_MM_MASK_GARO
.short GI_MM_MASK_ROMANI
.short GI_MM_MASK_TROUPE_LEADER
.short GI_MM_MASK_POSTMAN
.short GI_MM_MASK_COUPLE
.short GI_MM_MASK_GREAT_FAIRY
.short GI_MM_MASK_GIBDO
.short GI_MM_MASK_DON_GERO
.short GI_MM_MASK_KAMARO
.short GI_MM_MASK_CAPTAIN
.short GI_MM_MASK_STONE
.short GI_MM_MASK_BREMEN
.short GI_MM_MASK_BLAST
.short GI_MM_MASK_SCENTS
.short GI_MM_MASK_GIANT
.short GI_MM_MASK_FIERCE_DEITY
.short GI_MM_MASK_GORON
.short GI_MM_MASK_ZORA
.short GI_MM_MASK_DEKU
.short GI_MM_LENS
.short GI_MM_TUNIC_GORON
.short GI_MM_TUNIC_ZORA
.short 0x00

/*
Replaces:
  SLTI    AT, V1, 0x0053
*/
PATCH_START 0x8085B8A4
  slti    at, v1, PLAYER_CUSTOM_IA_MAX
PATCH_END

PATCH_START 0x80853c38
  nop
PATCH_END

GiFromItem:
  lui t6,%HI(kItemToGi)
  sll t5,1
  addu t6,t5
  jr ra
   lhu t6,%LO(kItemToGi)(t6)

PATCH_START 0x80853c40
  jal GiFromItem
   nop
PATCH_END

PATCH_START 0x80853ab0
  nop
PATCH_END

PATCH_START 0x80853abc
  nop
PATCH_END

GiFromItem2:
  lui t2,%HI(kItemToGi)
  sll v0,1
  addu t2,v0
  jr ra
   lhu t2,%LO(kItemToGi)(t2)

PATCH_START 0x80853b0c
  jal GiFromItem2
PATCH_END

/* Relocs */
PATCH_START 0x80861c44
.int 0
.int 0
PATCH_END

PATCH_START 0x80861c5c
.int 0
.int 0
PATCH_END

/* Magic Spells. TODO PATCH_GROUP ? */

/*
Replaces:
  JR    RA
   NOP
*/
PATCH_START 0x8082F58C
  j     Player_CustomItemToItemAction
   or   a2, v0, zero
PATCH_END

Player_CustomCsItem_Hook:
  addiu     sp, sp, -0x20
  sw        ra, 0x0014 (sp)

  jal       Player_CustomCsItem
   or       a0, s0, zero

  lb        a1, 0x014a (s0)
  lw        ra, 0x0014 (sp)
  jr        ra
  addiu     sp, sp, 0x20

/*
Replaces:
  lb        a1, 0x14a(s0)
  slti      at, a1, 0x3a
  bnez      at, .+0xE0
   slti     at, a1, 0x52
  beqzl     at, .+0xDC
*/
PATCH_START 0x80838b40
  jal       Player_CustomCsItem_Hook
   lw       a1, 0x003c (sp)
  bgtzl     v0, .+0x67C
   li       v0, 1
  beqzl     v0, .+0xDC
PATCH_END

/*
Replaces:
  beq       a2, zero, 0x80831a8c
   or       v1, a2, zero
  slti      v1, s1, 0x52
  beq       v1, zero, 0x80831a8c
   nop
  lbu       v1, 0x14b (s0)
  xori      v1, v1, 0x4
  sltu      v1, zero, v1
  bne       v1, zero, 0x80831a8c
   nop
  slti      v1, s1, 0x4d
  xori      v1, v1, 0x1
*/
PATCH_START 0x80831a5c
  or        a0, s0, zero
  lw        a1, 0x0068 (sp)
  jal       Player_CustomUseItem
   or       a2, s1, zero
  bgtzl     v0, .+0x4BC
   lw        ra, 0x002c (sp)
  sub      v1, zero, v0
  slti     a2, s1, 0x003a
  xori     a2, a2, 0x0001
  nop
  nop
  nop
PATCH_END

/*
Replaces:
  JR      RA
   NOP
*/
PATCH_START 0x80123350
  J       Player_CheckCustomBoots
   or     a0, a3, zero
PATCH_END

/*
;==================================================================================================
; Handle item use while swimming check
;==================================================================================================
*/

Player_ShouldCheckItemUsabilityWhileSwimming_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)

    jal     Player_ShouldCheckItemUsabilityWhileSwimming
     sw     a2, 0x0020 (sp)

    // Displaced code:
    lb      t1, 0x0145 (s0)
    slti    at, t1, 0x0005

    lw      a2, 0x0020 (sp)
    lw      ra, 0x0014 (sp)
    jr      ra
     addiu  sp, sp, 0x18

/*
; Replaces:
;   ADDIU   AT, R0, 0x0050
;   SLL     T0, T9, 4
;   BGEZL   T0, 0x80831A5C
;   XORI    A2, A2, 0x0001
;   BEQL    S1, AT, 0x80831A5C
;   XORI    A2, A2, 0x0001
;   LB      T1, 0x0145 (S0)
;   SLTI    AT, T1, 0x0005
*/
PATCH_START 0x80831A24
    sll     t0, t9, 4
    bgezl   t0, .+0x34
     xori   a2, a2, 0x0001
    or      a0, s0, zero
    jal     Player_ShouldCheckItemUsabilityWhileSwimming_Hook
     or     a1, s1, zero
    beqzl   v0, .+0x20
     xori   a2, a2, 0x0001
PATCH_END

/*
; Replaces:
;   ADDIU   V1, R0, 0x000C // CAM_MODE_SLINGSHOT
*/
PATCH_START 0x808386DC
  addiu   v1, zero, 0x000D // CAM_MODE_BOWARROW
PATCH_END

Player_GetEnvironmentWindSpeed_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x0014 (sp)

  jal     Player_GetEnvironmentWindSpeed
   nop

  mov.s   $f18, $f0
  lui     at, 0x4240
  mtc1    at, $f8
  lw      ra, 0x0014 (sp)
  jr      ra
   addiu  sp, sp, 0x20

/*
Replaces:
  LW      V1, 0x0020 (SP)
  LUI     AT, 0x4240
  MTC1    AT, F8
  LWC1    F18, 0x70B8 (V1)
*/
PATCH_START 0x80844B94
  jal     Player_GetEnvironmentWindSpeed_Hook
   lw     a0, 0x0070 (sp)
  lw      v1, 0x0020 (sp)
  nop
PATCH_END

/*
Replaces:
  LWC1    F6, 0x0B80 (S0)
  C.EQ.S  F0, F6
  NOP
  BC1F    .+0x44
  NOP
  LWC1    F10, 0x0B2C (S0)
  LW      T5, 0x0070 (SP)
  LUI     AT, 0x0001
  C.EQ.S  F0, F10
  ADDU    AT, AT, T5
  BC1F    .+0x28
  NOP
  LWC1    F4, 0x70B8 (AT)
  LUI     AT, 0x4248
  MTC1    AT, F8
  NOP
  C.LE.S  F8, F4
  NOP
  BC1F    .+0x3A0
*/
PATCH_START 0x8084497C
  or      a0, s0, zero
  jal     Player_IsBeingPushed
   lw     a1, 0x0070 (sp)
  beqz    v0, .+0x3DC
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

/*
Allow player to pick up heart pieces underwater by changing
this->currentBoots == PLAYER_BOOTS_ZORA_UNDERWATER
to
this->currentBoots >= PLAYER_BOOTS_ZORA_UNDERWATER
Replaces:
  ADDIU   AT, R0, 0x0005
  BNEL    T5, AT, .+0x60
*/
PATCH_START 0x8083D2FC
  slti    at, t5, 0x0005
  bnezl   at, .+0x60
PATCH_END

PATCH_START 0x8083EA48
  sw      s0, 0x0018 (sp)
  or      s0, a0, zero
  sw      ra, 0x001c (sp)
  sw      a1, 0x002c (sp)
  jal     Player_PlayHoverSound
   addiu  a1, sp, 0x002c
  bnez    v0, .+0xF4
   lwc1   $f14, 0x002c (sp)
  lui     a2, 0x41e8
  lui     a3, 0x4120
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

Player_CheckHoverBoots_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x0014 (sp)

  jal     Player_CheckHoverBoots
   nop

  // Displaced code
  lh      t9, 0x0AD4 (s0)
  lh      t1, 0x00BE (s0)
  or      a1, s0, zero

  lw      ra, 0x0014 (sp)
  jr      ra
   addiu  sp, sp, 0x20

PATCH_START 0x80838404
  jal   Player_CheckHoverBoots_Hook
   or   a0, s0, zero
  bnez  v0, .+0x26C
   lw   a0, 0x006C (sp)
PATCH_END

/*
Replaces:
  lhu       t6,0x90(s0)
  lui       t8,0x8086
  andi      t7,t6,0x1
  beql      t7,zero,0x80844950
   lwc1     f8,0xad0(s0)
  lw        t8,0x2b08(t8)
  li        at,0x5
  bnel      t8,at,0x80844950
   lwc1     f8,0xad0(s0)
  lb        t9,0x145(s0)
  lui       a2,0x3eb3
  ori       a2,a2,0x3333
  slti      at,t9,0x5
  beq       at,zero,0x8084494c
*/
PATCH_START 0x80844798
  nop
  lui       a1, 0x8086
  nop
  or        a0, s0, zero
  jal       Player_IsFloorSlippery
   lw       a1, 0x2b08(a1)
  beqz      v0, .+0x19C
   nop
  nop
  nop
  lui       a2,0x3eb3
  ori       a2,a2,0x3333
  nop
  nop
PATCH_END

/*
For checking how long should the player be on fire for.
Replaces:
  bne       v0, at, .+0x10
   nop
  b         .+0x28
*/
PATCH_START 0x808342a8
  jal       Player_IsGoronOrGoronTunic
   or       a0, s4, zero
  bnezl     v0, .+0x28
PATCH_END

/*
During check if player should burn when dying.
Replaces:
  LBU       t6, 0x014B (S0)
  ADDIU     AT, R0, 0x0001
  BEQ       T6, AT, .+0x80
*/
PATCH_START 0x8084BFF0
  jal       Player_IsGoronOrGoronTunic
   or       a0, s0, zero
  bnez      v0, .+0x80
PATCH_END

Player_ShouldTakeFloorDamage_Hook:
  addiu     sp, sp, -0x20
  sw        ra, 0x0014 (sp)
  sw        v1, 0x0018 (sp)
  sw        a1, 0x0024 (sp)
  or        a2, a0, zero

  jal       Player_ShouldTakeFloorDamage
   or       a0, s0, zero

  lw        a1, 0x0024 (sp)
  lw        v1, 0x0018 (sp)
  lw        ra, 0x0014 (sp)
  jr        ra
   addiu    sp, sp, 0x20

/*
Timer for lava when wearing Goron Tunic.
Replaces:
  ADDIU     AT, R0, 0x0032
  BNE       V1, AT, .+0x10
  SB        T1, 0x00B8 (S0)
  B         .+0xC
  OR        A2, R0, R0
  ADDIU     A2, R0, 0x0001
  BEQZ      A1, .+0x10
   LW       A0, 0x007C (SP)
  B         .+0xC
   LH       V1, 0x0086 (S0)
  LH        V1, 0x00BE (S0)
*/
PATCH_START 0x80834C58
  jal       Player_ShouldTakeFloorDamage_Hook
   lw       a0, 0x0058 (sp)
  beqz      v0, .+0x50
   addiu    at, zero, 0x0032
  bnel      v1, at, .+0xC
   addiu    a2, zero, 0x0001
  or        a2, zero, zero
  beqzl     a1, .+0xC
   lh       v1, 0x00BE (s0)
  lh        v1, 0x0086 (s0)
  lw        a0, 0x007C (sp)
PATCH_END

/*
Replaces:
 lui     t8, 0xDE00
 lui     t2, 0x801C
 lw      v0, 0x02B0 (t5)
 addiu   t6, v0, 0x0008
 sw      t6, 0x02B0 (t5)
 sw      t8, 0x0000 (v0)
 lw      t9, 0x0154 (sp)
 sll     t7, t9, 2
 addiu   t2, t2, t7
 lw      t2, 0x0B20 (t2)
 sw      t2, 0x0004 (v0)
*/
PATCH_START 0x801296F4
  ori     a0, t5, 0x0000 // gfxctx
  lw      a1, 0x0154 (sp) // mask ID - 1
  lui     a2, 0x801C
  ori     a2, a2, 0x0B20 // mask DLs
  jal     Player_ColorAfterMask
   or     a3, s1, zero
  nop
  nop
  nop
  nop
  nop
PATCH_END

Player_CanSurface_Hook:
  HOOK_SAVE
  jal       Player_CanSurface
   nop
  sw        v0,0x40(sp) // store into HOOK_SAVE's v0
  HOOK_RESTORE
  jr        ra
   nop

/*
Replaces:
  lw      t9, 0x0a6c (a3)
  lw      t8, 0x0038 (sp)
  sll     t6, t9, 4
  bgezl   t6, .+0x1c
*/
PATCH_START 0x80842D60
  lw      t8, 0x0038 (sp)
  jal     Player_CanSurface_Hook
   or     a0, a3, zero
  beqzl   v0, .+0x1c
PATCH_END

/* Change child link object to use EnvColor instead of PrimColor */

PATCH_VROM 0x01166390
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x011668B8
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01166DC0
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01166F98
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01167058
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01167660
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01167AD8
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01167D10
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01167E98
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x011681E0
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x01168368
.int 0xFC12FE04
.int 0xFFFFFDFE
PATCH_END

PATCH_VROM 0x011711D8
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x011711E8
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171280
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171290
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171330
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171340
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x011713E0
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x011713F0
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171618
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171628
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171708
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171718
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x011717E0
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x011717F0
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x011718D0
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x011718E0
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171960
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171970
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171C98
.int 0xFC327604
.int 0xFFFCFEF8
.int 0xFA0000FF
.int 0x181840FF
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171D68
.int 0xFC327604
.int 0xFFFCFFF8
.int 0xFA0000FF
.int 0x3C3C60FF
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171E20
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171E30
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171F48
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171F58
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01171FD8
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01171FE8
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x011720A0
.int 0xFC327604
.int 0xFFFCFFF8
.int 0xFA0000FF
.int 0x3C3C60FF
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01172140
.int 0xFC327604
.int 0xFFFCFFF8
.int 0xFA0000FF
.int 0x3C3C60FF
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x011721E0
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x011721F0
.int 0x00000000
.int 0x00000000
PATCH_END

PATCH_VROM 0x01172280
.int 0xFC327604
.int 0xFFFCFFF8
PATCH_END

PATCH_VROM 0x01172290
.int 0x00000000
.int 0x00000000
PATCH_END

