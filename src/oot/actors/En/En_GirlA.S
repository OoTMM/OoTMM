#include <combo.h>

/* Use the shop variants of the shields */
PATCH_START 0x80864b9c
.int GI_OOT_SHOP_SHIELD_DEKU
PATCH_END

PATCH_START 0x80864b7c
.int GI_OOT_SHOP_SHIELD_HYLIAN
PATCH_END
