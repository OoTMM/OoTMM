#include <combo.h>

/* Fix FD mask item having weird colors sometimes */
PATCH_OBJECT 0x242, 0xae8
.word 0xFC30FE04
.word 0x5FFEF3F8
PATCH_END
