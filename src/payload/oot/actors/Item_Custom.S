#include <combo/asm.h>

/* Inject out actor at index 1 */
PATCH_START 0x800e8564
.int gActorInit_ItemCustom
PATCH_END
