#include <combo.h>
#include <combo/entrance.h>

/* Warp songs */
static void DemoKankyo_Warp(PlayState* play, void* unk, Actor* this)
{
    u32 entrance;

    gIsEntranceOverride = 1;
    entrance = Entrance_ToRelative(kEntrancesSongs[play->msgCtx.lastPlayedSong]);
    comboTransition(play, entrance);
}

PATCH_CALL(0x80930ab0, DemoKankyo_Warp);
