#include <combo.h>

/* Make small keys use the same draw function as heart pieces */
PATCH_START 0x80105fc8
.int 0x80013014
PATCH_END
