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

static void hintZoraEggs(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SONG_ZORA, GI_MM_SONG_ZORA, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you can bring all of the " TEXT_COLOR_BLUE "Zora Eggs ");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "here, you will get ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

void EnMk_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    switch (messageId)
    {
    case 0xfa0:
    case 0xfa1:
    case 0xfa6:
    case 0xfa7:
    case 0xfa8:
    case 0xfaa:
    case 0xfac:
    case 0xfae:
    case 0xfb0:
    case 0xfb1:
    case 0xfb2:
    case 0xfbf:
        messageId = 0xfbf;
        break;
    default:
        break;
    }

    PlayerDisplayTextBox(play, messageId, this);
    if (messageId != 0xfbf)
        return;

    /* Display the fixed hint */
    hintZoraEggs(play);
}

PATCH_CALL(0x80959a0c, EnMk_DisplayTextBox);
