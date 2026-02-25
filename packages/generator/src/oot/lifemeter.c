#include <combo.h>
#include <combo/common/cosmetics.h>

#define IS_PAUSED(pauseCtx) \
    (((pauseCtx)->state != 0 /* PAUSE_STATE_OFF */) || ((pauseCtx)->debugState != 0 /* PAUSE_DEBUG_STATE_CLOSED */))

u8 gNoLowHealthBeep;
EXPORT_SYMBOL(NO_LOW_HEALTH_BEEP, gNoLowHealthBeep);

#define FRAME_TO_BEEP 10

void Health_UpdateBeatingHeart(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (interfaceCtx->beatingHeartOscillatorDirection != 0) {
        interfaceCtx->beatingHeartOscillator--;
        if (interfaceCtx->beatingHeartOscillator <= 0) {
            interfaceCtx->beatingHeartOscillator = 0;
            interfaceCtx->beatingHeartOscillatorDirection = 0;
            if (!gNoLowHealthBeep && !Player_InCsMode(play) && !IS_PAUSED(&play->pauseCtx) && Health_IsCritical() &&
            !Play_InCsMode(play)) {
                Sfx_PlaySfxCentered(NA_SE_SY_HITPOINT_ALARM);
            }
        }
    } else {
        interfaceCtx->beatingHeartOscillator++;
        if (interfaceCtx->beatingHeartOscillator >= FRAME_TO_BEEP) {
            interfaceCtx->beatingHeartOscillator = FRAME_TO_BEEP;
            interfaceCtx->beatingHeartOscillatorDirection = 1;
        }
    }
}

PATCH_FUNC(0x80065a14, Health_UpdateBeatingHeart);
