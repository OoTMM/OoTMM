#include <combo.h>

/* Use a different chest ID for the HP chest */
PATCH_START 0x80ac35b8
  li v1,0x4ec1
PATCH_END
