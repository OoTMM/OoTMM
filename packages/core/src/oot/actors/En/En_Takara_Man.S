#include <combo.h>

PATCH_GROUP PG_OOT_CHEST_GAME_SHUFFLE

/* Don't reset the chest flags */
PATCH_START 0x80b1901c
  nop
PATCH_END

/* Don't reset the key count */
PATCH_START 0x80b19030
  nop
PATCH_END

/* Set the custom flag */
PATCH_START 0x80b194b8
  jal EnTakaraMan_HasGivenItem
PATCH_END

/* Give the custom check */
PATCH_START 0x80b19418
  jal EnTakaraMan_GiveItem
PATCH_END

PATCH_START 0x80b194ec
  jal EnTakaraMan_GiveItem
PATCH_END

/* Use our custom handler */
PATCH_START 0x80b19210
  lui t8,%HI(EnTakaraMan_InitHandler)
PATCH_END

PATCH_START 0x80b1921c
  addiu t8,%LO(EnTakaraMan_InitHandler)
PATCH_END

/* Use our custom handler (reloc) */
PATCH_START 0x80b1981c
.int 0
.int 0
PATCH_END

PATCH_GROUP_END
