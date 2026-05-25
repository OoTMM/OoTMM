#include <combo.h>

/*
 * Allow Fairy Ocarina for all transformation forms.
 *
 * Table:
 *   u8 gPlayerFormItemRestrictions[PLAYER_FORM_MAX][114]
 *
 * Base address:
 *   gPlayerFormItemRestrictions = 0x801C2410
 *
 * Fairy Ocarina index:
 *   ITEM_MM_OCARINA_FAIRY = 5
 *
 * Offset formula:
 *   form * 114 + 5
 *
 * Offsets:
 *   Fierce Deity = 0x005
 *   Goron        = 0x077
 *   Zora         = 0x0e9
 *   Deku         = 0x15b
 */

/* PLAYER_FORM_FIERCE_DEITY: allow ITEM_OCARINA_FAIRY */
PATCH_START 0x801C2410 + 0x005
  .byte 0x01
PATCH_END

/* PLAYER_FORM_GORON: allow ITEM_OCARINA_FAIRY */
PATCH_START 0x801C2410 + 0x077
  .byte 0x01
PATCH_END

/* PLAYER_FORM_ZORA: allow ITEM_OCARINA_FAIRY */
PATCH_START 0x801C2410 + 0x0e9
  .byte 0x01
PATCH_END

/* PLAYER_FORM_DEKU: allow ITEM_OCARINA_FAIRY */
PATCH_START 0x801C2410 + 0x15b
  .byte 0x01
PATCH_END