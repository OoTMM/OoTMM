#include <combo.h>

void EnHeishi2_AfterZeldaLetter(GameState_Play* play)
{
    Message_Close(play);
    comboRemoveTradeItemChild(XITEM_OOT_CHILD_ZELDA_LETTER);
}

PATCH_CALL(0x8097e120, EnHeishi2_AfterZeldaLetter);
