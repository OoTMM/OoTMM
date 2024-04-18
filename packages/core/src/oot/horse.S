#include <combo.h>

/* Prevent Epona from being initialized near Lon Lon */
PATCH_START 0x8005a474
  jr ra
   nop
PATCH_END
