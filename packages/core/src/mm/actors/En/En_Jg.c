#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x1dc) = (h); } while (0)

void EnJg_HandlerNull(void)
{
}

void EnJg_LearnSongGoronIntro(Actor* this, GameState_Play* play)
{
    if (gMmExtraFlags.songGoronIntro)
    {
        SET_HANDLER(this, EnJg_HandlerNull);
        return;
    }

    if (Actor_HasParent(this))
    {
        gMmExtraFlags.songGoronIntro = 1;
        SET_HANDLER(this, EnJg_HandlerNull);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_GORON_HALF, NPC_MM_GORON_ELDER, 10000.f, 10000.f);
}

PATCH_FUNC(0x80b74b54, EnJg_LearnSongGoronIntro);
