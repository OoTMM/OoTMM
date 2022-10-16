#include <combo.h>

/* Inject out actor at index 3 */
PATCH_START 0x800e85a4
.int ItemGiver_gActorInit
PATCH_END
