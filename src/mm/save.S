#include <combo.h>

/* Rebase MM save */
PATCH_START 0x801c67c8
.int SAVE_OFFSET_MM_1 >> 7
.int SAVE_OFFSET_MM_1_BACKUP >> 7
.int SAVE_OFFSET_MM_2 >> 7
.int SAVE_OFFSET_MM_2_BACKUP >> 7
.int SAVE_OFFSET_MM_3 >> 7
.int SAVE_OFFSET_MM_3_BACKUP >> 7
PATCH_END

/* Disable Owl Save reset */
PATCH_START 0x80147314
  jr ra
   nop
PATCH_END

/* Patch file save size to always match owl save size */
PATCH_START 0x801c6870
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
.int 0x3ca0
PATCH_END

PATCH_START 0x801c67f0
.int 0x80
.int 0x80
.int 0x80
.int 0x80
PATCH_END

/* Normal and Owl saves are the same */
PATCH_START 0x80144f18
  move v1,v0
PATCH_END

PATCH_START 0x80146518
  slti at,v1,3
PATCH_END

/* TODO: Change that */
PATCH_START 0x80147414
  jr ra
   nop
PATCH_END

/* Always checksum the whole thing */
PATCH_START 0x80145490
  nop
PATCH_END

PATCH_START 0x80145648
  li a1,0x3ca0
PATCH_END

PATCH_START 0x8014565c
  li a1,0x3ca0
PATCH_END

PATCH_START 0x80145d64
  li a1,0x3ca0
PATCH_END

PATCH_START 0x80146bac
  li a1,0x3ca0
PATCH_END

/* Hook after save load */
PATCH_START 0x80145464
  j Sram_AfterOpenSave
PATCH_END

/* Nop the original save func */
PATCH_START 0x80146ff0
  nop
PATCH_END

PATCH_START 0x801584a0
  nop
PATCH_END

PATCH_START 0x801585a4
  nop
PATCH_END

PATCH_START 0x80829264
  nop
PATCH_END

PATCH_START 0x80829f54
  nop
PATCH_END

PATCH_START 0x80146f44
  nop
PATCH_END

/* Do not reset chests on cycle */
PATCH_START 0x80143ba8
  nop
PATCH_END

PATCH_START 0x80143c2c
  nop
PATCH_END

PATCH_START 0x80143cb8
  nop
PATCH_END

PATCH_START 0x80143d04
  nop
PATCH_END

/* Do not reset gold skulltula tokens */
PATCH_START 0x8014449c
  nop
PATCH_END

PATCH_START 0x801444a4
  nop
PATCH_END

/* Do not remove fierce deity */
PATCH_START 0x80143d98
  nop
PATCH_END

/* Do not reset perm unk_14/room (will be done later) */
PATCH_START 0x80143c58
  nop
PATCH_END

PATCH_START 0x80143c78
  nop
PATCH_END

PATCH_START 0x80143cc8
  nop
  nop
PATCH_END

PATCH_START 0x80143d14
  nop
  nop
PATCH_END

/* Skip razor sword check */
PATCH_START 0x80144174
  li at,0
PATCH_END

/* Skip bottle check */
PATCH_START 0x80144068
  li at,1
PATCH_END

/* Leave trade items alone */
PATCH_START 0x80144394
  nop
PATCH_END

PATCH_START 0x801443a0
  nop
PATCH_END

PATCH_START 0x801443ac
  nop
PATCH_END

/* Disable B/C buttons trade item removal */
PATCH_START 0x80144470
  nop
PATCH_END

PATCH_START 0x80144474
  nop
PATCH_END

PATCH_START 0x80144478
  nop
PATCH_END

/* Do not reset dungeon keys/fairies */
PATCH_START 0x801444fc
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

PATCH_START 0x8014452c
  nop
PATCH_END

/* Disable reset of dungeons */
PATCH_START 0x801c5fc0 + 0x10 * SCE_MM_TEMPLE_WOODFALL
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
PATCH_END

PATCH_START 0x801c5fc0 + 0x10 * SCE_MM_TEMPLE_SNOWHEAD
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
PATCH_END

PATCH_START 0x801c5fc0 + 0x10 * SCE_MM_TEMPLE_GREAT_BAY
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
PATCH_END

PATCH_START 0x801c5fc0 + 0x10 * SCE_MM_TEMPLE_STONE_TOWER
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
PATCH_END

PATCH_START 0x801c5fc0 + 0x10 * SCE_MM_TEMPLE_STONE_TOWER_INVERTED
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
.int 0xffffffff
PATCH_END

/* Disable save options write */
PATCH_START 0x8080a518
  nop
PATCH_END

PATCH_START 0x8080a520
  nop
PATCH_END

/* Make the town stray fairy a permanent item */
PATCH_START 0x801c66e0
.short 0xcc00
PATCH_END
