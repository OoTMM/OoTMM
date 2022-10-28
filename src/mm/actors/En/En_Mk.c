#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x280) = (h); } while (0)

void EnMk_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnMk_HandlerZoraEggs(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (gSave.playerForm != MM_PLAYER_FORM_ZORA || Actor_HasParent(this))
    {
        SET_HANDLER(this, EnMk_HandlerNull);
        return;
    }

    gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_ZORA, GI_MM_SONG_ZORA);
    GiveItem(this, play, gi, 10000.f, 10000.f);
}

PATCH_FUNC(0x80959d28, EnMk_HandlerZoraEggs);
