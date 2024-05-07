#include <combo.h>

/* Change the weird void into a normal void */
PATCH_VROM 0x02c09000 + 0x1144
.int 0x00000001
.int 0x000017ce
.int 0x30000001
.int 0x000017CE
PATCH_END
