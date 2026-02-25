#include <combo.h>

/* Hook ParseSceneRoomHeaders_ActorsList */
PATCH_START 0x800f8f74
.int ParseSceneRoomHeaders_ActorsList
PATCH_END
