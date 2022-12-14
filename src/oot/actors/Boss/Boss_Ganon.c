#include <combo.h>

void BossGanon_LightArrowHint(GameState_Play* play, s16 textId, void* unk)
{
    PlayerDisplayTextBox(play, textId, unk);
    comboTextHijackLightArrows(play);
}

PATCH_CALL(0x809f3af4, BossGanon_LightArrowHint);
