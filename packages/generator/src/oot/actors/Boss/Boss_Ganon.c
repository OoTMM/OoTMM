#include <combo.h>

void BossGanon_LightArrowHint(PlayState* play, s16 textId, void* unk)
{
    PlayerDisplayTextBox(play, textId, unk);
    comboTextHijackLightArrows(play);
}

PATCH_CALL(0x809f3af4, BossGanon_LightArrowHint);
