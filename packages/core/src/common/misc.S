#include <combo.h>

/*
 * Set the values for wallet capacity.
 * No wallet = 0, child wallet is handled separately
 */

PATCH_START gMaxRupees
.short 0
.short 200
.short 500
.short 999
PATCH_END
