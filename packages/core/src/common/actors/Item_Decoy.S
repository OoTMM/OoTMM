#include <combo.h>
#include <combo/actor_id.h>

/* Custom actor */
PATCH_START gActorOvl + 0x20 * AC_ITEM_DECOY + 0x14
.int ItemDecoy_gActorProfile
PATCH_END
