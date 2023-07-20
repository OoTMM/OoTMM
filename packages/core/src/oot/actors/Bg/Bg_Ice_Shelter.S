#include <combo.h>
/* Starts at VRAM 0x80A28E20 - VROM 0xDB2950 */

blue_fire_arrows:
    li      at, 0x0016 # sets at to arrow actor id
    bne     t9, at, return # return if actor isn't an arrow
     li      at, 0x0004 # sets at to arrow type for ice arrows
     jr ra
     lh      t9, 0x1C(v1) # load arrow type from actor - 4 for ice arrows

return:
    jr ra
    li      at, 0x00F0 # set at to blue fire actor id

PATCH_GROUP PG_OOT_BLUE_FIRE_ARROWS
  PATCH_START 0x80A28E20 + 0x978
    jal blue_fire_arrows
  PATCH_END

  PATCH_START 0x80A28E20 + 0xFAE
  .byte 0xEF
  PATCH_END

  PATCH_START 0x80A28E20 + 0xFCB
  .byte 0x50
  PATCH_END

  PATCH_START 0x80A28E20 + 0xFD7
  .byte 0x5A
  PATCH_END
PATCH_GROUP_END
