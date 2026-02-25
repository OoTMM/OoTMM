#include <combo.h>
#include <combo/player.h>
#include <combo/config.h>

static u32 lastClimbFrame = 0;
static u32 startClimbingTimer = 5;
u32 Player_GetCollisionType(Player* link, PlayState* ctxt, u32 collisionType) {
    if (!Config_Flag(CFG_MM_CLIMB_MOST_SURFACES)) {
        return collisionType;
    }

    if ((link->stateFlags1 & (1 << 27)) && !(link->stateFlags2 & (1 << 10))) {
        return 8;
    }

    u32 currentClimbFrame = ctxt->state.frameCount;

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
