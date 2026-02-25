#include <combo.h>
#include <combo/actor_id.h>

PATCH_START gActorOvl + 0x20 * ACTOR_CUSTOM_TORCH2 + 0x14
.int En_Torch2_InitVars
PATCH_END
