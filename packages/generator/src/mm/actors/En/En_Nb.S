#include <combo.h>

/* Change the heart piece IDs to 0x01 and 0x02 to be able to dinstinguish them */
PATCH_START 0x80bc1518
.short 0x0002
PATCH_END

PATCH_START 0x80bc1553
.short 0x0001
PATCH_END
