#include <combo.h>
#include <combo/entrance.h>
#include <combo/config.h>

/* Warp songs */
static void DemoKankyo_Warp(PlayState* play, void* unk, Actor* this)
{
    comboTransition(play, gComboConfig.entrancesSong[play->msgCtx.lastPlayedSong]);
}

PATCH_CALL(0x80930ab0, DemoKankyo_Warp);
