#include <combo.h>

/* Skip keeta cutscene */
PATCH_VROM 0x027f1000 + 0x1ff3
.byte 0xff
PATCH_END

/* Change post-keeta entrance to have link stand in place */
PATCH_VROM 0x027f1000 + 0xa4 + 5 * 0x10 + 0xe
.short 0x0bff
PATCH_END
