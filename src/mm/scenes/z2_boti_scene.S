#include <combo.h>

/* Skip keeta cutscene */
PATCH_VROM 0x027f1000 + 0x1ff3
.byte 0xff
PATCH_END
