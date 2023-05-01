#include <combo.h>
// Makes the item screen default when pressing start the first time
PATCH_START 0x8005B9DC
 b .+0x4c
PATCH_END