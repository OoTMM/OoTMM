#include <combo.h>

/* Silver rupee shuffle */
PATCH_GROUP PG_OOT_SILVER_RUPEE_SHUFFLE

PATCH_START 0x80a707e4
  move a1,s0
PATCH_END

PATCH_START 0x80a707e8
  jal EnGSwitch_AlreadyTaken
PATCH_END

PATCH_START 0x80a707d4
  la t7,EnGSwitch_DrawSilverRupee
PATCH_END

PATCH_START 0x80a70d04
  move a0,a2
PATCH_END

PATCH_START 0x80a70d18
  nop
  nop
  jal EnGSwitch_GiveItemSilverRupee
PATCH_END

PATCH_START 0x80a70d9c
  nop
PATCH_END

/* Relocs */
PATCH_START 0x80a71dec
.int 0
.int 0
PATCH_END

PATCH_GROUP_END
