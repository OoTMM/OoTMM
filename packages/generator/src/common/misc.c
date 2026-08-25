#include <combo.h>
#include <combo/sfx.h>

#if defined(GAME_OOT)
#define PlaySfx Sfx_PlaySfxCentered
#endif
#if defined(GAME_MM)
#define PlaySfx Audio_PlaySfx_2
#endif

static u32 successChimeCooldown = 0;
void Sfx_PlaySuccessChime() {
    if (gPlay->gameplayFrames > successChimeCooldown) {
        PlaySfx(NA_SE_SY_CORRECT_CHIME);
        successChimeCooldown = gPlay->gameplayFrames + 120;
    }
}
