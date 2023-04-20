#include <combo.h>

void EnMa4_DisplayTextBoxAfterGame(GameState_Play* play, s16 messageId, Actor* actor)
{
    comboSpawnItemGiver(play, NPC_MM_SONG_EPONA);
    PlayerDisplayTextBox(play, messageId, actor);
}

PATCH_CALL(0x80abee64, EnMa4_DisplayTextBoxAfterGame);
