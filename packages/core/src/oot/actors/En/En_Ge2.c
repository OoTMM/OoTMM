#include <combo.h>

static void EnGe2_SetCaughtWrapper(Actor* this, GameState_Play* play)
{
    ActorFunc EnGe2_SetCaught;

    if (GET_LINK(play)->mask == PLAYER_MASK_STONE)
        return;

    EnGe2_SetCaught = actorAddr(AC_EN_GE2, 0x80b24960);
    EnGe2_SetCaught(this, play);
}

PATCH_CALL(0x80b24d4c, EnGe2_SetCaughtWrapper);
PATCH_CALL(0x80b24d84, EnGe2_SetCaughtWrapper);
