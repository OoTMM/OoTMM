#include <combo/asm.h>

/* Inject out actor at index 0x13 */
PATCH_START 0x801af244
.int gActorInit_ItemCustom
PATCH_END
