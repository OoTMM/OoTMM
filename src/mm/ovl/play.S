#include <combo.h>

/* Hook into Play_Init */
PATCH_START(0x801bd9b8)
.int hookPlay_Init
PATCH_END
