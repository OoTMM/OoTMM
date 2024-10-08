#include <combo.h>
#include <combo/actor_id.h>

PATCH_START gActorOvl + 0x20 * AC_CUSTOM_TRIGGERS + 0x14
.int CustomTriggers_gActorProfile
PATCH_END
