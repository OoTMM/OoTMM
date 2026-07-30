#include <combo.h>
#include <combo/sfx.h>

#if defined(GAME_OOT)
static u32 successChimeCooldown = 0;
void Sfx_PlaySuccessChime() {
    if (gPlay->gameplayFrames > successChimeCooldown) {
        Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
        successChimeCooldown = gPlay->gameplayFrames + 120;
    }
}
#endif
