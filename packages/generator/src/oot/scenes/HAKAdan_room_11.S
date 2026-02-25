#include <combo.h>


/* Fixes the shared flags between the ReDeads and the first Silver Rupee puzzle in the Scythe room of Shadow Temple */
/* #if gComboConfig & (1 << MQ_TEMPLE_SHADOW) */
/* TODO: This cannot work, need to be fixed in make_mq.ts */

#if 0
PATCH_VROM 0x280cdde
.half 0x0002
PATCH_END

PATCH_VROM 0x280cdee
.half 0x0002
PATCH_END
#endif

/* #else */

PATCH_VROM 0x280905e
.half 0x0002
PATCH_END

PATCH_VROM 0x280906e
.half 0x0002
PATCH_END

/* #endif */
