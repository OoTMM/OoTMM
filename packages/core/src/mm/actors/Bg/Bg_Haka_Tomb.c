#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x15c) = (h); } while (0)

void BgHakaTomb_HandlerNull(Actor* this, GameState_Play* play)
{
}

void BgHakaTomb_LearnSongStorms(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (gMmExtraFlags.songStorms || Actor_HasParent(this))
    {
        gMmExtraFlags.songStorms = 1;
        SET_HANDLER(this, BgHakaTomb_HandlerNull);
        return;
    }

    gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_STORMS, GI_MM_SONG_STORMS);
    GiveItem(this, play, gi, 9999.f, 9999.f);
}

PATCH_FUNC(0x80bd6768, BgHakaTomb_LearnSongStorms);
