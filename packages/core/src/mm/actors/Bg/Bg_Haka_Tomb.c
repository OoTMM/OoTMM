#include <combo.h>
#include <combo/item.h>
#include <combo/souls.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x15c) = (h); } while (0)

void BgHakaTomb_HandlerNull(Actor* this, GameState_Play* play)
{
}

void BgHakaTomb_LearnSongStorms(Actor* this, GameState_Play* play)
{
    if (!comboHasSoulMm(GI_MM_SOUL_NPC_COMPOSER_BROS))
        return;

    if (gMmExtraFlags.songStorms || Actor_HasParentZ(this))
    {
        gMmExtraFlags.songStorms = 1;
        SET_HANDLER(this, BgHakaTomb_HandlerNull);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_STORMS, NPC_MM_SONG_STORMS, 9999.f, 9999.f);
}

PATCH_FUNC(0x80bd6768, BgHakaTomb_LearnSongStorms);
