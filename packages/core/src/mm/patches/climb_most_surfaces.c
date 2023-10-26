#include <combo.h>

static u32 lastClimbFrame = 0;
static u32 startClimbingTimer = 5;
u32 Player_GetCollisionType(Actor_Player* link, GameState_Play* ctxt, u32 collisionType) {
    if (!comboConfig(CFG_MM_CLIMB_MOST_SURFACES)) {
        return collisionType;
    }

    if ((link->state & (1 << 27)) && !(link->state2 & (1 << 10))) {
        return 8;
    }

    u32 currentClimbFrame = ctxt->gs.frameCount;

    if (currentClimbFrame == lastClimbFrame + 1) {
        startClimbingTimer--;
    } else {
        startClimbingTimer = 5;
    }

    if (startClimbingTimer == 0) {
        return 8;
    }

    lastClimbFrame = currentClimbFrame;

    return collisionType;
}