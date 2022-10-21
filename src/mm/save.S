#include <combo.h>

.align 4
kMmOwlSaveCellOffsets:
.int SAVE_OFFSET_MM_1 >> 7
.int SAVE_OFFSET_MM_1_BACKUP >> 7
.int SAVE_OFFSET_MM_2 >> 7
.int SAVE_OFFSET_MM_2_BACKUP >> 7
.int SAVE_OFFSET_MM_3 >> 7
.int SAVE_OFFSET_MM_3_BACKUP >> 7

kMmOwlSaveCellSizes:
.int 0x80
.int 0x80
.int 0x80
.int 0x80
.int 0x80
.int 0x80

/* Rebase MM save */
/* TODO: Redo this correctly */
PATCH_START(0x801c67c8)
.int SAVE_OFFSET_MM_1 >> 7
.int SAVE_OFFSET_MM_1_BACKUP >> 7
.int SAVE_OFFSET_MM_2 >> 7
.int SAVE_OFFSET_MM_2_BACKUP >> 7
.int SAVE_OFFSET_MM_1 >> 7
.int SAVE_OFFSET_MM_1_BACKUP >> 7
.int SAVE_OFFSET_MM_2 >> 7
.int SAVE_OFFSET_MM_2_BACKUP >> 7
.int 0x300
.int 0x380
PATCH_END

PATCH_START 0x801c6840
.int SAVE_OFFSET_MM_1 >> 7
.int SAVE_OFFSET_MM_1_BACKUP >> 7
.int SAVE_OFFSET_MM_2 >> 7
.int SAVE_OFFSET_MM_2_BACKUP >> 7
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

/* Always checksum the whole thing */
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
