#include <combo/asm.h>

#if 0
PATCH_START 0x80ab829c
  li t9,0x8a5
PATCH_END

PATCH_START 0x80ab7f04
  li t0,0x8a5
PATCH_END

PATCH_START 0x80ab82f0
  nop
PATCH_END

/* DEBUG */
PATCH_START 0x80ab828c
  li v0,1
PATCH_END

PATCH_START 0x80ab82c4
  nop
PATCH_END
#endif

PATCH_START 0x80ab76b0
  j EnMnk_SongCutscene
   nop
PATCH_END
