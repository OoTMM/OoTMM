#include <combo.h>

/* Warp songs */
static void DemoKankyo_Warp(GameState_Play* play, void* unk, Actor* this)
{
    static const s16 sWarpEntrances[] = {
        0x600,
        0x4f6,
        0x604,
        0x1f1,
        0x568,
        0x5f4,
    };

    TransitionContext* t;

    t = &play->transition;
    t->type = TRANS_TYPE_NORMAL;
    t->gfx = TRANS_GFX_SHORTCUT;
    t->entrance = sWarpEntrances[play->msgCtx.lastSongPlayed];
}

PATCH_CALL(0x80930ab0, DemoKankyo_Warp);
