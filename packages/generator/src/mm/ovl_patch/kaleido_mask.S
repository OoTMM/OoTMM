#include <combo.h>
/* Vanilla MM hides Moon Child masks by visual slot. Stone/Gibdo are shared
 * slots here, so disabling these slot bits lets our custom shared-mask logic
 * decide which specific mask variant is removed or shown. */
PATCH_START 0x8082b68a
  .short 0
PATCH_END

PATCH_START 0x8082b6aa
  .short 0
PATCH_END