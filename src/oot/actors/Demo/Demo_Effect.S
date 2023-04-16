#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x8092b420
  li t5,(DRAWGI_OOT_MEDALLION_FIRE - 1)
PATCH_END

PATCH_START 0x8092b434
  li t6,(DRAWGI_OOT_MEDALLION_WATER - 1)
PATCH_END

PATCH_START 0x8092b448
  li t7,(DRAWGI_OOT_MEDALLION_FOREST - 1)
PATCH_END

PATCH_START 0x8092b45c
  li t8,(DRAWGI_OOT_MEDALLION_SPIRIT - 1)
PATCH_END

PATCH_START 0x8092b470
  li t9,(DRAWGI_OOT_MEDALLION_SHADOW - 1)
PATCH_END

PATCH_START 0x8092b484
  li t0,(DRAWGI_OOT_MEDALLION_LIGHT - 1)
PATCH_END

PATCH_START 0x8092b498
  li t1,(DRAWGI_ARROW_LIGHT - 1)
PATCH_END

/* Use our own draw wrapper */
PATCH_START 0x8092ad3c
  la t6, DemoEffect_DrawSpiritualLoader
PATCH_END

/* Check for big octo clear to delete the stone in jabu */
CheckSpiritualStoneJabu:
  /* Replaced code (checks for ruto event) */
  lhu   t8,-0x4b10(t8)
  andi  t9,t8,0x20

  /* Check for big octo clear */
  la    t8,(gOotSave + 0xd4 + 0x1c * SCE_OOT_INSIDE_JABU_JABU + 0x08)
  lw    t8,(t8)
  andi  t8,(1 << 6)

  jr    ra
    or  t9,t8

PATCH_START 0x8092b5bc
  jal CheckSpiritualStoneJabu
   nop
PATCH_END

/* Relocs */
PATCH_START 0x80930364
.int 0
.int 0
PATCH_END
