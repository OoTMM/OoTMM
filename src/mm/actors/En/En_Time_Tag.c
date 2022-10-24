#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x144) = (h); } while (0)

void EnTimeTag_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnTimeTag_GiveItemSoaring(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        gMmExtraFlags.songSoaring = 1;
        SET_HANDLER(this, EnTimeTag_HandlerNull);
        return;
    }

    gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_SOARING, GI_MM_SONG_SOARING);
    GiveItem(this, play, gi, 10000.f, 10000.f);
}

void EnTimeTag_HandlerSoaring(Actor* this, GameState_Play* play)
{
    this->messageId = 0xc02;

    if (gMmExtraFlags.songSoaring)
    {
        SET_HANDLER(this, EnTimeTag_HandlerNull);
        return;
    }

    if (ActorTalkedTo(this))
    {
        Message_Close(play);
        this->attachedA = NULL;
        SET_HANDLER(this, EnTimeTag_GiveItemSoaring);
        EnTimeTag_GiveItemSoaring(this, play);
        return;
    }

    ActorEnableTalk(this, play, 110.f);
}

PATCH_FUNC(0x80aca268, EnTimeTag_HandlerSoaring);
