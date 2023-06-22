#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x144) = (h); } while (0)

void EnTimeTag_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnTimeTag_GiveItemSoaring(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags.songSoaring = 1;
        SET_HANDLER(this, EnTimeTag_HandlerNull);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_SOARING, NPC_MM_SONG_SOARING, 10000.f, 10000.f);
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

void EnTimeTag_MoonCutscene(Actor* this, GameState_Play* play)
{
    if (comboSpecialCond(SPECIAL_MOON))
    {
        /* Skip to Moon */
        gSave.day = 3;
        gSave.time = 0x4000;
        play->nextEntrance = ((SCE_MM_MOON - 3) << 9);
        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
        play->transitionType = TRANS_TYPE_BLACK;
    }
    else
    {
        PlayerDisplayTextBox(play, 0x2039, NULL);
        SET_HANDLER(this, EnTimeTag_HandlerNull);
    }
}

PATCH_FUNC(0x80ac9fe4, EnTimeTag_MoonCutscene);
