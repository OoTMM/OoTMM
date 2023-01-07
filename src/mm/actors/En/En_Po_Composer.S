#include <combo.h>

/* Use short dialogue */
PATCH_START 0x80bc51b4
  li t4,1
PATCH_END

/* Handle song of storms */
PATCH_START 0x80bc54ec
  jal EnPoComposer_SetHandlerStorms
PATCH_END

/* Remove relocs */
PATCH_START 0x80bc6a48
.int 0
PATCH_END
