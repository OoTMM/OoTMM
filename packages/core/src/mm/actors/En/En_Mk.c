#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x280) = (h); } while (0)

static void EnMk_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = GI_MM_SONG_ZORA;
    q->id = NPC_MM_SONG_ZORA;
}

void EnMk_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnMk_HandlerZoraEggs(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    if (gSave.playerForm != MM_PLAYER_FORM_ZORA || Actor_HasParent(this))
    {
        SET_HANDLER(this, EnMk_HandlerNull);
        return;
    }

    EnMk_ItemQuery(&q);
    comboGiveItem(this, play, &q, 10000.f, 10000.f);
}

PATCH_FUNC(0x80959d28, EnMk_HandlerZoraEggs);

static void hintZoraEggs(GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnMk_ItemQuery(&q);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you can bring all of the " TEXT_COLOR_BLUE "Zora Eggs ");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "here, you will get ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
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
