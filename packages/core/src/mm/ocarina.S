#include <combo.h>

/* Use our own note handler */
PATCH_START 0x8019af50
  j OcarinaMaskButtons
PATCH_END
