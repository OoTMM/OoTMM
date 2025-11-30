#include <combo.h>
#include <combo/common/cosmetics.h>

#define IS_PAUSED(pauseCtx) ((pauseCtx)->state != 0)

u8 gNoLowHealthBeep;
EXPORT_SYMBOL(NO_LOW_HEALTH_BEEP, gNoLowHealthBeep);

#define FRAME_TO_BEEP 5

void LifeMeter_UpdateSizeAndBeep(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (interfaceCtx->lifeSizeChangeDirection != 0) {
        interfaceCtx->lifeSizeChange--;
        if (interfaceCtx->lifeSizeChange <= 0) {
            interfaceCtx->lifeSizeChange = 0;
            interfaceCtx->lifeSizeChangeDirection = 0;
            if (!gNoLowHealthBeep && !Player_InCsMode(play) && !IS_PAUSED(&play->pauseCtx) && LifeMeter_IsCritical() &&
                !Play_InCsMode(play)) {
                Audio_PlaySfx(NA_SE_SY_WIN_OPEN);
            }
        }
    } else {
        interfaceCtx->lifeSizeChange++;
        if ((s32)interfaceCtx->lifeSizeChange >= FRAME_TO_BEEP) {
            interfaceCtx->lifeSizeChange = FRAME_TO_BEEP;
            interfaceCtx->lifeSizeChangeDirection = 1;
        }
    }
}

PATCH_FUNC(0x80101844, LifeMeter_UpdateSizeAndBeep);
