#include <combo.h>

void EnDai_FallAsleep(Actor* this, GameState_Play* play)
{
    MM_SET_EVENT_WEEK(MM_EV(30, 0));
    ActorDestroy(this);
}

PATCH_FUNC(0x80b3ee8c, EnDai_FallAsleep);
