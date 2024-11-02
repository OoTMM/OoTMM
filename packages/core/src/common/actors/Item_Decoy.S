#include <combo.h>
#include <combo/actor_id.h>

/* Custom actor */
PATCH_START gActorOvl + 0x20 * ACTOR_ITEM_DECOY + 0x14
.int ItemDecoy_gActorInit
PATCH_END
