#include <combo.h>
#include <combo/entrance.h>

/* Warp songs */
static void DemoKankyo_Warp(GameState_Play* play, void* unk, Actor* this)
{
    comboTransition(play, gComboData.entrancesSong[play->msgCtx.lastSongPlayed]);
}

PATCH_CALL(0x80930ab0, DemoKankyo_Warp);
