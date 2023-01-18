#include <combo.h>

/* Skip hand waving cutscene */
PATCH_VROM 0x028d9000 + 0x1a0f
.byte 0xff
PATCH_END
